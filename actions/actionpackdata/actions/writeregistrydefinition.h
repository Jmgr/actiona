/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#ifndef WRITEREGISTRYDEFINITION_H
#define WRITEREGISTRYDEFINITION_H

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
	class WriteRegistryDefinition : public QObject, public ActionTools::ActionDefinition
	{
		Q_OBJECT

	public:
		explicit WriteRegistryDefinition(ActionTools::ActionPack *pack)
			: ActionDefinition(pack)
		{
	#ifdef Q_WS_WIN
			ActionTools::ListParameterDefinition *key = new ActionTools::ListParameterDefinition("key", tr("Key"), this);
			key->setTooltip(tr("The registry key to write to"));
			key->setItems(ReadRegistryInstance::keys);
			key->setDefaultValue(ReadRegistryInstance::keys.second.at(ActionTools::Registry::CurrentUser));
			addElement(key);

			ActionTools::TextParameterDefinition *subKey = new ActionTools::TextParameterDefinition("subKey", tr("Subkey"), this);
			subKey->setTooltip(tr("The registry subkey to write to"));
			addElement(subKey);

			ActionTools::TextParameterDefinition *value = new ActionTools::TextParameterDefinition("value", tr("Value"), this);
			value->setTooltip(tr("The value to write to"));
			addElement(value);

			ActionTools::TextParameterDefinition *data = new ActionTools::TextParameterDefinition("data", tr("Data"), this);
			data->setTooltip(tr("The data to write"));
			addElement(data);

			addException(WriteRegistryInstance::CannotFindSubKeyException, tr("Cannot find subKey"));
			addException(WriteRegistryInstance::CannotWriteValueException, tr("Cannot write value"));
	#endif
		}

		QString name() const													{ return QObject::tr("Write registry"); }
		QString id() const														{ return "ActionWriteRegistry"; }
		ActionTools::Flag flags() const											{ return ActionTools::WorksOnWindows | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Writes an entry to the registry"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new WriteRegistryInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Data; }
		QPixmap icon() const													{ return QPixmap(":/icons/writeregistry.png"); }

	private:
		Q_DISABLE_COPY(WriteRegistryDefinition)
	};
}

#endif // WRITEREGISTRYDEFINITION_H
