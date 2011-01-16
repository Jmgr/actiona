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

#include "colordialog.h"
#include "code/color.h"

#include <QScriptValueIterator>
#include <QColorDialog>

namespace Code
{
	QScriptValue ColorDialog::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		ColorDialog *colorDialog = new ColorDialog;
		colorDialog->setupConstructorParameters(context->argument(0));

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == "showAlphaChannel")
				colorDialog->mColorDialog->setOption(QColorDialog::ShowAlphaChannel, it.value().toBool());
			else if(it.name() == "color")
				colorDialog->setColorPrivate(it.value(), context);
			else if(it.name() == "onClosed")
				colorDialog->mOnClosed = it.value();
			else if(it.name() == "onColorSelected")
				colorDialog->mOnColorSelected = it.value();
			else if(it.name() == "onColorChanged")
				colorDialog->mOnColorChanged = it.value();
		}

		return colorDialog->mThisObject = CodeClass::constructor(colorDialog, context, engine);
	}

	ColorDialog::ColorDialog()
		: BaseWindow(),
		mColorDialog(new QColorDialog)
	{
		setWidget(mColorDialog);
		
		connect(mColorDialog, SIGNAL(finished(int)), this, SLOT(finished(int)));
		connect(mColorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(colorSelected(QColor)));
		connect(mColorDialog, SIGNAL(currentColorChanged(QColor)), this, SLOT(currentColorChanged(QColor)));
	}
	
	ColorDialog::~ColorDialog()
	{
		delete mColorDialog;
	}
	
	QScriptValue ColorDialog::showAlphaChannel(bool showAlphaChannel)
	{
		mColorDialog->setOption(QColorDialog::ShowAlphaChannel, showAlphaChannel);
		
		return context()->thisObject();
	}

	QScriptValue ColorDialog::setColor(const QScriptValue &color)
	{
		setColorPrivate(color, context());
		
		return context()->thisObject();
	}
	
	QScriptValue ColorDialog::show()
	{
		mColorDialog->open();

		return context()->thisObject();
	}

	int ColorDialog::showModal()
	{
		return mColorDialog->exec();
	}
	
	QScriptValue ColorDialog::color() const
	{
		return Color::constructor(mColorDialog->currentColor(), context(), engine());
	}
	
	void ColorDialog::finished(int result)
	{
		if(mOnClosed.isValid())
			mOnClosed.call(mThisObject, QScriptValueList() << result);
	}
	
	void ColorDialog::colorSelected(const QColor &color)
	{
		if(mOnColorSelected.isValid())
			mOnColorSelected.call(mThisObject, QScriptValueList() << Color::constructor(color, context(), engine()));
	}

	void ColorDialog::currentColorChanged(const QColor &color)
	{
		if(mOnColorChanged.isValid())
			mOnColorChanged.call(mThisObject, QScriptValueList() << Color::constructor(color, context(), engine()));
	}
	
	void ColorDialog::setColorPrivate(const QScriptValue &color, QScriptContext *context)
	{
		if(context->argumentCount() == 1)
		{
			QObject *object = color.toQObject();
			if(Color *codeColor = qobject_cast<Color*>(object))
				mColorDialog->setCurrentColor(codeColor->color());
			else
				mColorDialog->setCurrentColor(QColor(color.toString()));
		}
		else if(context->argumentCount() == 3)
			mColorDialog->setCurrentColor(QColor(context->argument(0).toInt32(), context->argument(1).toInt32(), context->argument(2).toInt32()));
	}
}
