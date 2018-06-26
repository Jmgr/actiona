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
#include "writeregistryinstance.h"
#include "readregistryinstance.h"
#include "listparameterdefinition.h"
#include "textparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class WriteRegistryDefinition : public ActionTools::ActionDefinition
	{
		Q_OBJECT

	public:
		explicit WriteRegistryDefinition(ActionTools::ActionPack *pack)
			: ActionDefinition(pack)
		{
			auto &key = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("key"), tr("Key")});
            key.setTooltip(tr("The registry key to write to"));
            key.setItems(ReadRegistryInstance::keys);
            key.setDefaultValue(ReadRegistryInstance::keys.second.at(ActionTools::Registry::CurrentUser));

			auto &subKey = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("subKey"), tr("Subkey")});
            subKey.setTooltip(tr("The registry subkey to write to"));

			auto &value = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("value"), tr("Value")});
            value.setTooltip(tr("The value to write to"));

			auto &data = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("data"), tr("Data")});
            data.setTooltip(tr("The data to write"));

			addException(WriteRegistryInstance::CannotFindSubKeyException, tr("Cannot find subKey"));
			addException(WriteRegistryInstance::CannotWriteValueException, tr("Cannot write value"));
		}

		QString name() const override													{ return QObject::tr("Write registry"); }
		QString id() const override														{ return QStringLiteral("ActionWriteRegistry"); }
		ActionTools::Flag flags() const override											{ return ActionTools::WorksOnWindows | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Writes an entry to the registry"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new WriteRegistryInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Data; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/writeregistry.png")); }

	private:
		Q_DISABLE_COPY(WriteRegistryDefinition)
	};
}

