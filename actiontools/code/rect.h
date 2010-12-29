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

#ifndef RECT_H
#define RECT_H

#include "actiontools_global.h"

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>
#include <QRect>
#include <QPoint>
#include <QSize>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Rect : public QObject, public QScriptable
	{
		Q_OBJECT
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue constructor(const QRect &rect, QScriptContext *context, QScriptEngine *engine);
		static QRect parameter(QScriptContext *context);

		static void registerClass(QScriptEngine *scriptEngine);
		
		Rect();
		Rect(const Rect &other);
		Rect(const QRect &rect);
		
		Rect &operator=(Rect other);
		Rect &operator=(QRect rect);
		
		void swap(Rect &other);
		void swap(QRect &rect);
		
		const QRect &rect() const;
		
	public slots:
		QScriptValue clone() const;
		bool equals(const QScriptValue &other) const;
		QString toString() const;
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
		QScriptValue moveTop(int y);
		QScriptValue moveBottom(int y);
		QScriptValue moveLeft(int x);
		QScriptValue moveRight(int x);
		QScriptValue moveCenter();
		QScriptValue moveTo();
		bool contains(const QScriptValue &point) const;
		QScriptValue united() const;
		QScriptValue intersected() const;
		bool intersects() const;
		bool isEmpty() const;
		QScriptValue center() const;
		QScriptValue size() const;
		int width() const;
		int height() const;
		int x() const;
		int y() const;
		int left() const;
		int right() const;
		int top() const;
		int bottom() const;
		
	private:
		QRect mRect;
	};
}

#endif // RECT_H
