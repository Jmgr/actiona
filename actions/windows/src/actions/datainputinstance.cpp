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

	Contact: jmgr@jmgr.info
*/

#include "datainputinstance.hpp"
#include "actiontools/script.hpp"

#include <QInputDialog>
#include <QApplication>
#include <QIcon>
#include <QLabel>
#include <QScreen>

#include <limits>

namespace Actions
{
    Tools::StringListPair DataInputInstance::dataTypes =
    {
        {
            QStringLiteral("text"),
            QStringLiteral("integer"),
            QStringLiteral("decimal")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("DataInputInstance::dataTypes", "Text")),
            QStringLiteral(QT_TRANSLATE_NOOP("DataInputInstance::dataTypes", "Integer")),
            QStringLiteral(QT_TRANSLATE_NOOP("DataInputInstance::dataTypes", "Decimal"))
        }
    };

    Tools::StringListPair DataInputInstance::editorTypes =
    {
        {
            QStringLiteral("line"),
            QStringLiteral("multiline"),
            QStringLiteral("password")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("DataInputInstance::editorTypes", "Line")),
            QStringLiteral(QT_TRANSLATE_NOOP("DataInputInstance::editorTypes", "Multiline")),
            QStringLiteral(QT_TRANSLATE_NOOP("DataInputInstance::editorTypes", "Password"))
        }
    };

	DataInputInstance::DataInputInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		mInputDialog(nullptr)
	{
	}

	void DataInputInstance::startExecution()
	{
		bool ok = true;

		QString question = evaluateString(ok, QStringLiteral("question"));
		mDataType = evaluateListElement<DataType>(ok, dataTypes, QStringLiteral("dataType"));
		auto editorType = evaluateListElement<EditorType>(ok, editorTypes, QStringLiteral("editorType"));
		mVariable = evaluateVariable(ok, QStringLiteral("variable"));
		QString windowTitle = evaluateString(ok, QStringLiteral("windowTitle"));
		QImage windowIcon = evaluateImage(ok, QStringLiteral("windowIcon"));

		if(!ok)
			return;

		QString textDefaultValue;
		double decimalDefaultValue = 0.0;
		int integerDefaultValue = 0;

		mInputDialog = nullptr;

		switch(mDataType)
		{
		case IntegerType:
			integerDefaultValue = evaluateInteger(ok, QStringLiteral("defaultValue"));
			if(!ok)
				return;
			break;
		case DecimalType:
			decimalDefaultValue = evaluateDouble(ok, QStringLiteral("defaultValue"));
			if(!ok)
				return;
			break;
		default:
			textDefaultValue = evaluateString(ok, QStringLiteral("defaultValue"));
			if(!ok)
				return;
			break;
		}

		mInputDialog = new QInputDialog();

        mInputDialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
		mInputDialog->setWindowModality(Qt::NonModal);
		mInputDialog->setLabelText(question);
		mInputDialog->setWindowTitle(windowTitle);

        switch(editorType)
        {
        case LineEditorType:
            break;
        case MultilineEditorType:
            mInputDialog->setOption(QInputDialog::UsePlainTextEditForTextInput);
            break;
        case PasswordEditorType:
            mInputDialog->setTextEchoMode(QLineEdit::Password);
            break;
        }

        if(!windowIcon.isNull())
            mInputDialog->setWindowIcon(QIcon(QPixmap::fromImage(windowIcon)));

		switch(mDataType)
		{
		case IntegerType:
			mInputDialog->setInputMode(QInputDialog::IntInput);
			mInputDialog->setIntRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
            mInputDialog->setIntValue(integerDefaultValue);
			break;
		case DecimalType:
			mInputDialog->setInputMode(QInputDialog::DoubleInput);
			mInputDialog->setDoubleRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
            mInputDialog->setDoubleDecimals(4);
            mInputDialog->setDoubleValue(decimalDefaultValue);
			break;
		default:
			mInputDialog->setInputMode(QInputDialog::TextInput);
			mInputDialog->setTextValue(textDefaultValue);
			break;
		}

		mInputDialog->adjustSize();
        auto screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();
		mInputDialog->move(screenGeometry.center());
		mInputDialog->move(mInputDialog->pos().x() - mInputDialog->width()/2, mInputDialog->pos().y() - mInputDialog->height()/2);

        const auto labels = mInputDialog->findChildren<QLabel*>();
        for(QLabel *label: labels)
            label->setOpenExternalLinks(true);

		switch(mDataType)
		{
		case IntegerType:
			mInputDialog->open(this, SLOT(dataEntered(int)));
			break;
		case DecimalType:
			mInputDialog->open(this, SLOT(dataEntered(double)));
			break;
		default:
			mInputDialog->open(this, SLOT(dataEntered(const QString &)));
			break;
		}
        connect(mInputDialog, &QInputDialog::rejected, this, &DataInputInstance::canceled);
	}

	void DataInputInstance::stopExecution()
	{
		if(mInputDialog)
		{
			mInputDialog->close();
			mInputDialog = nullptr;
		}
	}

	void DataInputInstance::dataEntered(int value)
	{
        setVariable(mVariable, value);

		endExecution();
	}

	void DataInputInstance::dataEntered(double value)
	{
        setVariable(mVariable, value);

		endExecution();
	}

	void DataInputInstance::dataEntered(const QString &value)
	{
        setVariable(mVariable, value);

		endExecution();
	}

	void DataInputInstance::canceled()
	{
		//TODO: add an exception to trigger when canceled
        setVariable(mVariable, QString{});

		endExecution();
	}

	void DataInputInstance::endExecution()
	{
		mInputDialog->disconnect();
		mInputDialog->close();

		executionEnded();
	}
}
