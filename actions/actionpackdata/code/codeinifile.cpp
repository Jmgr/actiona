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

#include "codeinifile.h"

QScriptValue CodeIniFile::constructor(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(context)
	
	return engine->newQObject(new CodeIniFile, QScriptEngine::ScriptOwnership);
}

QScriptValue CodeIniFile::load(const QString &filename)
{
	if(!mConfig.load(filename.toLocal8Bit()))
	{
		context()->throwError(tr("Cannot load the file"));
		return context()->thisObject();
	}
		
	return context()->thisObject();
}

QScriptValue CodeIniFile::save(const QString &filename)
{
	bool saveResult;
	
	if(filename.isEmpty())
		saveResult = mConfig.save();
	else
		saveResult = mConfig.save(filename.toLocal8Bit());
	
	if(!saveResult)
	{
		context()->throwError(tr("Cannot save the file"));
		return context()->thisObject();
	}
	
	return context()->thisObject();
}

QScriptValue CodeIniFile::clear()
{
	mConfig.clear();
	
	return context()->thisObject();
}

QScriptValue CodeIniFile::preserveDeletedData(bool preserve)
{
	mConfig.preserveDeletedData(preserve);
	
	return context()->thisObject();
}

QScriptValue CodeIniFile::setDelimiter(char delimiter)
{
	mConfig.setDefaultDelimiter(delimiter);
	
	return context()->thisObject();
}

QScriptValue CodeIniFile::setCommentCharacter(char commentchar)
{
	mConfig.setDefaultCommentCharacter(commentchar);
	
	return context()->thisObject();
}

QScriptValue CodeIniFile::setSection(const QString &sectionName, bool create)
{
	if(!mConfig.setSection(sectionName.toLocal8Bit(), create))
	{
		context()->throwError(tr("Cannot find the section named \"%1\"").arg(sectionName));
		return context()->thisObject();
	}
	
	return context()->thisObject();
}

QString CodeIniFile::sectionAt(int sectionIndex) const
{
	if(sectionIndex < 0 || sectionIndex >= mConfig.getNumSections())
	{
		context()->throwError(tr("Invalid section index"));
		return QString();
	}
	
	return mConfig.getSectionNameAt(sectionIndex);
}

QScriptValue CodeIniFile::deleteSection(const QString &sectionName)
{
	if(!mConfig.deleteSection(sectionName.toLocal8Bit()))
	{
		context()->throwError(tr("Cannot delete section named \"%1\"").arg(sectionName));
		return context()->thisObject();
	}
	
	return context()->thisObject();
}

int CodeIniFile::sectionCount() const
{
	return mConfig.getNumSections();
}

bool CodeIniFile::keyExists(const QString &keyName) const
{
	return mConfig.exists(keyName.toLocal8Bit());
}

QString CodeIniFile::keyAt(int keyIndex) const
{
	if(keyIndex < 0 || keyIndex >= mConfig.getNumDataMembers())
	{
		context()->throwError(tr("Invalid key index"));
		return QString();
	}
	
	return mConfig.getDataNameAt(keyIndex);
}

QString CodeIniFile::keyValue(const QString &keyName) const
{
	return mConfig.getStringValue(keyName.toLocal8Bit());
}

QScriptValue CodeIniFile::setKeyValue(const QString &keyName, const QString &value)
{
	mConfig.setStringValue(keyName.toLocal8Bit(), value.toLocal8Bit());
	
	return context()->thisObject();
}

QScriptValue CodeIniFile::deleteKey(const QString &keyName)
{
	if(!mConfig.deleteData(keyName.toLocal8Bit()))
	{
		context()->throwError(tr("Cannot delete key named \"%1\"").arg(keyName));
		return context()->thisObject();
	}
	
	return context()->thisObject();
}

int CodeIniFile::keyCount() const
{
	return mConfig.getNumDataMembers();
}
