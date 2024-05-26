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

#include "actiontools/actiontools_global.hpp"
#include "codeclass.hpp"

#include <QJSValue>
#include <QPoint>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Point : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(int x READ x WRITE setX)
		Q_PROPERTY(int y READ y WRITE setY)
		
    public:
        Q_INVOKABLE Point();
        Q_INVOKABLE Point(int x, int y);
        Q_INVOKABLE Point(const Point &other);
		Point(const QPoint &point);
		
		Point &operator=(Point other);
		Point &operator=(QPoint point);
		
		void swap(Point &other);
		void swap(QPoint &point);
		
		const QPoint &point() const;

		int x() const;
		int y() const;
		
        Q_INVOKABLE QJSValue clone() const;
        Q_INVOKABLE bool equals(const QJSValue &other) const;
        Q_INVOKABLE QString toString() const override;
        Q_INVOKABLE Point *setX(int x);
        Q_INVOKABLE Point *setY(int y);

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

	private:
		QPoint mPoint;
	};
}
