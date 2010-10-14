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

#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include "basewindow.h"

class QColorDialog;

namespace Code
{
	class ColorDialog : public BaseWindow
	{
		Q_OBJECT
		Q_CLASSINFO("type", "CodeClass")
		Q_PROPERTY(QScriptValue onClosed READ onClosed WRITE setOnClosed)
		Q_PROPERTY(QScriptValue onColorSelected READ onColorSelected WRITE setOnColorSelected)
		Q_PROPERTY(QScriptValue onColorChanged READ onColorChanged WRITE setOnColorChanged)
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		
		ColorDialog();
		~ColorDialog();
		
		void setOnClosed(const QScriptValue &onClosed)					{ mOnClosed = onClosed; }
		void setOnColorSelected(const QScriptValue &onColorSelected)	{ mOnColorSelected = onColorSelected; }
		void setOnColorChanged(const QScriptValue &onColorChanged)		{ mOnColorChanged = onColorChanged; }
		
		QScriptValue onClosed() const									{ return mOnClosed; }
		QScriptValue onColorSelected() const							{ return mOnColorSelected; }
		QScriptValue onColorChanged() const								{ return mOnColorChanged; }
		
	public slots:
		QScriptValue showAlphaChannel(bool showAlphaChannel);
		QScriptValue setColor(const QScriptValue &color);
		QScriptValue show();
		int showModal();
		QScriptValue color() const;
		
	private slots:
		QString toString() const					{ return "ColorDialog"; }
		void finished(int result);
		void colorSelected(const QColor &color);
		void currentColorChanged(const QColor &color);
		
	private:
		void setColorPrivate(const QScriptValue &color, QScriptContext *context);
		
		QColorDialog *mColorDialog;
		QScriptValue mOnClosed;
		QScriptValue mOnColorSelected;
		QScriptValue mOnColorChanged;
		QScriptValue mThisObject;
	};
}

#endif // COLORDIALOG_H
