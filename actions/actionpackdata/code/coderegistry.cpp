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

#include "coderegistry.h"

QScriptValue CodeRegistry::constructor(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(context)
	
	return engine->newQObject(new CodeRegistry, QScriptEngine::ScriptOwnership);
}

QScriptValue CodeRegistry::openKey(Key key, const QString &subKey) const
{
#ifdef Q_WS_WIN
#else
	return context()->thisObject();
#endif	
}

QScriptValue CodeRegistry::createKey(Key key, const QString &subKey) const
{
#ifdef Q_WS_WIN
#else
	return context()->thisObject();
#endif
}

QScriptValue CodeRegistry::setValue(const QString &valueName, const QVariant &value) const
{
#ifdef Q_WS_WIN
#else
	return context()->thisObject();
#endif
}

QVariant CodeRegistry::value(const QString &valueName) const
{
#ifdef Q_WS_WIN
#else
	return QVariant();
#endif
}

QStringList CodeRegistry::valueNames() const	
{
#ifdef Q_WS_WIN
#else
	return QStringList();
#endif
}

QStringList CodeRegistry::keys() const
{
#ifdef Q_WS_WIN
#else
	return QStringList();
#endif
}

QScriptValue CodeRegistry::deleteKey(bool recursive) const
{
#ifdef Q_WS_WIN
#else
	return context()->thisObject();
#endif
}

QScriptValue CodeRegistry::closeKey() const
{
#ifdef Q_WS_WIN
#else
	return context()->thisObject();
#endif
}
