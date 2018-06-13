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

#pragma once

#include "actiontools_global.h"
#include "code/codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QRect>
#include <QPoint>
#include <QSize>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Rect : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(int top READ top WRITE setTop)
		Q_PROPERTY(int bottom READ bottom WRITE setBottom)
		Q_PROPERTY(int left READ left WRITE setLeft)
		Q_PROPERTY(int right READ right WRITE setRight)
		Q_PROPERTY(int x READ x WRITE setX)
		Q_PROPERTY(int y READ y WRITE setY)
		Q_PROPERTY(int width READ width WRITE setWidth)
		Q_PROPERTY(int height READ height WRITE setHeight)
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue constructor(const QRect &rect, QScriptEngine *engine);
		static QRect parameter(QScriptContext *context, QScriptEngine *engine);

		static void registerClass(QScriptEngine *scriptEngine);
		
		Rect();
		Rect(const Rect &other);
		Rect(const QRect &rect);
		
		Rect &operator=(Rect other);
		Rect &operator=(QRect rect);
		
		void swap(Rect &other);
		void swap(QRect &rect);
		
		const QRect &rect() const;

		int width() const;
		int height() const;
		int x() const;
		int y() const;
		int left() const;
		int right() const;
		int top() const;
		int bottom() const;
		
	public slots:
		QScriptValue clone() const;
		bool equals(const QScriptValue &other) const override;
		QString toString() const override;
		QScriptValue normalize();
		QScriptValue setTop(int top);
		QScriptValue setBottom(int bottom);
		QScriptValue setLeft(int left);
		QScriptValue setRight(int right);
		QScriptValue setX(int x);
		QScriptValue setY(int y);
		QScriptValue setWidth(int width);
		QScriptValue setHeight(int height);
		QScriptValue setSize();
		QScriptValue setCoords(int x1, int y1, int x2, int y2);
		QScriptValue setRect();
		QScriptValue translate();
		bool contains(const QScriptValue &point) const;
		QScriptValue united() const;
		QScriptValue intersected() const;
		bool intersects() const;
		bool isEmpty() const;
		QScriptValue center() const;
		QScriptValue size() const;
		
	private:
		QRect mRect;
	};
}

