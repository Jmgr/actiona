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

#ifndef ACTIONWRITEREGISTRYDEFINITION_H
#define ACTIONWRITEREGISTRYDEFINITION_H

#include "actiondefinition.h"
#include "actionwriteregistryinstance.h"
#include "actionreadregistryinstance.h"
#include "listparameterdefinition.h"
#include "textparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionWriteRegistryDefinition : public QObject, public ActionTools::ActionDefinition
{
	Q_OBJECT

public:
	explicit ActionWriteRegistryDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
	{
#ifdef Q_WS_WIN
		ActionTools::ListParameterDefinition *key = new ActionTools::ListParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"key",
																								tr("Key"),
																								this);
		key->setTooltip(tr("The registry key to write to"));
		key->setItems(ActionReadRegistryInstance::keys);
		key->setDefaultValue(ActionReadRegistryInstance::keys.second.at(ActionTools::Registry::CurrentUser));
		addElement(key);

		ActionTools::TextParameterDefinition *subKey = new ActionTools::TextParameterDefinition( ActionTools::ElementDefinition::INPUT,
																							   "subKey",
																							   tr("Subkey"),
																							   this);
		subKey->setTooltip(tr("The registry subkey to write to"));
		addElement(subKey);

		ActionTools::TextParameterDefinition *value = new ActionTools::TextParameterDefinition( ActionTools::ElementDefinition::INPUT,
																							   "value",
																							   tr("Value"),
																							   this);
		value->setTooltip(tr("The value to write to"));
		addElement(value);

		ActionTools::TextParameterDefinition *data = new ActionTools::TextParameterDefinition( ActionTools::ElementDefinition::INPUT,
																							   "data",
																							   tr("Data"),
																							   this);
		data->setTooltip(tr("The data to write"));
		addElement(data);

		addException(ActionWriteRegistryInstance::CannotFindSubKeyException, tr("Cannot find subKey"));
		addException(ActionWriteRegistryInstance::CannotWriteValueException, tr("Cannot write value"));
#endif
	}

	QString name() const													{ return QObject::tr("Write registry"); }
	QString id() const														{ return "ActionWriteRegistry"; }
	Flag flags() const														{ return WorksOnWindows | Official; }
	QString description() const												{ return QObject::tr("Writes an entry to the registry"); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionWriteRegistryInstance(this); }
	Category category() const												{ return Data; }
	QPixmap icon() const													{ return QPixmap(":/icons/clipboard.png"); }

private:
	Q_DISABLE_COPY(ActionWriteRegistryDefinition)
};

#endif // ACTIONWRITEREGISTRYDEFINITION_H
