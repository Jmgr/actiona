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
#include "code.h"

#include <QCryptographicHash>
#include <QScriptValueIterator>
#include <cstdlib>

namespace Code
{
	QScriptValue Algorithms::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
	
		return engine->newQObject(new Algorithms, QScriptEngine::ScriptOwnership);
	}

	QScriptValue Algorithms::md4(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

		return QString(QCryptographicHash::hash(context->argument(0).toString().toUtf8(), QCryptographicHash::Md4).toHex());
	}

	QScriptValue Algorithms::md5(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

		return QString(QCryptographicHash::hash(context->argument(0).toString().toUtf8(), QCryptographicHash::Md5).toHex());
	}

	QScriptValue Algorithms::sha1(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

		return QString(QCryptographicHash::hash(context->argument(0).toString().toUtf8(), QCryptographicHash::Sha1).toHex());
	}

	QScriptValue Algorithms::setRandomSeed(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

		qsrand(context->argument(0).toInt32());

		return QScriptValue();
	}

	QScriptValue Algorithms::randomMax(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
		Q_UNUSED(engine)

		return RAND_MAX;
	}

	QScriptValue Algorithms::randomInteger(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

		switch(context->argumentCount())
		{
		case 0:
			return qrand();
		case 2:
			{
				int min = context->argument(0).toInt32();
				int max = context->argument(1).toInt32();

				return randomInteger(min, max);
			}
		default:
			return QScriptValue();
		}
	}

	QScriptValue Algorithms::randomFloat(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

		float min = context->argument(0).toNumber();
		float max = context->argument(1).toNumber();

		return (qrand() / static_cast<float>(RAND_MAX)) * (max - min) + min;
	}

	QScriptValue Algorithms::randomString(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

		QScriptValueIterator it(context->argument(0));
		QString characters("abcdefghijklmnopqrstuvwxyz0123456789");
		int minLength = 5;
		int maxLength = 15;

		while(it.hasNext())
		{
			it.next();

			if(it.name() == "characters")
				characters = it.value().toString();
			else if(it.name() == "minLength")
				minLength = it.value().toInt32();
			else if(it.name() == "maxLength")
				maxLength = it.value().toInt32();
		}

		QString back;
		int charactersLength = characters.length();
		int finalLength = randomInteger(minLength, maxLength);
		for(int i = 0; i < finalLength; ++i)
		{
			back += characters.at(randomInteger(0, charactersLength - 1));
		}

		return back;
	}

	void Algorithms::registerClass(QScriptEngine *scriptEngine)
	{
		Code::addClassToScriptEngine<Algorithms>(scriptEngine);
		Code::addClassGlobalFunctionToScriptEngine<Algorithms>(&md4, "md4", scriptEngine);
		Code::addClassGlobalFunctionToScriptEngine<Algorithms>(&md5, "md5", scriptEngine);
		Code::addClassGlobalFunctionToScriptEngine<Algorithms>(&sha1, "sha1", scriptEngine);
		Code::addClassGlobalFunctionToScriptEngine<Algorithms>(&setRandomSeed, "setRandomSeed", scriptEngine);
		Code::addClassGlobalFunctionToScriptEngine<Algorithms>(&randomMax, "randomMax", scriptEngine);
		Code::addClassGlobalFunctionToScriptEngine<Algorithms>(&randomInteger, "randomInteger", scriptEngine);
		Code::addClassGlobalFunctionToScriptEngine<Algorithms>(&randomFloat, "randomFloat", scriptEngine);
		Code::addClassGlobalFunctionToScriptEngine<Algorithms>(&randomString, "randomString", scriptEngine);
	}

	int Algorithms::randomInteger(int min, int max)
	{
		return static_cast<int>(min + (static_cast<float>(qrand()) / RAND_MAX * (max - min + 1)));
	}
}
