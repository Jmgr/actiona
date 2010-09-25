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

#include "algorithms.h"

#include <QCryptographicHash>
#include <cstdlib>

namespace Code
{
	QScriptValue Algorithms::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
	
		return engine->newQObject(new Algorithms, QScriptEngine::ScriptOwnership);
	}
	
	QString Algorithms::md4(const QString &data) const
	{
		return QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Md4).toHex();
	}
	
	QString Algorithms::md5(const QString &data) const
	{
		return QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Md5).toHex();
	}
	
	QString Algorithms::sha1(const QString &data) const
	{
		return QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Sha1).toHex();
	}
	
	void Algorithms::setRandomSeed(uint seed) const
	{
		qsrand(seed);
	}
	
	int Algorithms::randomMax() const
	{
		return RAND_MAX;
	}
	
	int Algorithms::randomInteger() const
	{
		return qrand();
	}
	
	int Algorithms::randomInteger(int min, int max) const
	{
		return static_cast<int>(min + (static_cast<float>(qrand()) / RAND_MAX * (max - min + 1)));
	}
	
	float Algorithms::randomFloat(float min, float max) const
	{
		return (qrand() / static_cast<float>(RAND_MAX)) * (max - min) + min;
	}
}
