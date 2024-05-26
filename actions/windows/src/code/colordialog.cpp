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

#include "colordialog.hpp"

#include <QJSValueIterator>
#include <QColorDialog>

namespace Code
{
	ColorDialog::ColorDialog()
		: BaseWindow(),
		mColorDialog(new QColorDialog)
	{
        mColorDialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);

		setWidget(mColorDialog);
		
        connect(mColorDialog, &QColorDialog::finished, this, &ColorDialog::finished);
        connect(mColorDialog, &QColorDialog::colorSelected, this, &ColorDialog::colorSelected);
        connect(mColorDialog, &QColorDialog::currentColorChanged, this, &ColorDialog::currentColorChanged);
	}

    ColorDialog::ColorDialog(const QJSValue &parameters)
        : ColorDialog()
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

            if(it.name() == QLatin1String("showAlphaChannel"))
                mColorDialog->setOption(QColorDialog::ShowAlphaChannel, it.value().toBool());
            else if(it.name() == QLatin1String("color"))
            {
                if(auto color = qobject_cast<Color*>(it.value().toQObject()))
                    mColorDialog->setCurrentColor(color->color());
                else
                {
                    throwError(QStringLiteral("Parameter"), QStringLiteral("parameter has to be a Color"));
                    return;
                }
            }
            else if(it.name() == QLatin1String("onClosed"))
                mOnClosed = it.value();
            else if(it.name() == QLatin1String("onColorSelected"))
                mOnColorSelected = it.value();
            else if(it.name() == QLatin1String("onColorChanged"))
                mOnColorChanged = it.value();
        }
    }
	
	ColorDialog::~ColorDialog()
	{
		delete mColorDialog;
	}

    const Color *ColorDialog::color() const
    {
        auto color = CodeClass::construct<Color>(mColorDialog->currentColor());

        return qobject_cast<Color *>(color.toQObject());
	}
	
    ColorDialog *ColorDialog::showAlphaChannel(bool showAlphaChannel)
	{
		mColorDialog->setOption(QColorDialog::ShowAlphaChannel, showAlphaChannel);
		
        return this;
	}

    ColorDialog *ColorDialog::setColor(const Color *color)
	{
        mColorDialog->setCurrentColor(color->color());
		
        return this;
	}
	
    ColorDialog *ColorDialog::show()
	{
		mColorDialog->open();

        return this;
	}

	int ColorDialog::showModal()
	{
		return mColorDialog->exec();
	}

    void ColorDialog::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<ColorDialog*>("const ColorDialog *");

        CodeClass::registerClass<ColorDialog>(QStringLiteral("ColorDialog"), scriptEngine);
    }
	
	void ColorDialog::finished(int result)
	{
        if(!mOnClosed.isUndefined())
            mOnClosed.call(QJSValueList() << result);
	}
	
	void ColorDialog::colorSelected(const QColor &color)
	{
        if(!mOnColorSelected.isUndefined())
            mOnColorSelected.call(QJSValueList() << CodeClass::construct<Color>(color));
	}

	void ColorDialog::currentColorChanged(const QColor &color)
	{
        if(!mOnColorChanged.isUndefined())
            mOnColorChanged.call(QJSValueList() << CodeClass::construct<Color>(color));
	}
}
