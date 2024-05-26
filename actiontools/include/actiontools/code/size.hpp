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
#include <QSize>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Size : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(int width READ width WRITE setWidth)
		Q_PROPERTY(int height READ height WRITE setHeight)
		
	public:
        Q_INVOKABLE Size();
        Q_INVOKABLE Size(int width, int height);
        Q_INVOKABLE Size(const Size &other);
		Size(const QSize &size);
		
		Size &operator=(Size other);
		Size &operator=(QSize size);
		
		void swap(Size &other);
		void swap(QSize &size);
		
		const QSize &size() const;

		int width() const;
		int height() const;
		
        Q_INVOKABLE QJSValue clone() const;
        Q_INVOKABLE bool equals(const QJSValue &other) const;
        Q_INVOKABLE QString toString() const override;
        Q_INVOKABLE Size *setWidth(int width);
        Q_INVOKABLE Size *setHeight(int height);

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

	private:
		QSize mSize;
	};
}

