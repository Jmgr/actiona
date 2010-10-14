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

#ifndef ACTIONPACK_H
#define ACTIONPACK_H

#include <QList>
#include <QScriptValue>
#include <QScriptEngine>
#include "version.h"

namespace ActionTools
{
	class ActionDefinition;

	typedef QList<ActionDefinition *> ActionDefinitionList;

	class ActionPack
	{
	public:
		ActionPack()												{}
		virtual ~ActionPack()										{}

		virtual QString id() const = 0;
		virtual QString name() const = 0;
		virtual Tools::Version version() const = 0;
		virtual void codeInit(QScriptEngine *scriptEngine) const	{ Q_UNUSED(scriptEngine) }
		
		//Returns an instance of each plugin definition
		const ActionDefinitionList &actionsDefinitions() const		{ return mActionDefinitions; }

		void setFilename(const QString &filename)					{ mFilename = filename; }
		const QString &filename() const								{ return mFilename; }

	protected:
		void addActionDefinition(ActionDefinition *actionDefinition)
		{
			mActionDefinitions.append(actionDefinition);
		}
		
		template<typename T>
		void addCodeClass(const QString &objectName, QScriptEngine *scriptEngine) const
		{
			QScriptValue metaObject = scriptEngine->newQMetaObject(&T::staticMetaObject, scriptEngine->newFunction(&T::constructor));
			metaObject.setData("CodeClass");
			scriptEngine->globalObject().setProperty(objectName, metaObject);
		}
		
		void addCodeFunction(const QString &objectName, QScriptEngine::FunctionSignature function, int length, QScriptEngine *scriptEngine) const
		{
			scriptEngine->globalObject().setProperty(objectName, scriptEngine->newFunction(function, length));
		}
		
		void addCodeFunction(const QString &objectName, QScriptEngine::FunctionSignature function, QScriptEngine *scriptEngine) const
		{
			addCodeFunction(objectName, function, 0, scriptEngine);
		}

	private:
		ActionDefinitionList mActionDefinitions;
		QString mFilename;

		Q_DISABLE_COPY(ActionPack)
	};
}

Q_DECLARE_INTERFACE(ActionTools::ActionPack,
					"eu.actionaz.ActionPack/1.0")

#endif // ACTIONPACK_H
