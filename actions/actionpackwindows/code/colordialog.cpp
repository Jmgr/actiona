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

#include "colordialog.h"
#include "code/color.h"

#include <QScriptValueIterator>
#include <QColorDialog>

namespace Code
{
	QScriptValue ColorDialog::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		auto colorDialog = new ColorDialog;
		colorDialog->setupConstructorParameters(context, engine, context->argument(0));

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == QLatin1String("showAlphaChannel"))
				colorDialog->mColorDialog->setOption(QColorDialog::ShowAlphaChannel, it.value().toBool());
			else if(it.name() == QLatin1String("color"))
				colorDialog->setColorPrivate(it.value(), context);
			else if(it.name() == QLatin1String("onClosed"))
				colorDialog->mOnClosed = it.value();
			else if(it.name() == QLatin1String("onColorSelected"))
				colorDialog->mOnColorSelected = it.value();
			else if(it.name() == QLatin1String("onColorChanged"))
				colorDialog->mOnColorChanged = it.value();
		}

		return CodeClass::constructor(colorDialog, context, engine);
	}

	ColorDialog::ColorDialog()
		: BaseWindow(),
		mColorDialog(new QColorDialog)
	{
        mColorDialog->setWindowFlags(mColorDialog->windowFlags() | Qt::WindowContextHelpButtonHint);

		setWidget(mColorDialog);
		
        connect(mColorDialog, &QColorDialog::finished, this, &ColorDialog::finished);
        connect(mColorDialog, &QColorDialog::colorSelected, this, &ColorDialog::colorSelected);
        connect(mColorDialog, &QColorDialog::currentColorChanged, this, &ColorDialog::currentColorChanged);
	}
	
	ColorDialog::~ColorDialog()
	{
		delete mColorDialog;
	}

	QScriptValue ColorDialog::color() const
	{
		return Color::constructor(mColorDialog->currentColor(), engine());
	}
	
	QScriptValue ColorDialog::showAlphaChannel(bool showAlphaChannel)
	{
		mColorDialog->setOption(QColorDialog::ShowAlphaChannel, showAlphaChannel);
		
		return thisObject();
	}

	QScriptValue ColorDialog::setColor(const QScriptValue &color)
	{
		setColorPrivate(color, context());
		
		return thisObject();
	}
	
	QScriptValue ColorDialog::show()
	{
		mColorDialog->open();

		return thisObject();
	}

	int ColorDialog::showModal()
	{
		return mColorDialog->exec();
	}
	
	void ColorDialog::finished(int result)
	{
		if(mOnClosed.isValid())
			mOnClosed.call(thisObject(), QScriptValueList() << result);
	}
	
	void ColorDialog::colorSelected(const QColor &color)
	{
		if(mOnColorSelected.isValid())
			mOnColorSelected.call(thisObject(), QScriptValueList() << Color::constructor(color, engine()));
	}

	void ColorDialog::currentColorChanged(const QColor &color)
	{
		if(mOnColorChanged.isValid())
			mOnColorChanged.call(thisObject(), QScriptValueList() << Color::constructor(color, engine()));
	}
	
	void ColorDialog::setColorPrivate(const QScriptValue &color, QScriptContext *context)
	{
		if(context->argumentCount() == 1)
		{
			QObject *object = color.toQObject();
			if(auto codeColor = qobject_cast<Color*>(object))
				mColorDialog->setCurrentColor(codeColor->color());
			else
				mColorDialog->setCurrentColor(QColor(color.toString()));
		}
		else if(context->argumentCount() == 3)
			mColorDialog->setCurrentColor(QColor(context->argument(0).toInt32(), context->argument(1).toInt32(), context->argument(2).toInt32()));
	}
}
