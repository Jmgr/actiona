/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "actiontools_global.h"
#include "codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Algorithms : public CodeClass
	{
		Q_OBJECT
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

		static QScriptValue md4(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue md5(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue sha1(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue setRandomSeed(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue randomMax(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue randomInteger(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue randomFloat(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue randomString(QScriptContext *context, QScriptEngine *engine);

		static void registerClass(QScriptEngine *scriptEngine);

    public slots:
        virtual QString toString() const { return "Algorithms"; }
        virtual bool equals(const QScriptValue &other) const { Q_UNUSED(other) return true; }

	private:
		static int randomInteger(int min, int max);
	};
}

#endif // ALGORITHMS_H
