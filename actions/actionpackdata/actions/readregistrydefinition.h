/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#ifndef READREGISTRYDEFINITION_H
#define READREGISTRYDEFINITION_H

#include "actiondefinition.h"
#include "readregistryinstance.h"
#include "listparameterdefinition.h"
#include "textparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ReadRegistryDefinition : public QObject, public ActionTools::ActionDefinition
	{
		Q_OBJECT

	public:
		explicit ReadRegistryDefinition(ActionTools::ActionPack *pack)
			: ActionDefinition(pack)
		{
			ActionTools::ListParameterDefinition *key = new ActionTools::ListParameterDefinition(ActionTools::Name("key", tr("Key")), this);
			key->setTooltip(tr("The registry key to read from"));
			key->setItems(ReadRegistryInstance::keys);
			key->setDefaultValue(ReadRegistryInstance::keys.second.at(ActionTools::Registry::CurrentUser));
			addElement(key);

			ActionTools::TextParameterDefinition *subKey = new ActionTools::TextParameterDefinition(ActionTools::Name("subKey", tr("Subkey")), this);
			subKey->setTooltip(tr("The registry subkey to read from"));
			addElement(subKey);

			ActionTools::TextParameterDefinition *value = new ActionTools::TextParameterDefinition(ActionTools::Name("value", tr("Value")), this);
			value->setTooltip(tr("The value to read"));
			addElement(value);

			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition(ActionTools::Name("variable", tr("Variable")), this);
			variable->setTooltip(tr("The variable where to save the value read from the registry"));
			addElement(variable);

			addException(ReadRegistryInstance::CannotFindSubKeyException, tr("Cannot find subKey"));
			addException(ReadRegistryInstance::CannotFindValueException, tr("Cannot find value"));
			addException(ReadRegistryInstance::InvalidValueType, tr("Invalid value type"));
		}

		QString name() const													{ return QObject::tr("Read registry"); }
		QString id() const														{ return "ActionReadRegistry"; }
		ActionTools::Flag flags() const											{ return ActionTools::WorksOnWindows | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Read an entry from the registry"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new ReadRegistryInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Data; }
		QPixmap icon() const													{ return QPixmap(":/icons/readregistry.png"); }

	private:
		Q_DISABLE_COPY(ReadRegistryDefinition)
	};
}

#endif // READREGISTRYDEFINITION_H
