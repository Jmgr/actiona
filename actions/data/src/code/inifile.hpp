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

class INIStructure;

namespace Code
{
    class IniFile : public CodeClass
	{
		Q_OBJECT
	
	public:
        Q_INVOKABLE IniFile();
        Q_INVOKABLE explicit IniFile(const QJSValue &parameters);
        ~IniFile();
	
        Q_INVOKABLE QString toString() const override                                { return QStringLiteral("IniFile"); }
        Q_INVOKABLE IniFile *load(const QString &filename);
        Q_INVOKABLE IniFile *save(const QString &filename = QString());
        Q_INVOKABLE IniFile *clear();
        Q_INVOKABLE IniFile *setSection(const QString &sectionName, bool create = true);
        Q_INVOKABLE IniFile *setEncoding(Encoding encoding);
        Q_INVOKABLE QString sectionAt(int sectionIndex) const;
        Q_INVOKABLE IniFile *deleteSection(const QString &sectionName);
        Q_INVOKABLE int sectionCount() const;
        Q_INVOKABLE bool keyExists(const QString &keyName) const;
        Q_INVOKABLE QString keyAt(int keyIndex) const;
        Q_INVOKABLE QString keyValue(const QString &keyName) const;
        Q_INVOKABLE IniFile *setKeyValue(const QString &keyName, const QString &value);
        Q_INVOKABLE IniFile *deleteKey(const QString &keyName);
        Q_INVOKABLE int keyCount() const;

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
	
	private:
        Encoding mEncoding{Native};
        std::unique_ptr<INIStructure> mStructure;
        QByteArray mCurrentSectionName;
        QString mLatestFilename;
	};
}

