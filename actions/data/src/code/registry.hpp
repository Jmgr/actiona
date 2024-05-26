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

#include "actiontools/code/codeclass.hpp"

#include <QJSValue>
#include <QStringList>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

namespace Code
{
	class Registry : public CodeClass
	{
		Q_OBJECT
	
	public:
		enum Key
		{
			ClassesRoot,
			CurrentConfig,
			CurrentUser,
			Users,
			LocalMachine
		};
        Q_ENUM(Key)
	
        Q_INVOKABLE Registry();
		~Registry() override;
	
        Q_INVOKABLE QString toString() const override                                { return QStringLiteral("Registry"); }
        Q_INVOKABLE Registry *openKey(Key key, const QString &subKey);
        Q_INVOKABLE Registry *createKey(Key key, const QString &subKey);
        Q_INVOKABLE Registry *setValue(const QString &value, const QVariant &data);
        Q_INVOKABLE QVariant value(const QString &value = QString()) const;
        Q_INVOKABLE QStringList valueNames() const;
        Q_INVOKABLE QStringList keys() const;
        Q_INVOKABLE Registry *deleteValue(const QString &value = QString());
        Q_INVOKABLE Registry *deleteKey(Key key, const QString &subKey);
        Q_INVOKABLE Registry *deleteKey();
        Q_INVOKABLE Registry *closeKey();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

	private:
	#ifdef Q_OS_WIN
		HKEY enumToKey(Key key) const;
	
		HKEY mHKey;
		Key mRootKey;
		QString mSubKey;
	#endif
	};
}

