/*
	Actionaz
    Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#include "datainputinstance.h"
#include "script.h"

#include <QInputDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QIcon>

#include <limits>

namespace Actions
{
	ActionTools::StringListPair DataInputInstance::dataTypes = qMakePair(
		QStringList() << "text" << "integer" << "decimal",
		QStringList()
		<< QT_TRANSLATE_NOOP("DataInputInstance::dataTypes", "Text")
		<< QT_TRANSLATE_NOOP("DataInputInstance::dataTypes", "Integer")
		<< QT_TRANSLATE_NOOP("DataInputInstance::dataTypes", "Decimal"));

	DataInputInstance::DataInputInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		mInputDialog(0)
	{
	}

	void DataInputInstance::startExecution()
	{
		bool ok = true;

		QString question = evaluateString(ok, "question");
		mDataType = evaluateListElement<DataType>(ok, dataTypes, "dataType");
		mVariable = evaluateVariable(ok, "variable");
		QString windowTitle = evaluateString(ok, "windowTitle");
        QImage windowIcon = evaluateImage(ok, "windowIcon");

		if(!ok)
			return;

		QString textDefaultValue;
		double decimalDefaultValue = 0.0;
		int integerDefaultValue = 0;

		mInputDialog = 0;

		switch(mDataType)
		{
		case IntegerType:
			integerDefaultValue = evaluateInteger(ok, "defaultValue");
			if(!ok)
				return;
			break;
		case DecimalType:
			decimalDefaultValue = evaluateDouble(ok, "defaultValue");
			if(!ok)
				return;
			break;
		default:
			textDefaultValue = evaluateString(ok, "defaultValue");
			if(!ok)
				return;
			break;
		}

		mInputDialog = new QInputDialog();

		mInputDialog->setWindowModality(Qt::NonModal);
		mInputDialog->setLabelText(question);
		mInputDialog->setWindowTitle(windowTitle);

        if(!windowIcon.isNull())
            mInputDialog->setWindowIcon(QIcon(QPixmap::fromImage(windowIcon)));

		switch(mDataType)
		{
		case IntegerType:
			mInputDialog->setInputMode(QInputDialog::IntInput);
			mInputDialog->setIntValue(integerDefaultValue);
			mInputDialog->setIntRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
			break;
		case DecimalType:
			mInputDialog->setInputMode(QInputDialog::DoubleInput);
			mInputDialog->setDoubleValue(decimalDefaultValue);
			mInputDialog->setDoubleRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
			mInputDialog->setDoubleDecimals(4);
			break;
		default:
			mInputDialog->setInputMode(QInputDialog::TextInput);
			mInputDialog->setTextValue(textDefaultValue);
			break;
		}

		mInputDialog->adjustSize();
		QRect screenGeometry = QApplication::desktop()->availableGeometry();
		mInputDialog->move(screenGeometry.center());
		mInputDialog->move(mInputDialog->pos().x() - mInputDialog->width()/2, mInputDialog->pos().y() - mInputDialog->height()/2);

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
		connect(mInputDialog, SIGNAL(rejected()), this, SLOT(canceled()));
	}

	void DataInputInstance::stopExecution()
	{
		if(mInputDialog)
		{
			mInputDialog->close();
			mInputDialog = 0;
		}
	}

	void DataInputInstance::dataEntered(int value)
	{
        setVariable(mVariable, value);

		mInputDialog->disconnect();
		mInputDialog->close();

		emit executionEnded();
	}

	void DataInputInstance::dataEntered(double value)
	{
        setVariable(mVariable, value);

		mInputDialog->disconnect();
		mInputDialog->close();

		emit executionEnded();
	}

	void DataInputInstance::dataEntered(const QString &value)
	{
        setVariable(mVariable, value);

		mInputDialog->disconnect();
		mInputDialog->close();

		emit executionEnded();
	}

	void DataInputInstance::canceled()
	{
		//TODO: add an exception to trigger when canceled
		dataEntered(QString());
	}
}
