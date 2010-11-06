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

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "actiontools_global.h"

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Algorithms : public QObject, public QScriptable
	{
		Q_OBJECT
		Q_CLASSINFO("type", "CodeClass")
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		
	public slots:
		QString md4(const QString &data) const;
		QString md5(const QString &data) const;
		QString sha1(const QString &data) const;
		void setRandomSeed(uint seed) const;
		int randomMax() const;
		int randomInteger() const;
		int randomInteger(int min, int max) const;
		float randomFloat(float min, float max) const;
		QString randomString() const;
	};
}

#endif // ALGORITHMS_H
