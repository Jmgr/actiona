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

#include "code/codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>

#include <boost/property_tree/ptree.hpp>

namespace Code
{
	class IniFile : public CodeClass
	{
		Q_OBJECT
	
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		
		IniFile();
	
	public slots:
		QString toString() const override                                { return QStringLiteral("IniFile"); }
        bool equals(const QScriptValue &other) const override    { return defaultEqualsImplementation<IniFile>(other); }
		QScriptValue load(const QString &filename);
		QScriptValue save(const QString &filename = QString());
		QScriptValue clear();
		QScriptValue setSection(const QString &sectionName, bool create = true);
		QScriptValue setEncoding(Encoding encoding);
        QString sectionAt(int sectionIndex) const;
		QScriptValue deleteSection(const QString &sectionName);
		int sectionCount() const;
		bool keyExists(const QString &keyName) const;
		QString keyAt(int keyIndex) const;
		QString keyValue(const QString &keyName) const;
		QScriptValue setKeyValue(const QString &keyName, const QString &value);
		QScriptValue deleteKey(const QString &keyName);
        int keyCount() const;
	
	private:
        Encoding mEncoding{Native};
        boost::property_tree::ptree mCurrentSection;
        boost::property_tree::ptree mTree;
        QByteArray mCurrentSectionName;
        QString mLatestFilename;
	};
}

