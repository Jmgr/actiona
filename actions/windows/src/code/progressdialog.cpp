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

#include "progressdialog.hpp"

#include <QJSValueIterator>
#include <QProgressDialog>

namespace Code
{
	ProgressDialog::ProgressDialog()
		: BaseWindow(),
		mProgressDialog(new QProgressDialog)
	{
        mProgressDialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);

		setWidget(mProgressDialog);

        connect(mProgressDialog, &QProgressDialog::canceled, this, &ProgressDialog::canceled);
    }

    ProgressDialog::ProgressDialog(const QJSValue &parameters)
        : ProgressDialog()
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

            if(it.name() == QLatin1String("value"))
                mProgressDialog->setValue(it.value().toInt());
            else if(it.name() == QLatin1String("labelText"))
                mProgressDialog->setLabelText(it.value().toString());
            else if(it.name() == QLatin1String("minimum"))
                mProgressDialog->setMinimum(it.value().toInt());
            else if(it.name() == QLatin1String("maximum"))
                mProgressDialog->setMaximum(it.value().toInt());
            else if(it.name() == QLatin1String("range"))
            {
                mProgressDialog->setMinimum(it.value().property(QStringLiteral("minimum")).toInt());
                mProgressDialog->setMaximum(it.value().property(QStringLiteral("maximum")).toInt());
            }
            else if(it.name() == QLatin1String("onCanceled"))
                mOnCanceled = it.value();
        }
    }

	ProgressDialog::~ProgressDialog()
	{
		delete mProgressDialog;
	}

	int ProgressDialog::value() const
	{
		return mProgressDialog->value();
	}
	
    ProgressDialog *ProgressDialog::setValue(int value)
	{
		mProgressDialog->setValue(value);

        return this;
	}
	
    ProgressDialog *ProgressDialog::setLabelText(const QString &labelText)
	{
		mProgressDialog->setLabelText(labelText);
		
        return this;
	}
	
    ProgressDialog *ProgressDialog::setMinimum(int minimum)
	{
		mProgressDialog->setMinimum(minimum);
		
        return this;
	}

    ProgressDialog *ProgressDialog::setMaximum(int maximum)
	{
		mProgressDialog->setMaximum(maximum);
		
        return this;
	}

    ProgressDialog *ProgressDialog::setRange(int minimum, int maximum)
	{
		mProgressDialog->setMinimum(minimum);
		mProgressDialog->setMaximum(maximum);
		
        return this;
	}
	
    ProgressDialog *ProgressDialog::show()
	{
		mProgressDialog->open();

        return this;
    }

	int ProgressDialog::showModal()
	{
		return mProgressDialog->exec();
	}

    void ProgressDialog::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<ProgressDialog*>("const ProgressDialog *");

        CodeClass::registerClass<ProgressDialog>(QStringLiteral("ProgressDialog"), scriptEngine);
    }
	
	void ProgressDialog::canceled()
	{
        if(mProgressDialog->isVisible() && !mOnCanceled.isUndefined())
            mOnCanceled.call();
	}
}
