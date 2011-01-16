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

#include "inifile.h"

#include <QScriptValueIterator>

namespace Code
{
	QScriptValue IniFile::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		IniFile *iniFile = new IniFile;

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == "delimiter")
				iniFile->mConfig.setDelimiter(it.value().toInt32());
			else if(it.name() == "commentCharacter")
				iniFile->mConfig.setCommentCharacter(it.value().toInt32());
			else if(it.name() == "encoding")
				iniFile->mEncoding = static_cast<Encoding>(it.value().toInt32());
		}

		return CodeClass::constructor(iniFile, context, engine);
	}
	
	IniFile::IniFile()
		: CodeClass(),
		mEncoding(Native)
	{
	}
	
	QScriptValue IniFile::load(const QString &filename)
	{
		if(!mConfig.load(toEncoding(filename, mEncoding)))
		{
			throwError("LoadFileError", tr("Cannot load the file"));
			return context()->thisObject();
		}
	
		return context()->thisObject();
	}
	
	QScriptValue IniFile::save(const QString &filename)
	{
		bool saveResult;
		
		if(filename.isEmpty())
			saveResult = mConfig.save();
		else
			saveResult = mConfig.save(toEncoding(filename, mEncoding));
		
		if(!saveResult)
		{
			throwError("SaveFileError", tr("Cannot save the file"));
			return context()->thisObject();
		}
		
		return context()->thisObject();
	}
	
	QScriptValue IniFile::clear()
	{
		mConfig.clear();
		
		return context()->thisObject();
	}
	
	QScriptValue IniFile::preserveDeletedData(bool preserve)
	{
		mConfig.preserveDeletedData(preserve);
		
		return context()->thisObject();
	}
	
	QScriptValue IniFile::setDelimiter(char delimiter)
	{
		mConfig.setDefaultDelimiter(delimiter);
		
		return context()->thisObject();
	}
	
	QScriptValue IniFile::setCommentCharacter(char commentchar)
	{
		mConfig.setDefaultCommentCharacter(commentchar);
		
		return context()->thisObject();
	}
	
	QScriptValue IniFile::setSection(const QString &sectionName, bool create)
	{
		if(!mConfig.setSection(toEncoding(sectionName, mEncoding), create))
		{
			throwError("FindSectionError", tr("Cannot find the section named \"%1\"").arg(sectionName));
			return context()->thisObject();
		}
		
		return context()->thisObject();
	}
	
	QScriptValue IniFile::setEncoding(Encoding encoding)
	{
		mEncoding = encoding;
		
		return context()->thisObject();
	}
	
	QString IniFile::sectionAt(int sectionIndex) const
	{
		if(sectionIndex < 0 || sectionIndex >= mConfig.getNumSections())
		{
			throwError("FindSectionError", tr("Invalid section index"));
			return QString();
		}
		
		return mConfig.getSectionNameAt(sectionIndex);
	}
	
	QScriptValue IniFile::deleteSection(const QString &sectionName)
	{
		if(!mConfig.deleteSection(toEncoding(sectionName, mEncoding)))
		{
			throwError("FindSectionError", tr("Cannot delete section named \"%1\"").arg(sectionName));
			return context()->thisObject();
		}
		
		return context()->thisObject();
	}
	
	int IniFile::sectionCount() const
	{
		return mConfig.getNumSections();
	}
	
	bool IniFile::keyExists(const QString &keyName) const
	{
		return mConfig.exists(toEncoding(keyName, mEncoding));
	}
	
	QString IniFile::keyAt(int keyIndex) const
	{
		if(keyIndex < 0 || keyIndex >= mConfig.getNumDataMembers())
		{
			throwError("FindSectionError", tr("Invalid key index"));
			return QString();
		}
		
		return mConfig.getDataNameAt(keyIndex);
	}
	
	QString IniFile::keyValue(const QString &keyName) const
	{
		return mConfig.getStringValue(toEncoding(keyName, mEncoding));
	}
	
	QScriptValue IniFile::setKeyValue(const QString &keyName, const QString &value)
	{
		mConfig.setStringValue(toEncoding(keyName, mEncoding), toEncoding(value, mEncoding));
		
		return context()->thisObject();
	}
	
	QScriptValue IniFile::deleteKey(const QString &keyName)
	{
		if(!mConfig.deleteData(toEncoding(keyName, mEncoding)))
		{
			throwError("FindSectionError", tr("Cannot delete key named \"%1\"").arg(keyName));
			return context()->thisObject();
		}
		
		return context()->thisObject();
	}
	
	int IniFile::keyCount() const
	{
		return mConfig.getNumDataMembers();
	}
}
