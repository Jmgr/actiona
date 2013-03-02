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

#ifndef RECONFIGUREACTIONAZWINDOWDEFINITION_H
#define RECONFIGUREACTIONAZWINDOWDEFINITION_H

#include "actiondefinition.h"
#include "reconfigureactionazwindowinstance.h"
#include "textparameterdefinition.h"
#include "listparameterdefinition.h"
#include "screenpositionparameterdefinition.h"
#include "booleanparameterdefinition.h"
#include "positionparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ReconfigureActionazWindowDefinition : public QObject, public ActionTools::ActionDefinition
	{
		Q_OBJECT

	public:
		explicit ReconfigureActionazWindowDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::ScreenPositionParameterDefinition *executionPosition = new ActionTools::ScreenPositionParameterDefinition(ActionTools::Name("executionPosition", tr("Execution window")), this);
			executionPosition->setTooltip(tr("The position of the execution window"));
			executionPosition->setDefaultScreenPosition("Screen1:E");
			executionPosition->setDefaultWindowVisible(true);
			addElement(executionPosition);

			ActionTools::ScreenPositionParameterDefinition *consolePosition = new ActionTools::ScreenPositionParameterDefinition(ActionTools::Name("consolePosition", tr("Console window")), this);
			consolePosition->setTooltip(tr("The position of the console window"));
			consolePosition->setDefaultScreenPosition("Screen1:SE");
			consolePosition->setDefaultWindowVisible(false);
			addElement(consolePosition);

			ActionTools::BooleanParameterDefinition *clearConsole = new ActionTools::BooleanParameterDefinition(ActionTools::Name("clearConsole", tr("Clear the Console window")), this);
			clearConsole->setTooltip(tr("Check me, if you want to clear the Console window"));
			addElement(clearConsole, 1);

			ActionTools::PositionParameterDefinition *sizeOfConsole = new ActionTools::PositionParameterDefinition(ActionTools::Name("sizeOfConsole", tr("Select the size of the Console")), this);
			sizeOfConsole->setTooltip(tr("The size of the Console window"));
			addElement(sizeOfConsole, 1);
		}

		QString name() const													{ return QObject::tr("ReconfigureActionazWindow"); }
		QString id() const														{ return "ActionReconfigureActionazWindow"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Reconfigure the position of execution and console windows"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new ReconfigureActionazWindowInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/positionwindow.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(ReconfigureActionazWindowDefinition)
	};
}

#endif // RECONFIGUREACTIONAZWINDOWDEFINITION_H
