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
#include "actiontools/code/codeclass.hpp"

#include <QJSValue>
#include <QRect>

namespace Code
{
    class Point;
    class Size;

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
        Q_INVOKABLE Rect();
        Q_INVOKABLE Rect(int left, int top, int width, int height);
        Q_INVOKABLE Rect(const Rect &other);
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

        Q_INVOKABLE QJSValue clone() const;
        Q_INVOKABLE bool equals(const QJSValue &other) const;
        Q_INVOKABLE QString toString() const override;
        Q_INVOKABLE Rect *normalize();
        Q_INVOKABLE Rect *setTop(int top);
        Q_INVOKABLE Rect *setBottom(int bottom);
        Q_INVOKABLE Rect *setLeft(int left);
        Q_INVOKABLE Rect *setRight(int right);
        Q_INVOKABLE Rect *setX(int x);
        Q_INVOKABLE Rect *setY(int y);
        Q_INVOKABLE Rect *setWidth(int width);
        Q_INVOKABLE Rect *setHeight(int height);
        Q_INVOKABLE Rect *setSize(const Size *size);
        Q_INVOKABLE Rect *setCoords(int x1, int y1, int x2, int y2);
        Q_INVOKABLE Rect *setRect(int x, int y, int width, int height);
        Q_INVOKABLE Rect *translate(const Point *point);
        Q_INVOKABLE bool contains(const Point *point) const;
        Q_INVOKABLE bool contains(const Rect *rect) const;
        Q_INVOKABLE bool contains(int x, int y) const;
        Q_INVOKABLE bool contains(int left, int top, int width, int height) const;
        Q_INVOKABLE QJSValue united(const Rect *rect) const;
        Q_INVOKABLE QJSValue intersected(const Rect *rect) const;
        Q_INVOKABLE bool intersects(const Rect *rect) const;
        Q_INVOKABLE bool isEmpty() const;
        Q_INVOKABLE QJSValue center() const;
        Q_INVOKABLE QJSValue size() const;

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
		
	private:
		QRect mRect;
	};
}

