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

#ifndef ACTIONTEXTINPUTDEFINITION_H
#define ACTIONTEXTINPUTDEFINITION_H

#include "actiondefinition.h"
#include "actiontextinputinstance.h"
#include "textparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionTextInputDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionTextInputDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::TextParameterDefinition *question = new ActionTools::TextParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"question",
																								 tr("Question"),
																								 this);
		question->setTooltip(tr("The question to ask"));
		addElement(question);
		
		ActionTools::TextParameterDefinition *defaultValue = new ActionTools::TextParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"defaultValue",
																								 tr("Default value"),
																								 this);
		defaultValue->setTooltip(tr("The default value"));
		addElement(defaultValue);
		
		ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition(	ActionTools::ElementDefinition::OUTPUT,
																								"variable",
																								 tr("Variable"),
																								 this);
		variable->setTooltip(tr("The variable where to save the entered input"));
		addElement(variable);
	}

	QString name() const													{ return QObject::tr("Text input"); }
	QString id() const														{ return "ActionTextInput"; }
	Flag flags() const														{ return ActionDefinition::flags() | Official; }
	QString description() const												{ return QObject::tr("Ask the user to enter some text"); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionTextInputInstance(this); }
	Category category() const												{ return Windows; }
	QPixmap icon() const													{ return QPixmap(":/icons/msg.png"); }

private:
	Q_DISABLE_COPY(ActionTextInputDefinition)
};

#endif // ACTIONTEXTINPUTDEFINITION_H
