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

#include "inifile.hpp"

#include <QScriptValueIterator>

namespace Code
{
	QScriptValue IniFile::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		auto iniFile = new IniFile;

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == QLatin1String("encoding"))
				iniFile->mEncoding = static_cast<Encoding>(it.value().toInt32());
		}

		return CodeClass::constructor(iniFile, context, engine);
	}
	
	IniFile::IniFile()
		: CodeClass()
		
    {
    }

	QScriptValue IniFile::load(const QString &filename)
	{
        mINI::INIFile file(toEncoding(filename, mEncoding).constData());

        if(!file.read(mStructure))
        {
            throwError(QStringLiteral("LoadFileError"), tr("Cannot load the file"));
            return thisObject();
        }

        mLatestFilename = filename;
	
		return thisObject();
	}
	
	QScriptValue IniFile::save(const QString &filename)
	{
        QByteArray filenameByteArray = toEncoding(filename.isEmpty() ? mLatestFilename : filename, mEncoding);
        mINI::INIFile file(filenameByteArray.constData());

        if(!file.write(mStructure))
        {
            throwError(QStringLiteral("SaveFileError"), tr("Cannot save the file"));
            return thisObject();
        }

        return thisObject();
	}
	
	QScriptValue IniFile::clear()
	{
        mStructure.clear();

		return thisObject();
	}

	QScriptValue IniFile::setSection(const QString &sectionName, bool create)
	{
        QByteArray sectionNameByteArray = toEncoding(sectionName, mEncoding);

        if(!mStructure.has(sectionNameByteArray.constData()))
        {
            if(!create)
            {
                throwError(QStringLiteral("FindSectionError"), tr("Cannot find the section named \"%1\"").arg(sectionName));
                return thisObject();
            }

            mStructure[sectionNameByteArray.constData()];
        }

        mCurrentSectionName = sectionNameByteArray;

		return thisObject();
	}
	
	QScriptValue IniFile::setEncoding(Encoding encoding)
	{
		mEncoding = encoding;
		
		return thisObject();
	}
	
    QString IniFile::sectionAt(int sectionIndex) const
	{
        if(sectionIndex < 0 || sectionIndex >= static_cast<int>(mStructure.size()))
		{
			throwError(QStringLiteral("FindSectionError"), tr("Invalid section index"));
			return QString();
		}

        auto it = mStructure.begin();

        std::advance(it, sectionIndex);

        return QString::fromStdString(it->first);
	}
	
	QScriptValue IniFile::deleteSection(const QString &sectionName)
	{
        QByteArray sectionNameByteArray = toEncoding(sectionName, mEncoding);

        if(!mStructure.remove(sectionNameByteArray.constData()))
		{
			throwError(QStringLiteral("FindSectionError"), tr("Cannot delete section named \"%1\"").arg(sectionName));
			return thisObject();
		}
		
		return thisObject();
	}
	
	int IniFile::sectionCount() const
	{
        return static_cast<int>(mStructure.size());
	}
	
	bool IniFile::keyExists(const QString &keyName) const
    {
        if(!mStructure.has(mCurrentSectionName.constData()))
            return false;

        QByteArray keyNameByteArray = toEncoding(keyName, mEncoding);

        return mStructure.get(mCurrentSectionName.constData()).has(keyNameByteArray.constData());
	}
	
	QString IniFile::keyAt(int keyIndex) const
	{
        if(!mStructure.has(mCurrentSectionName.constData()))
            return {};

        const auto section = mStructure.get(mCurrentSectionName.constData());

        if(keyIndex < 0 || keyIndex >= static_cast<int>(section.size()))
		{
			throwError(QStringLiteral("KeyError"), tr("Invalid key index"));
            return {};
		}

        auto it = section.begin();

        std::advance(it, keyIndex);

        return QString::fromStdString(it->first);
	}
	
	QString IniFile::keyValue(const QString &keyName) const
	{
        if(!keyExists(keyName))
        {
			throwError(QStringLiteral("KeyError"), tr("Cannot find any key named \"%1\"").arg(keyName));
            return {};
        }

        const auto section = mStructure.get(mCurrentSectionName.constData());

        return QString::fromStdString(section.get(toEncoding(keyName, mEncoding).constData()));
	}
	
	QScriptValue IniFile::setKeyValue(const QString &keyName, const QString &value)
	{
        auto &section = mStructure[mCurrentSectionName.constData()];

        section[toEncoding(keyName, mEncoding).constData()] = toEncoding(value, mEncoding).constData();

		return thisObject();
	}
	
	QScriptValue IniFile::deleteKey(const QString &keyName)
	{
        if(!mStructure.has(mCurrentSectionName.constData()))
        {
            throwError(QStringLiteral("KeyError"), tr("Cannot delete key named \"%1\"").arg(keyName));
            return thisObject();
        }

        auto &section = mStructure[mCurrentSectionName.constData()];
        QByteArray keyNameByteArray = toEncoding(keyName, mEncoding);

        if(!section.remove(keyNameByteArray.constData()))
        {
			throwError(QStringLiteral("KeyError"), tr("Cannot delete key named \"%1\"").arg(keyName));
            return thisObject();
        }

        return thisObject();
	}
	
    int IniFile::keyCount() const
	{
        if(!mStructure.has(mCurrentSectionName.constData()))
            return 0;

        return static_cast<int>(mStructure.get(mCurrentSectionName.constData()).size());
	}
}
