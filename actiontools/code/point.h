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

#ifndef POINT_H
#define POINT_H

#include "actiontools_global.h"

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>
#include <QPoint>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Point : public QObject, public QScriptable
	{
		Q_OBJECT
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue constructor(const QPoint &point, QScriptContext *context, QScriptEngine *engine);
		static QPoint parameter(QScriptContext *context);
		
		Point();
		Point(const Point &other);
		Point(const QPoint &point);
		
		Point &operator=(Point other);
		Point &operator=(QPoint point);
		
		void swap(Point &other);
		void swap(QPoint &point);
		
		const QPoint &point() const;
		
	public slots:
		QScriptValue clone() const;
		bool equals(const QScriptValue &other) const;
		QScriptValue setX(int x);
		QScriptValue setY(int y);
		int x() const;
		int y() const;
		
	private:
		QPoint mPoint;
	};
}

#endif // POINT_H
