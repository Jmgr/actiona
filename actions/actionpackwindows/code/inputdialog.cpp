/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#include "inputdialog.h"

#include <QScriptValueIterator>
#include <QInputDialog>

namespace Code
{
	QScriptValue InputDialog::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		InputDialog *inputDialog = new InputDialog;
		inputDialog->setupConstructorParameters(context->argument(0));

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == "labelText")
				inputDialog->mInputDialog->setLabelText(it.value().toString());
			else if(it.name() == "okButtonText")
				inputDialog->mInputDialog->setOkButtonText(it.value().toString());
			else if(it.name() == "cancelButtonText")
				inputDialog->mInputDialog->setCancelButtonText(it.value().toString());
			else if(it.name() == "textEchoMode")
				inputDialog->mInputDialog->setTextEchoMode(static_cast<QLineEdit::EchoMode>(it.value().toInt32()));
			else if(it.name() == "floatDecimals")
				inputDialog->mInputDialog->setDoubleDecimals(it.value().toInt32());
			else if(it.name() == "integerStep")
				inputDialog->mInputDialog->setIntStep(it.value().toInt32());
			else if(it.name() == "minimum")
				inputDialog->mMinimum = it.value();
			else if(it.name() == "maximum")
				inputDialog->mMaximum = it.value();
			else if(it.name() == "range")
			{
				inputDialog->mMinimum = it.value().property("minimum");
				inputDialog->mMaximum = it.value().property("maximum");
			}
			else if(it.name() == "inputType")
				inputDialog->mInputType = static_cast<InputType>(it.value().toInt32());
			else if(it.name() == "value")
				inputDialog->mValue = it.value();
			else if(it.name() == "items")
				inputDialog->mItems = it.value();
			else if(it.name() == "itemsEditable")
				inputDialog->mInputDialog->setComboBoxEditable(it.value().toBool());
			else if(it.name() == "onClosed")
				inputDialog->mOnClosed = it.value();
			else if(it.name() == "onValueChanged")
				inputDialog->mOnValueChanged = it.value();
		}

		return inputDialog->mThisObject = CodeClass::constructor(inputDialog, context, engine);
	}

	InputDialog::InputDialog()
		: BaseWindow(),
		mInputType(Text),
		mInputDialog(new QInputDialog)
	{
		setWidget(mInputDialog);
		
		connect(mInputDialog, SIGNAL(finished(int)), this, SLOT(finished(int)));
		connect(mInputDialog, SIGNAL(doubleValueChanged(double)), this, SLOT(doubleValueChanged(double)));
		connect(mInputDialog, SIGNAL(intValueChanged(int)), this, SLOT(intValueChanged(int)));
		connect(mInputDialog, SIGNAL(textValueChanged(const QString &)), this, SLOT(textValueChanged(const QString &)));
	}
	
	InputDialog::~InputDialog()
	{
		delete mInputDialog;
	}
	
	QScriptValue InputDialog::setLabelText(const QString &labelText)
	{
		mInputDialog->setLabelText(labelText);

		return context()->thisObject();
	}
	
	QScriptValue InputDialog::setOkButtonText(const QString &okButtonText)
	{
		mInputDialog->setOkButtonText(okButtonText);
		
		return context()->thisObject();
	}

	QScriptValue InputDialog::setCancelButtonText(const QString &cancelButtonText)
	{
		mInputDialog->setCancelButtonText(cancelButtonText);
		
		return context()->thisObject();
	}

	QScriptValue InputDialog::setTextEchoMode(TextEchoMode textEchoMode)
	{
		mInputDialog->setTextEchoMode(static_cast<QLineEdit::EchoMode>(textEchoMode));
		
		return context()->thisObject();
	}

	QScriptValue InputDialog::setFloatDecimals(int decimals)
	{
		mInputDialog->setDoubleDecimals(decimals);
		
		return context()->thisObject();
	}

	QScriptValue InputDialog::setIntegerStep(int step)
	{
		mInputDialog->setIntStep(step);
		
		return context()->thisObject();
	}

	QScriptValue InputDialog::setMaximum(const QScriptValue &maximum)
	{
		mMaximum = maximum;
		
		return context()->thisObject();
	}

	QScriptValue InputDialog::setMinimum(const QScriptValue &minimum)
	{
		mMinimum = minimum;
		
		return context()->thisObject();
	}

	QScriptValue InputDialog::setRange(const QScriptValue &minimum, const QScriptValue &maximum)
	{
		mMinimum = minimum;
		mMaximum = maximum;
		
		return context()->thisObject();
	}
	
	QScriptValue InputDialog::setInputType(InputType inputType)
	{
		mInputType = inputType;
		
		return context()->thisObject();
	}
	
	QScriptValue InputDialog::setValue(const QScriptValue &value)
	{
		mValue = value;
		
		return context()->thisObject();
	}
	
	QScriptValue InputDialog::setItems(const QScriptValue &items)
	{
		mItems = items;
		
		return context()->thisObject();
	}
	
	QScriptValue InputDialog::setItemsEditable(bool itemsEditable)
	{
		mInputDialog->setComboBoxEditable(itemsEditable);
		
		return context()->thisObject();
	}
	
	QScriptValue InputDialog::show()
	{
		setup();
		
		mInputDialog->open();

		return context()->thisObject();
	}

	int InputDialog::showModal()
	{
		setup();
		
		return mInputDialog->exec();
	}
	
	QScriptValue InputDialog::value() const
	{
		switch(mInputType)
		{
		case Integer:
			return engine()->newVariant(mInputDialog->intValue());
		case Float:
			return engine()->newVariant(mInputDialog->doubleValue());
		case Items:
		case Text:
		default:
			return engine()->newVariant(mInputDialog->textValue());
			break;
		}
	}
	
	void InputDialog::finished(int result)
	{
		if(mOnClosed.isValid())
			mOnClosed.call(mThisObject, QScriptValueList() << result);
	}
	
	void InputDialog::doubleValueChanged(double value)
	{
		if(mOnValueChanged.isValid())
			mOnValueChanged.call(mThisObject, QScriptValueList() << value);
	}

	void InputDialog::intValueChanged(int value)
	{
		if(mOnValueChanged.isValid())
			mOnValueChanged.call(mThisObject, QScriptValueList() << value);
	}

	void InputDialog::textValueChanged(const QString &value)
	{
		if(mOnValueChanged.isValid())
			mOnValueChanged.call(mThisObject, QScriptValueList() << value);
	}
	
	void InputDialog::setup()
	{
		switch(mInputType)
		{
		case Integer:
			mInputDialog->setInputMode(QInputDialog::IntInput);
			mInputDialog->setIntValue(mValue.toInt32());
			if(mMinimum.isValid())
				mInputDialog->setIntMinimum(mMinimum.toInt32());
			if(mMaximum.isValid())
				mInputDialog->setIntMaximum(mMaximum.toInt32());
			break;
		case Float:
			mInputDialog->setInputMode(QInputDialog::DoubleInput);
			mInputDialog->setDoubleValue(mValue.toNumber());
			if(mMinimum.isValid())
				mInputDialog->setDoubleMinimum(mMinimum.toNumber());
			if(mMaximum.isValid())
				mInputDialog->setDoubleMaximum(mMaximum.toNumber());
			break;
		case Items:
			{
				QScriptValueIterator it(mItems);
				QStringList values;
				while(it.hasNext())
				{
					it.next();
					
					if (it.flags() & QScriptValue::SkipInEnumeration)
							 continue;
					
					values << it.value().toString();
				}
				mInputDialog->setComboBoxItems(values);
				
				mInputDialog->setTextValue(mValue.toString());
			}
			break;
		case Text:
		default:
			mInputDialog->setInputMode(QInputDialog::TextInput);
			mInputDialog->setTextValue(mValue.toString());
			break;
		}
	}
}
