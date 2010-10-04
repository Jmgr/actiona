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

#ifndef ACTIONREADREGISTRYDEFINITION_H
#define ACTIONREADREGISTRYDEFINITION_H

#include "actiondefinition.h"
#include "actionreadregistryinstance.h"
#include "listparameterdefinition.h"
#include "textparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionReadRegistryDefinition : public QObject, public ActionTools::ActionDefinition
{
	Q_OBJECT

public:
	explicit ActionReadRegistryDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
	{
#ifdef Q_WS_WIN
		ActionTools::ListParameterDefinition *key = new ActionTools::ListParameterDefinition("key", tr("Key"), this);
		key->setTooltip(tr("The registry key to read from"));
		key->setItems(ActionReadRegistryInstance::keys);
		key->setDefaultValue(ActionReadRegistryInstance::keys.second.at(ActionTools::Registry::CurrentUser));
		addElement(key);

		ActionTools::TextParameterDefinition *subKey = new ActionTools::TextParameterDefinition("subKey", tr("Subkey"), this);
		subKey->setTooltip(tr("The registry subkey to read from"));
		addElement(subKey);

		ActionTools::TextParameterDefinition *value = new ActionTools::TextParameterDefinition("value", tr("Value"), this);
		value->setTooltip(tr("The value to read"));
		addElement(value);

		ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition("variable", tr("Variable"), this);
		variable->setTooltip(tr("The variable where to save the value read from the registry"));
		addElement(variable);

		addException(ActionReadRegistryInstance::CannotFindSubKeyException, tr("Cannot find subKey"));
		addException(ActionReadRegistryInstance::CannotFindValueException, tr("Cannot find value"));
		addException(ActionReadRegistryInstance::InvalidValueType, tr("Invalid value type"));
#endif
	}

	QString name() const													{ return QObject::tr("Read registry"); }
	QString id() const														{ return "ActionReadRegistry"; }
	Flag flags() const														{ return WorksOnWindows | Official; }
	QString description() const												{ return QObject::tr("Read an entry from the registry"); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionReadRegistryInstance(this); }
	Category category() const												{ return Data; }
	QPixmap icon() const													{ return QPixmap(":/icons/clipboard.png"); }

private:
	Q_DISABLE_COPY(ActionReadRegistryDefinition)
};

#endif // ACTIONREADREGISTRYDEFINITION_H
