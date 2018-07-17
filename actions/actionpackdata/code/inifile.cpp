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

#include "inifile.h"

#include <QScriptValueIterator>

#include <boost/property_tree/ini_parser.hpp>

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
        try
        {
            boost::property_tree::ini_parser::read_ini(toEncoding(filename, mEncoding).constData(), mTree);
        }
        catch(const std::runtime_error &)
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

        try
        {
            boost::property_tree::ini_parser::write_ini(filenameByteArray.constData(), mTree);
        }
        catch(const std::runtime_error &)
        {
			throwError(QStringLiteral("SaveFileError"), tr("Cannot save the file"));
            return thisObject();
        }

        return thisObject();
	}
	
	QScriptValue IniFile::clear()
	{
        mTree.clear();

		return thisObject();
	}

	QScriptValue IniFile::setSection(const QString &sectionName, bool create)
	{
        QByteArray sectionNameByteArray = toEncoding(sectionName, mEncoding);

        if(mTree.count(sectionNameByteArray.constData()) == 0)
        {
            if(!create)
            {
				throwError(QStringLiteral("FindSectionError"), tr("Cannot find the section named \"%1\"").arg(sectionName));
                return thisObject();
            }

            mCurrentSection = mTree.add_child(sectionNameByteArray.constData(), {});
        }
        else
            mCurrentSection = mTree.get_child(sectionNameByteArray.constData());

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
        if(sectionIndex < 0 || sectionIndex >= static_cast<int>(mTree.size()))
		{
			throwError(QStringLiteral("FindSectionError"), tr("Invalid section index"));
			return QString();
		}

        auto it = mTree.begin();

        std::advance(it, sectionIndex);

        return QString::fromStdString(it->first);
	}
	
	QScriptValue IniFile::deleteSection(const QString &sectionName)
	{
        QByteArray sectionNameByteArray = toEncoding(sectionName, mEncoding);

        if(!mTree.erase(sectionNameByteArray.constData()))
		{
			throwError(QStringLiteral("FindSectionError"), tr("Cannot delete section named \"%1\"").arg(sectionName));
			return thisObject();
		}
		
		return thisObject();
	}
	
	int IniFile::sectionCount() const
	{
        return static_cast<int>(mTree.size());
	}
	
	bool IniFile::keyExists(const QString &keyName) const
    {
        QByteArray keyNameByteArray = toEncoding(keyName, mEncoding);

        return (mCurrentSection.count(keyNameByteArray.constData()) > 0);
	}
	
	QString IniFile::keyAt(int keyIndex) const
	{
        if(keyIndex < 0 || keyIndex >= static_cast<int>(mCurrentSection.size()))
		{
			throwError(QStringLiteral("KeyError"), tr("Invalid key index"));
			return QString();
		}

        auto it = mCurrentSection.begin();

        std::advance(it, keyIndex);

        return QString::fromStdString(it->first);
	}
	
	QString IniFile::keyValue(const QString &keyName) const
	{
        if(!keyExists(keyName))
        {
			throwError(QStringLiteral("KeyError"), tr("Cannot find any key named \"%1\"").arg(keyName));
            return QString();
        }

        return QString::fromStdString(mCurrentSection.get<std::string>(toEncoding(keyName, mEncoding).constData()));
	}
	
	QScriptValue IniFile::setKeyValue(const QString &keyName, const QString &value)
	{
        mCurrentSection.put(toEncoding(keyName, mEncoding).constData(), toEncoding(value, mEncoding).constData());

        mTree.put_child(mCurrentSectionName.constData(), mCurrentSection);
		
		return thisObject();
	}
	
	QScriptValue IniFile::deleteKey(const QString &keyName)
	{
        QByteArray keyNameByteArray = toEncoding(keyName, mEncoding);

        if(!mCurrentSection.erase(keyNameByteArray.constData()))
        {
			throwError(QStringLiteral("KeyError"), tr("Cannot delete key named \"%1\"").arg(keyName));
            return thisObject();
        }

        mTree.put_child(mCurrentSectionName.constData(), mCurrentSection);

        return thisObject();
	}
	
    int IniFile::keyCount() const
	{
        return static_cast<int>(mCurrentSection.size());
	}
}
