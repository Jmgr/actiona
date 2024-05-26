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
#include "mini/ini.h"

#include <QJSValueIterator>

struct INIStructure
{
    mINI::INIStructure structure;
};

namespace Code
{
	IniFile::IniFile()
        : CodeClass(),
        mStructure(std::make_unique<INIStructure>())
    {
    }

    IniFile::IniFile(const QJSValue &parameters)
        : IniFile()
    {
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return;
        }

        QJSValueIterator it(parameters);

        while(it.hasNext())
        {
            it.next();

            if(it.name() == QLatin1String("encoding"))
                mEncoding = static_cast<Encoding>(it.value().toInt());
        }
    }

    IniFile::~IniFile()
    {
    }

    IniFile *IniFile::load(const QString &filename)
	{
        mINI::INIFile file(toEncoding(filename, mEncoding).constData());

        if(!file.read(mStructure->structure))
        {
            throwError(QStringLiteral("LoadFileError"), tr("Cannot load the file"));
            return this;
        }

        mLatestFilename = filename;
	
        return this;
	}
	
    IniFile *IniFile::save(const QString &filename)
	{
        QByteArray filenameByteArray = toEncoding(filename.isEmpty() ? mLatestFilename : filename, mEncoding);
        mINI::INIFile file(filenameByteArray.constData());

        if(!file.write(mStructure->structure))
        {
            throwError(QStringLiteral("SaveFileError"), tr("Cannot save the file"));
            return this;
        }

        return this;
	}
	
    IniFile *IniFile::clear()
	{
        mStructure->structure.clear();

        return this;
	}

    IniFile *IniFile::setSection(const QString &sectionName, bool create)
	{
        QByteArray sectionNameByteArray = toEncoding(sectionName, mEncoding);

        if(!mStructure->structure.has(sectionNameByteArray.constData()))
        {
            if(!create)
            {
                throwError(QStringLiteral("FindSectionError"), tr("Cannot find the section named \"%1\"").arg(sectionName));
                return this;
            }

            mStructure->structure[sectionNameByteArray.constData()];
        }

        mCurrentSectionName = sectionNameByteArray;

        return this;
	}
	
    IniFile *IniFile::setEncoding(Encoding encoding)
	{
		mEncoding = encoding;
		
        return this;
	}
	
    QString IniFile::sectionAt(int sectionIndex) const
	{
        if(sectionIndex < 0 || sectionIndex >= static_cast<int>(mStructure->structure.size()))
		{
			throwError(QStringLiteral("FindSectionError"), tr("Invalid section index"));
			return QString();
		}

        auto it = mStructure->structure.begin();

        std::advance(it, sectionIndex);

        return QString::fromStdString(it->first);
	}
	
    IniFile *IniFile::deleteSection(const QString &sectionName)
	{
        QByteArray sectionNameByteArray = toEncoding(sectionName, mEncoding);

        if(!mStructure->structure.remove(sectionNameByteArray.constData()))
		{
			throwError(QStringLiteral("FindSectionError"), tr("Cannot delete section named \"%1\"").arg(sectionName));
            return this;
		}
		
        return this;
	}
	
	int IniFile::sectionCount() const
	{
        return static_cast<int>(mStructure->structure.size());
	}
	
	bool IniFile::keyExists(const QString &keyName) const
    {
        if(!mStructure->structure.has(mCurrentSectionName.constData()))
            return false;

        QByteArray keyNameByteArray = toEncoding(keyName, mEncoding);

        return mStructure->structure.get(mCurrentSectionName.constData()).has(keyNameByteArray.constData());
	}
	
	QString IniFile::keyAt(int keyIndex) const
	{
        if(!mStructure->structure.has(mCurrentSectionName.constData()))
            return {};

        const auto section = mStructure->structure.get(mCurrentSectionName.constData());

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

        const auto section = mStructure->structure.get(mCurrentSectionName.constData());

        return QString::fromStdString(section.get(toEncoding(keyName, mEncoding).constData()));
	}
	
    IniFile *IniFile::setKeyValue(const QString &keyName, const QString &value)
	{
        auto &section = mStructure->structure[mCurrentSectionName.constData()];

        section[toEncoding(keyName, mEncoding).constData()] = toEncoding(value, mEncoding).constData();

        return this;
	}
	
    IniFile *IniFile::deleteKey(const QString &keyName)
	{
        if(!mStructure->structure.has(mCurrentSectionName.constData()))
        {
            throwError(QStringLiteral("KeyError"), tr("Cannot delete key named \"%1\"").arg(keyName));
            return this;
        }

        auto &section = mStructure->structure[mCurrentSectionName.constData()];
        QByteArray keyNameByteArray = toEncoding(keyName, mEncoding);

        if(!section.remove(keyNameByteArray.constData()))
        {
			throwError(QStringLiteral("KeyError"), tr("Cannot delete key named \"%1\"").arg(keyName));
            return this;
        }

        return this;
	}
	
    int IniFile::keyCount() const
	{
        if(!mStructure->structure.has(mCurrentSectionName.constData()))
            return 0;

        return static_cast<int>(mStructure->structure.get(mCurrentSectionName.constData()).size());
	}

    void IniFile::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<IniFile*>("const IniFile *");

        CodeClass::registerClass<IniFile>(QStringLiteral("IniFile"), scriptEngine);
    }
}
