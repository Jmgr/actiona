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

#ifndef ACTIONTEXTINPUT_H
#define ACTIONTEXTINPUT_H

#include "actioninterface.h"
#include "actiontextinputinstance.h"
#include "textparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPackInterface;
	class Action;
}

class ActionTextInput : public QObject, public ActionTools::ActionInterface
{
   Q_OBJECT

public:
	explicit ActionTextInput(ActionTools::ActionPackInterface *pack)
	: ActionTextInput::ActionInterface(pack)
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

	QString name() const											{ return QObject::tr("Text input"); }
	QString id() const												{ return metaObject()->className(); }
	Flag flags() const												{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const										{ return QObject::tr("Ask the user to enter some text"); }
	Tools::Version version() const									{ return Tools::Version(0, 0, 1); }
	ActionTools::Action *newAction()								{ return new ActionTextInputInstance(this, 0); }
	Status status() const											{ return Alpha; }
	Category category() const										{ return Windows; }
	QPixmap icon() const											{ return QPixmap(":/icons/msg.png"); }
	ActionTools::Action *scriptInit(QScriptEngine *scriptEngine)	{ SCRIPT_INIT(ActionTextInput) }

private:
	Q_DISABLE_COPY(ActionTextInput)
};

#endif // ACTIONTEXTINPUT_H
