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
#include <QSize>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Size : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(int width READ width WRITE setWidth)
		Q_PROPERTY(int height READ height WRITE setHeight)
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue constructor(const QSize &size, QScriptEngine *engine);
		static QSize parameter(QScriptContext *context, QScriptEngine *engine);

		static void registerClass(QScriptEngine *scriptEngine);
		
		Size();
		Size(const Size &other);
		Size(const QSize &size);
		
		Size &operator=(Size other);
		Size &operator=(QSize size);
		
		void swap(Size &other);
		void swap(QSize &size);
		
		const QSize &size() const;

		int width() const;
		int height() const;
		
	public slots:
		QScriptValue clone() const;
		bool equals(const QScriptValue &other) const override;
		QString toString() const override;
		QScriptValue setWidth(int width);
		QScriptValue setHeight(int height);
		
	private:
		QSize mSize;
	};
}

