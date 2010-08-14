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

#include "actiondefinition.h"
#include "actioninstance.h"
#include "actionexception.h"
#include "elementdefinition.h"

#include <QScriptEngine>
#include <QScriptValue>

namespace ActionTools
{
	ActionDefinition::~ActionDefinition()
	{
		qDeleteAll(mElements);
		qDeleteAll(mExceptions);
	}
	
	ActionInstance *ActionDefinition::scriptInit(QScriptEngine *scriptEngine) const
	{
		ActionTools::ActionInstance *actionInstance = newActionInstance();
		QScriptValue scriptObject = scriptEngine->newQObject(actionInstance, QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
		
		scriptEngine->globalObject().setProperty(id(), scriptObject);
		
		for(int enumeratorIndex = 0; enumeratorIndex < actionInstance->metaObject()->enumeratorCount(); ++enumeratorIndex)
		{
			const QMetaEnum &metaEnum = actionInstance->metaObject()->enumerator(enumeratorIndex);
			QScriptValue enumObject = scriptEngine->newObject();
			
			scriptObject.setProperty(metaEnum.name(), enumObject);
			
			for(int keyIndex = 0; keyIndex < metaEnum.keyCount(); ++keyIndex)
				enumObject.setProperty(metaEnum.key(keyIndex), metaEnum.value(keyIndex));
		}
		
		return actionInstance;
	}
	
	QString ActionDefinition::CategoryName[CategoryCount] =
	{
		QObject::tr("Windows"),
		QObject::tr("Device"),
		QObject::tr("System"),
		QObject::tr("Internal"),
		QObject::tr("Other")
	};

	QStringList ActionDefinition::StandardTabs = QStringList() << QObject::tr("Standard") << QObject::tr("Advanced");
	
	void ActionDefinition::addElement(ElementDefinition *element, int tab)
	{
		if(tab > 0 && tabs().count() > 0)
		{
			if(tab < tabs().count())
				element->setTab(tab);
			else
				qWarning("Trying to add an element with an incorrect tab number");
		}

		mElements.append(element);
	}
}
