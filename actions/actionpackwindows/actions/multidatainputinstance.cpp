/*
	Actiona
    Copyright (C) 2005 Jonathan Mercier-Ganady

	Actiona is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actiona is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#include "multidatainputinstance.h"
#include "script.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QCheckBox>
#include <QRadioButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QRegExp>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QButtonGroup>
#include <QScriptEngine>

namespace Actions
{
    Tools::StringListPair MultiDataInputInstance::modes =
    {
        {
            QStringLiteral("comboBox"),
            QStringLiteral("editableComboBox"),
            QStringLiteral("list"),
            QStringLiteral("checkbox"),
            QStringLiteral("radioButton")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("MultiDataInputInstance::modes", "ComboBox")),
            QStringLiteral(QT_TRANSLATE_NOOP("MultiDataInputInstance::modes", "Editable ComboBox")),
            QStringLiteral(QT_TRANSLATE_NOOP("MultiDataInputInstance::modes", "List")),
            QStringLiteral(QT_TRANSLATE_NOOP("MultiDataInputInstance::modes", "CheckBox")),
            QStringLiteral(QT_TRANSLATE_NOOP("MultiDataInputInstance::modes", "RadioButton"))
        }
    };

	MultiDataInputInstance::MultiDataInputInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		  mDialog(nullptr),
		  mMode(ComboBoxMode),
		  mMinimumChoiceCount(1),
		  mMaximumChoiceCount(1),
		  mComboBox(nullptr),
		  mListWidget(nullptr),
		  mButtonGroup(nullptr)
	{
	}

	void MultiDataInputInstance::startExecution()
	{
		bool ok = true;

		QString question = evaluateString(ok, QStringLiteral("question"));
		mMode = evaluateListElement<Mode>(ok, modes, QStringLiteral("mode"));
		mItems = evaluateItemList(ok, QStringLiteral("items"));
		QString defaultValue = evaluateString(ok, QStringLiteral("defaultValue"));
		mVariable = evaluateVariable(ok, QStringLiteral("variable"));
		QString windowTitle = evaluateString(ok, QStringLiteral("windowTitle"));
		QImage windowIcon = evaluateImage(ok, QStringLiteral("windowIcon"));
		mMaximumChoiceCount = evaluateInteger(ok, QStringLiteral("maximumChoiceCount"));

		if(!ok)
			return;

		if(mDialog)
			delete mDialog;

		mDialog = new QDialog;

        mDialog->setWindowFlags(mDialog->windowFlags() | Qt::WindowContextHelpButtonHint);

		auto layout = new QVBoxLayout(mDialog);

		mDialog->setLayout(layout);
		mDialog->setWindowTitle(windowTitle);

        if(!windowIcon.isNull())
            mDialog->setWindowIcon(QIcon(QPixmap::fromImage(windowIcon)));

		QLabel *questionLabel = new QLabel(mDialog);
		questionLabel->setText(question);
		layout->addWidget(questionLabel);

		switch(mMode)
		{
		case ComboBoxMode:
		case EditableComboBoxMode:
			{
				mComboBox = new QComboBox(mDialog);
				mComboBox->addItems(mItems);

				int currentItem = mComboBox->findText(defaultValue);
				if(currentItem == -1)
					currentItem = 0;
				mComboBox->setCurrentIndex(currentItem);

				mComboBox->setEditable(mMode == EditableComboBoxMode);

				layout->addWidget(mComboBox);
			}
			break;
		case ListMode:
			{
				mListWidget = new QListWidget(mDialog);

				if(mMaximumChoiceCount <= 1)
					mListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
				else
					mListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

				mListWidget->addItems(mItems);

				QList<QListWidgetItem *> defaultItem = mListWidget->findItems(defaultValue, Qt::MatchExactly);
				if(!defaultItem.isEmpty())
					mListWidget->setCurrentItem(defaultItem.first());

				layout->addWidget(mListWidget);

				if(mMaximumChoiceCount > 1)
                    connect(mListWidget, &QListWidget::itemSelectionChanged, this, &MultiDataInputInstance::listItemSelectionChanged);
			}
			break;
		case CheckboxMode:
			layout->addLayout(createRadioButtonsOrCheckboxes<QCheckBox>(defaultValue, mMaximumChoiceCount <= 1));
			break;
		case RadioButtonMode:
			layout->addLayout(createRadioButtonsOrCheckboxes<QRadioButton>(defaultValue, true));
			break;
		}

        if(mMode != ListMode)
            layout->addStretch(1);

		QDialogButtonBox *dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, mDialog);
		layout->addWidget(dialogButtonBox);

        connect(dialogButtonBox, &QDialogButtonBox::accepted, mDialog, &QDialog::accept);
        connect(dialogButtonBox, &QDialogButtonBox::rejected, mDialog, &QDialog::reject);
        connect(mDialog, &QDialog::accepted, this, &MultiDataInputInstance::accepted);
        connect(mDialog, &QDialog::rejected, this, &MultiDataInputInstance::rejected);

        for(QLabel *label: mDialog->findChildren<QLabel*>())
            label->setOpenExternalLinks(true);

		mDialog->show();
	}

	void MultiDataInputInstance::stopExecution()
	{
		closeDialog();
	}

	void MultiDataInputInstance::accepted()
	{
		switch(mMode)
		{
		case ComboBoxMode:
		case EditableComboBoxMode:
            setVariable(mVariable, mComboBox->currentText());
			break;
		case ListMode:
			{
				QList<QListWidgetItem *> selectedItems = mListWidget->selectedItems();

				if(mMaximumChoiceCount <= 1)
				{
					if(selectedItems.isEmpty())
                        setVariable(mVariable, scriptEngine()->nullValue());
					else
                        setVariable(mVariable, selectedItems.first()->text());
				}
				else
				{
					QScriptValue back = scriptEngine()->newArray(selectedItems.size());

					for(int index = 0; index < selectedItems.size(); ++index)
						back.setProperty(index, selectedItems.at(index)->text());

                    setVariable(mVariable, back);
				}
			}
			break;
		case CheckboxMode:
			if(mMaximumChoiceCount <= 1)
				saveSelectedRadioButtonOrCheckBox();
			else
			{
				QStringList selectedButtons;

                for(QAbstractButton *button: mButtonGroup->buttons())
				{
					if(button->isChecked())
						selectedButtons.append(button->text());
				}

				QScriptValue back = scriptEngine()->newArray(selectedButtons.size());

				for(int index = 0; index < selectedButtons.size(); ++index)
					back.setProperty(index, selectedButtons.at(index));

                setVariable(mVariable, back);
			}
			break;
		case RadioButtonMode:
			{
				saveSelectedRadioButtonOrCheckBox();
			}
			break;
		}

		closeDialog();

		executionEnded();
	}

	void MultiDataInputInstance::rejected()
	{
		closeDialog();

		executionEnded();
	}

	void MultiDataInputInstance::listItemSelectionChanged()
	{
		if(mMaximumChoiceCount <= 1)
			return;

		QList<QListWidgetItem *> selectedItems = mListWidget->selectedItems();
		int itemsToDeselect = selectedItems.size() - mMaximumChoiceCount;

		for(int itemIndex = 0; itemIndex < selectedItems.size() && itemsToDeselect > 0; ++itemIndex, --itemsToDeselect)
			selectedItems.at(itemIndex)->setSelected(false);
	}

	void MultiDataInputInstance::checkboxChecked(QAbstractButton *checkbox)
	{
		int checkedButtonCount = 0;

        for(QAbstractButton *button: mButtonGroup->buttons())
		{
			if(button->isChecked())
				++checkedButtonCount;
		}

		if(checkedButtonCount > mMaximumChoiceCount)
			checkbox->setChecked(false);
	}

	template<class T>
	QGridLayout *MultiDataInputInstance::createRadioButtonsOrCheckboxes(const QString &defaultValue, bool exclusive)
	{
		mButtonGroup = new QButtonGroup(mDialog);
		mButtonGroup->setExclusive(exclusive);

		if(!exclusive && mMaximumChoiceCount > 1)
            connect(mButtonGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked), this, &MultiDataInputInstance::checkboxChecked);

		int itemCount = mItems.size();
		auto gridLayout = new QGridLayout;

		for(int i = 0, row = 0, col = 0; i < itemCount; ++i)
		{
			QString text = mItems.at(i);
			T *itemWidget = new T(text, mDialog);

			if(defaultValue == text)
				itemWidget->setChecked(true);

			gridLayout->addWidget(itemWidget, row, col);
			mButtonGroup->addButton(itemWidget);

			if(col == 3)
			{
				col = 0;
				++row;
			}
			else
				++col;
		}

		return gridLayout;
	}

	void MultiDataInputInstance::saveSelectedRadioButtonOrCheckBox()
	{
		QAbstractButton *checkedButton = mButtonGroup->checkedButton();
		if(checkedButton)
            setVariable(mVariable, checkedButton->text());
		else
            setVariable(mVariable, scriptEngine()->nullValue());
	}

	void MultiDataInputInstance::closeDialog()
	{
		if(mDialog)
		{
			mDialog->close();
			mDialog = nullptr;
		}
	}
}
