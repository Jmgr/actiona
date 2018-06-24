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
			auto key = new ActionTools::ListParameterDefinition(ActionTools::Name(QStringLiteral("key"), tr("Key")), this);
			key->setTooltip(tr("The registry key to read from"));
			key->setItems(ReadRegistryInstance::keys);
			key->setDefaultValue(ReadRegistryInstance::keys.second.at(ActionTools::Registry::CurrentUser));
			addElement(key);

			auto subKey = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("subKey"), tr("Subkey")), this);
			subKey->setTooltip(tr("The registry subkey to read from"));
			addElement(subKey);

			auto value = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("value"), tr("Value")), this);
			value->setTooltip(tr("The value to read"));
			addElement(value);

			auto variable = new ActionTools::VariableParameterDefinition(ActionTools::Name(QStringLiteral("variable"), tr("Variable")), this);
			variable->setTooltip(tr("The variable where to save the value read from the registry"));
			addElement(variable);

			addException(ReadRegistryInstance::CannotFindSubKeyException, tr("Cannot find subKey"));
			addException(ReadRegistryInstance::CannotFindValueException, tr("Cannot find value"));
			addException(ReadRegistryInstance::InvalidValueType, tr("Invalid value type"));
		}

		QString name() const override													{ return QObject::tr("Read registry"); }
		QString id() const override														{ return QStringLiteral("ActionReadRegistry"); }
		ActionTools::Flag flags() const override											{ return ActionTools::WorksOnWindows | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Read an entry from the registry"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new ReadRegistryInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Data; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/readregistry.png")); }

	private:
		Q_DISABLE_COPY(ReadRegistryDefinition)
	};
}

