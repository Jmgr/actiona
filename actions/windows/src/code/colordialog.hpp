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

#pragma once

#include "basewindow.hpp"
#include "actiontools/code/color.hpp"

class QColorDialog;

namespace Code
{
	class ColorDialog : public BaseWindow
	{
		Q_OBJECT
		Q_PROPERTY(QJSValue onClosed READ onClosed WRITE setOnClosed)
		Q_PROPERTY(QJSValue onColorSelected READ onColorSelected WRITE setOnColorSelected)
		Q_PROPERTY(QJSValue onColorChanged READ onColorChanged WRITE setOnColorChanged)
        Q_PROPERTY(const Color *color READ color WRITE setColor)
		
	public:
        Q_INVOKABLE ColorDialog();
        Q_INVOKABLE ColorDialog(const QJSValue &parameters);
		~ColorDialog() override;
		
		void setOnClosed(const QJSValue &onClosed)					{ mOnClosed = onClosed; }
		void setOnColorSelected(const QJSValue &onColorSelected)	{ mOnColorSelected = onColorSelected; }
		void setOnColorChanged(const QJSValue &onColorChanged)		{ mOnColorChanged = onColorChanged; }
		
		QJSValue onClosed() const									{ return mOnClosed; }
		QJSValue onColorSelected() const							{ return mOnColorSelected; }
		QJSValue onColorChanged() const								{ return mOnColorChanged; }

        const Color *color() const;
		
        Q_INVOKABLE ColorDialog *showAlphaChannel(bool showAlphaChannel);
        Q_INVOKABLE ColorDialog *setColor(const Color *color);
        Q_INVOKABLE ColorDialog *show();
        Q_INVOKABLE int showModal();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
		
	private slots:
		QString toString() const override					{ return QStringLiteral("ColorDialog"); }
		void finished(int result);
		void colorSelected(const QColor &color);
		void currentColorChanged(const QColor &color);
		
	private:
		QColorDialog *mColorDialog;
		QJSValue mOnClosed;
		QJSValue mOnColorSelected;
		QJSValue mOnColorChanged;
	};
}

