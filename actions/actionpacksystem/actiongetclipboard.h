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

#ifndef ACTIONGETCLIPBOARD_H
#define ACTIONGETCLIPBOARD_H

#include "actioninterface.h"
#include "actiongetclipboardinstance.h"
#include "textparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPackInterface;
	class Action;
}

class ActionGetClipboard : public QObject, public ActionTools::ActionInterface
{
   Q_OBJECT

public:
	explicit ActionGetClipboard(ActionTools::ActionPackInterface *pack)
	: ActionGetClipboard::ActionInterface(pack)
	{
		ActionTools::VariableParameterDefinition *output = new ActionTools::VariableParameterDefinition( ActionTools::ElementDefinition::OUTPUT,
																								"variable",
																								 tr("Variable"),
																								 this);
		output->setTooltip(tr("The variable where to save the current clipboard value"));
		addElement(output);
	}

	QString name() const											{ return QObject::tr("Get clipboard"); }
	QString id() const												{ return metaObject()->className(); }
	Flag flags() const												{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const										{ return QObject::tr("Get the clipboard contents"); }
	Tools::Version version() const									{ return Tools::Version(1, 0, 0); }
	ActionTools::Action *newAction()								{ return new ActionGetClipboardInstance(this, 0); }
	Status status() const											{ return Stable; }
	Category category() const										{ return System; }
	QString author() const											{ return "The Actionaz Team"; }
	QString website() const											{ return "www.actionaz.eu"; }
	QString email() const											{ return "jmgr@jmgr.info"; }
	QPixmap icon() const											{ return QPixmap(":/icons/clipboard.png"); }
	ActionTools::Action *scriptInit(QScriptEngine *scriptEngine)	{ SCRIPT_INIT(ActionGetClipboard) }

private:
	Q_DISABLE_COPY(ActionGetClipboard)
};

#endif // ACTIONGETCLIPBOARD_H
