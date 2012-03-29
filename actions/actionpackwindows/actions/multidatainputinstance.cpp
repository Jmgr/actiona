/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
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

namespace Actions
{
	ActionTools::StringListPair MultiDataInputInstance::modes = qMakePair(
			QStringList() << "comboBox" << "editableComboBox" << "list" << "checkbox" << "radioButton",
			QStringList()
			<< QT_TRANSLATE_NOOP("MultiDataInputInstance::modes", "ComboBox")
			<< QT_TRANSLATE_NOOP("MultiDataInputInstance::modes", "Editable ComboBox")
			<< QT_TRANSLATE_NOOP("MultiDataInputInstance::modes", "List")
			<< QT_TRANSLATE_NOOP("MultiDataInputInstance::modes", "CheckBox")
			<< QT_TRANSLATE_NOOP("MultiDataInputInstance::modes", "RadioButton"));

	MultiDataInputInstance::MultiDataInputInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		  mDialog(0)
	{
		//TODO: Add windowTitle to this and DataInput
	}

	void MultiDataInputInstance::startExecution()
	{
		bool ok = true;

		QString question = evaluateString(ok, "question");
		Mode mode = evaluateListElement<Mode>(ok, modes, "mode");
		QStringList items = evaluateItemList(ok, "items");
		QString defaultValue = evaluateString(ok, "defaultValue");
		QString variable = evaluateVariable(ok, "variable");
		QString windowIcon = evaluateString(ok, "windowIcon");
		int minimumChoiceCount = evaluateInteger(ok, "minimumChoiceCount");
		int maximumChoiceCount = evaluateInteger(ok, "maximumChoiceCount");
		QString inputRegexp = evaluateString(ok, "inputRegexp");

		if(!ok)
			return;

		mDialog = new QDialog;
		QVBoxLayout *layout = new QVBoxLayout(mDialog);

		mDialog->setLayout(layout);

		QLabel *questionLabel = new QLabel(mDialog);
		questionLabel->setText(question);
		layout->addWidget(questionLabel);

		switch(mode)
		{
		case ComboBoxMode:
		case EditableComboBoxMode:
			{
				QComboBox *comboBox = new QComboBox(mDialog);
				comboBox->addItems(items);

				int currentItem = comboBox->findText(defaultValue);
				if(currentItem == -1)
					currentItem = 0;
				comboBox->setCurrentIndex(currentItem);

				comboBox->setEditable(mode == EditableComboBoxMode);

				if(mode == EditableComboBoxMode)
				{
					QRegExp regExp(inputRegexp);
					if(!regExp.isValid())
					{
						//TODO: Exception here
					}

					comboBox->lineEdit()->setValidator(new QRegExpValidator(regExp, comboBox));
				}

				layout->addWidget(comboBox);
			}
			break;
		case ListMode:
			{
				QListWidget *listWidget = new QListWidget(mDialog);

				listWidget->addItems(items);

				QList<QListWidgetItem *> defaultItem = listWidget->findItems(defaultValue, Qt::MatchExactly);
				if(!defaultItem.isEmpty())
					listWidget->setCurrentItem(defaultItem.first());

				layout->addWidget(listWidget);
			}
			break;
		case CheckboxMode:
			{
				QButtonGroup *buttonGroup = new QButtonGroup(mDialog);
				buttonGroup->setExclusive(false);

				int itemCount = items.size();
				QGridLayout *gridLayout = new QGridLayout;

				for(int i = 0, row = 0, col = 0; i < itemCount; ++i)
				{
					QString text = items.at(i);
					QCheckBox *checkBox = new QCheckBox(text, mDialog);

					if(defaultValue == text)
						checkBox->setChecked(true);

					gridLayout->addWidget(checkBox, row, col);
					buttonGroup->addButton(checkBox);

					if(col == 3)
					{
						col = 0;
						++row;
					}
					else
						++col;
				}

				layout->addLayout(gridLayout);
			}
			break;
		case RadioButtonMode:
			{
				QButtonGroup *buttonGroup = new QButtonGroup(mDialog);

				int itemCount = items.size();
				QGridLayout *gridLayout = new QGridLayout;

				for(int i = 0, row = 0, col = 0; i < itemCount; ++i)
				{
					QString text = items.at(i);
					QRadioButton *radioButton = new QRadioButton(text, mDialog);

					if(defaultValue == text)
						radioButton->setChecked(true);

					gridLayout->addWidget(radioButton, row, col);
					buttonGroup->addButton(radioButton);

					if(col == 3)
					{
						col = 0;
						++row;
					}
					else
						++col;
				}

				layout->addLayout(gridLayout);
			}
			break;
		}

		QDialogButtonBox *dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, mDialog);
		layout->addWidget(dialogButtonBox);

		connect(dialogButtonBox, SIGNAL(accepted()), mDialog, SLOT(accept()));
		connect(dialogButtonBox, SIGNAL(rejected()), mDialog, SLOT(reject()));
		connect(mDialog, SIGNAL(accepted()), this, SLOT(accepted()));
		connect(mDialog, SIGNAL(rejected()), this, SLOT(rejected()));

		mDialog->show();
	}

	void MultiDataInputInstance::stopExecution()
	{
		closeDialog();
	}

	void MultiDataInputInstance::accepted()
	{
		//TODO: Save selection here

		closeDialog();

		emit executionEnded();
	}

	void MultiDataInputInstance::rejected()
	{
		closeDialog();

		emit executionEnded();
	}

	void MultiDataInputInstance::closeDialog()
	{
		if(mDialog)
		{
			mDialog->close();
			mDialog->deleteLater();
			mDialog = 0;
		}
	}
}
