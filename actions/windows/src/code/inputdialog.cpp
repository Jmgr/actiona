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

#include "inputdialog.hpp"

#include <QJSValueIterator>
#include <QInputDialog>

namespace Code
{
	InputDialog::InputDialog()
		: BaseWindow(),
		
		mInputDialog(new QInputDialog)
	{
        mInputDialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);

		setWidget(mInputDialog);
		
        connect(mInputDialog, &QInputDialog::finished, this, &InputDialog::finished);
        connect(mInputDialog, &QInputDialog::doubleValueChanged, this, &InputDialog::doubleValueChanged);
        connect(mInputDialog, &QInputDialog::intValueChanged, this, &InputDialog::intValueChanged);
        connect(mInputDialog, &QInputDialog::textValueChanged, this, &InputDialog::textValueChanged);
	}

    InputDialog::InputDialog(const QJSValue &parameters)
        : InputDialog()
    {
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return;
        }

        setupConstructorParameters(parameters);

        QJSValueIterator it(parameters);

        while(it.hasNext())
        {
            it.next();

            if(it.name() == QLatin1String("labelText"))
                mInputDialog->setLabelText(it.value().toString());
            else if(it.name() == QLatin1String("okButtonText"))
                mInputDialog->setOkButtonText(it.value().toString());
            else if(it.name() == QLatin1String("cancelButtonText"))
                mInputDialog->setCancelButtonText(it.value().toString());
            else if(it.name() == QLatin1String("textEchoMode"))
                mInputDialog->setTextEchoMode(static_cast<QLineEdit::EchoMode>(it.value().toInt()));
            else if(it.name() == QLatin1String("floatDecimals"))
                mInputDialog->setDoubleDecimals(it.value().toInt());
            else if(it.name() == QLatin1String("integerStep"))
                mInputDialog->setIntStep(it.value().toInt());
            else if(it.name() == QLatin1String("minimum"))
                mMinimum = it.value();
            else if(it.name() == QLatin1String("maximum"))
                mMaximum = it.value();
            else if(it.name() == QLatin1String("range"))
            {
                mMinimum = it.value().property(QStringLiteral("minimum"));
                mMaximum = it.value().property(QStringLiteral("maximum"));
            }
            else if(it.name() == QLatin1String("inputType"))
                mInputType = static_cast<InputType>(it.value().toInt());
            else if(it.name() == QLatin1String("value"))
                mValue = it.value();
            else if(it.name() == QLatin1String("items"))
                mItems = it.value();
            else if(it.name() == QLatin1String("itemsEditable"))
                mInputDialog->setComboBoxEditable(it.value().toBool());
            else if(it.name() == QLatin1String("onClosed"))
                mOnClosed = it.value();
            else if(it.name() == QLatin1String("onValueChanged"))
                mOnValueChanged = it.value();
        }
    }
	
	InputDialog::~InputDialog()
	{
		delete mInputDialog;
	}

	QJSValue InputDialog::value() const
	{
		switch(mInputType)
		{
		case Integer:
            return mInputDialog->intValue();
		case Float:
            return mInputDialog->doubleValue();
		case Items:
		case Text:
        case MultilineText:
		default:
            return mInputDialog->textValue();
			break;
		}
	}
	
    InputDialog *InputDialog::setLabelText(const QString &labelText)
	{
		mInputDialog->setLabelText(labelText);

        return this;
	}
	
    InputDialog *InputDialog::setOkButtonText(const QString &okButtonText)
	{
		mInputDialog->setOkButtonText(okButtonText);
		
        return this;
	}

    InputDialog *InputDialog::setCancelButtonText(const QString &cancelButtonText)
	{
		mInputDialog->setCancelButtonText(cancelButtonText);
		
        return this;
	}

    InputDialog *InputDialog::setTextEchoMode(TextEchoMode textEchoMode)
	{
		mInputDialog->setTextEchoMode(static_cast<QLineEdit::EchoMode>(textEchoMode));
		
        return this;
	}

    InputDialog *InputDialog::setFloatDecimals(int decimals)
	{
		mInputDialog->setDoubleDecimals(decimals);
		
        return this;
	}

    InputDialog *InputDialog::setIntegerStep(int step)
	{
		mInputDialog->setIntStep(step);
		
        return this;
	}

    InputDialog *InputDialog::setMaximum(const QJSValue &maximum)
	{
		mMaximum = maximum;
		
        return this;
	}

    InputDialog *InputDialog::setMinimum(const QJSValue &minimum)
	{
		mMinimum = minimum;
		
        return this;
	}

    InputDialog *InputDialog::setRange(const QJSValue &minimum, const QJSValue &maximum)
	{
		mMinimum = minimum;
		mMaximum = maximum;
		
        return this;
	}
	
    InputDialog *InputDialog::setInputType(InputType inputType)
	{
		mInputType = inputType;
		
        return this;
	}
	
    InputDialog *InputDialog::setValue(const QJSValue &value)
	{
		mValue = value;
		
        return this;
	}
	
    InputDialog *InputDialog::setItems(const QJSValue &items)
	{
		mItems = items;
		
        return this;
	}
	
    InputDialog *InputDialog::setItemsEditable(bool itemsEditable)
	{
		mInputDialog->setComboBoxEditable(itemsEditable);
		
        return this;
	}
	
    InputDialog *InputDialog::show()
	{
		setup();
		
		mInputDialog->open();

        return this;
	}

	int InputDialog::showModal()
	{
		setup();
		
		return mInputDialog->exec();
	}

    void InputDialog::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<InputDialog*>("const InputDialog *");

        CodeClass::registerClass<InputDialog>(QStringLiteral("InputDialog"), scriptEngine);
    }
	
	void InputDialog::finished(int result)
	{
        if(!mOnClosed.isUndefined())
            mOnClosed.call(QJSValueList() << result);
	}
	
	void InputDialog::doubleValueChanged(double value)
	{
        if(!mOnValueChanged.isUndefined())
            mOnValueChanged.call(QJSValueList() << value);
	}

	void InputDialog::intValueChanged(int value)
	{
        if(!mOnValueChanged.isUndefined())
            mOnValueChanged.call(QJSValueList() << value);
	}

	void InputDialog::textValueChanged(const QString &value)
	{
        if(!mOnValueChanged.isUndefined())
            mOnValueChanged.call(QJSValueList() << value);
	}
	
	void InputDialog::setup()
	{
        mInputDialog->setOption(QInputDialog::UsePlainTextEditForTextInput, mInputType == MultilineText);

		switch(mInputType)
		{
		case Integer:
			mInputDialog->setInputMode(QInputDialog::IntInput);
            mInputDialog->setIntValue(mValue.toInt());
            if(!mMinimum.isUndefined())
                mInputDialog->setIntMinimum(mMinimum.toInt());
            if(!mMaximum.isUndefined())
                mInputDialog->setIntMaximum(mMaximum.toInt());
			break;
		case Float:
			mInputDialog->setInputMode(QInputDialog::DoubleInput);
			mInputDialog->setDoubleValue(mValue.toNumber());
            if(!mMinimum.isUndefined())
				mInputDialog->setDoubleMinimum(mMinimum.toNumber());
            if(!mMaximum.isUndefined())
				mInputDialog->setDoubleMaximum(mMaximum.toNumber());
			break;
		case Items:
			{
				QJSValueIterator it(mItems);
				QStringList values;
				while(it.hasNext())
				{
					it.next();
					
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
