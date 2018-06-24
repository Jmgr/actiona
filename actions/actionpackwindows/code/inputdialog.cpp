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

#include "inputdialog.h"

#include <QScriptValueIterator>
#include <QInputDialog>

namespace Code
{
	QScriptValue InputDialog::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		auto inputDialog = new InputDialog;
		inputDialog->setupConstructorParameters(context, engine, context->argument(0));

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == QLatin1String("labelText"))
				inputDialog->mInputDialog->setLabelText(it.value().toString());
			else if(it.name() == QLatin1String("okButtonText"))
				inputDialog->mInputDialog->setOkButtonText(it.value().toString());
			else if(it.name() == QLatin1String("cancelButtonText"))
				inputDialog->mInputDialog->setCancelButtonText(it.value().toString());
			else if(it.name() == QLatin1String("textEchoMode"))
				inputDialog->mInputDialog->setTextEchoMode(static_cast<QLineEdit::EchoMode>(it.value().toInt32()));
			else if(it.name() == QLatin1String("floatDecimals"))
				inputDialog->mInputDialog->setDoubleDecimals(it.value().toInt32());
			else if(it.name() == QLatin1String("integerStep"))
				inputDialog->mInputDialog->setIntStep(it.value().toInt32());
			else if(it.name() == QLatin1String("minimum"))
				inputDialog->mMinimum = it.value();
			else if(it.name() == QLatin1String("maximum"))
				inputDialog->mMaximum = it.value();
			else if(it.name() == QLatin1String("range"))
			{
				inputDialog->mMinimum = it.value().property(QStringLiteral("minimum"));
				inputDialog->mMaximum = it.value().property(QStringLiteral("maximum"));
			}
			else if(it.name() == QLatin1String("inputType"))
				inputDialog->mInputType = static_cast<InputType>(it.value().toInt32());
			else if(it.name() == QLatin1String("value"))
				inputDialog->mValue = it.value();
			else if(it.name() == QLatin1String("items"))
				inputDialog->mItems = it.value();
			else if(it.name() == QLatin1String("itemsEditable"))
				inputDialog->mInputDialog->setComboBoxEditable(it.value().toBool());
			else if(it.name() == QLatin1String("onClosed"))
				inputDialog->mOnClosed = it.value();
			else if(it.name() == QLatin1String("onValueChanged"))
				inputDialog->mOnValueChanged = it.value();
		}

		return CodeClass::constructor(inputDialog, context, engine);
	}

	InputDialog::InputDialog()
		: BaseWindow(),
		
		mInputDialog(new QInputDialog)
	{
        mInputDialog->setWindowFlags(mInputDialog->windowFlags() | Qt::WindowContextHelpButtonHint);

		setWidget(mInputDialog);
		
        connect(mInputDialog, &QInputDialog::finished, this, &InputDialog::finished);
        connect(mInputDialog, &QInputDialog::doubleValueChanged, this, &InputDialog::doubleValueChanged);
        connect(mInputDialog, &QInputDialog::intValueChanged, this, &InputDialog::intValueChanged);
        connect(mInputDialog, &QInputDialog::textValueChanged, this, &InputDialog::textValueChanged);
	}
	
	InputDialog::~InputDialog()
	{
		delete mInputDialog;
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
        case MultilineText:
		default:
			return engine()->newVariant(mInputDialog->textValue());
			break;
		}
	}
	
	QScriptValue InputDialog::setLabelText(const QString &labelText)
	{
		mInputDialog->setLabelText(labelText);

		return thisObject();
	}
	
	QScriptValue InputDialog::setOkButtonText(const QString &okButtonText)
	{
		mInputDialog->setOkButtonText(okButtonText);
		
		return thisObject();
	}

	QScriptValue InputDialog::setCancelButtonText(const QString &cancelButtonText)
	{
		mInputDialog->setCancelButtonText(cancelButtonText);
		
		return thisObject();
	}

	QScriptValue InputDialog::setTextEchoMode(TextEchoMode textEchoMode)
	{
		mInputDialog->setTextEchoMode(static_cast<QLineEdit::EchoMode>(textEchoMode));
		
		return thisObject();
	}

	QScriptValue InputDialog::setFloatDecimals(int decimals)
	{
		mInputDialog->setDoubleDecimals(decimals);
		
		return thisObject();
	}

	QScriptValue InputDialog::setIntegerStep(int step)
	{
		mInputDialog->setIntStep(step);
		
		return thisObject();
	}

	QScriptValue InputDialog::setMaximum(const QScriptValue &maximum)
	{
		mMaximum = maximum;
		
		return thisObject();
	}

	QScriptValue InputDialog::setMinimum(const QScriptValue &minimum)
	{
		mMinimum = minimum;
		
		return thisObject();
	}

	QScriptValue InputDialog::setRange(const QScriptValue &minimum, const QScriptValue &maximum)
	{
		mMinimum = minimum;
		mMaximum = maximum;
		
		return thisObject();
	}
	
	QScriptValue InputDialog::setInputType(InputType inputType)
	{
		mInputType = inputType;
		
		return thisObject();
	}
	
	QScriptValue InputDialog::setValue(const QScriptValue &value)
	{
		mValue = value;
		
		return thisObject();
	}
	
	QScriptValue InputDialog::setItems(const QScriptValue &items)
	{
		mItems = items;
		
		return thisObject();
	}
	
	QScriptValue InputDialog::setItemsEditable(bool itemsEditable)
	{
		mInputDialog->setComboBoxEditable(itemsEditable);
		
		return thisObject();
	}
	
	QScriptValue InputDialog::show()
	{
		setup();
		
		mInputDialog->open();

		return thisObject();
	}

	int InputDialog::showModal()
	{
		setup();
		
		return mInputDialog->exec();
	}
	
	void InputDialog::finished(int result)
	{
		if(mOnClosed.isValid())
			mOnClosed.call(thisObject(), QScriptValueList() << result);
	}
	
	void InputDialog::doubleValueChanged(double value)
	{
		if(mOnValueChanged.isValid())
			mOnValueChanged.call(thisObject(), QScriptValueList() << value);
	}

	void InputDialog::intValueChanged(int value)
	{
		if(mOnValueChanged.isValid())
			mOnValueChanged.call(thisObject(), QScriptValueList() << value);
	}

	void InputDialog::textValueChanged(const QString &value)
	{
		if(mOnValueChanged.isValid())
			mOnValueChanged.call(thisObject(), QScriptValueList() << value);
	}
	
	void InputDialog::setup()
	{
        mInputDialog->setOption(QInputDialog::UsePlainTextEditForTextInput, mInputType == MultilineText);

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
        case MultilineText:
		case Text:
		default:
			mInputDialog->setInputMode(QInputDialog::TextInput);
			mInputDialog->setTextValue(mValue.toString());
			break;
		}
	}
}
