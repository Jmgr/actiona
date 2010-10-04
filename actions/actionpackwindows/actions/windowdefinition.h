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

#ifndef WINDOWDEFINITION_H
#define WINDOWDEFINITION_H

#include "actiondefinition.h"
#include "actions/windowinstance.h"
#include "textparameterdefinition.h"
#include "listparameterdefinition.h"
#include "groupdefinition.h"
#include "positionparameterdefinition.h"
#include "numberparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class WindowDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit WindowDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::TextParameterDefinition *title = new ActionTools::TextParameterDefinition("title", tr("Window title"), this);
			title->setTooltip(tr("The title of the window to find, you can use wildcards like * (any number of characters) or ? (one character) here"));
			addElement(title);

			ActionTools::ListParameterDefinition *action = new ActionTools::ListParameterDefinition("action", tr("Action"), this);
			action->setTooltip(tr("The condition to wait for"));
			action->setItems(WindowInstance::actions);
			action->setDefaultValue(WindowInstance::actions.second.at(WindowInstance::Close));
			addElement(action);

			ActionTools::GroupDefinition *moveGroup = new ActionTools::GroupDefinition("move", tr("Move"), this);
			moveGroup->setMasterList(action);
			moveGroup->setMasterValues(QStringList() << WindowInstance::actions.first.at(WindowInstance::Move));

			ActionTools::PositionParameterDefinition *movePosition = new ActionTools::PositionParameterDefinition("movePosition", tr("Move position"), this);
			movePosition->setTooltip(tr("The position to move the window"));
			moveGroup->addMember(movePosition);

			addElement(moveGroup);

			ActionTools::GroupDefinition *resizeGroup = new ActionTools::GroupDefinition("resize", tr("Resize"), this);
			resizeGroup->setMasterList(action);
			resizeGroup->setMasterValues(QStringList() << WindowInstance::actions.first.at(WindowInstance::Resize));

			ActionTools::NumberParameterDefinition *resizeWidth = new ActionTools::NumberParameterDefinition("resizeWidth", tr("Resize width"), this);
			resizeWidth->setTooltip(tr("The new width of the window"));
			resizeWidth->setMinimum(0);
			resizeWidth->setMaximum(INT_MAX);
			resizeGroup->addMember(resizeWidth);

			ActionTools::NumberParameterDefinition *resizeHeight = new ActionTools::NumberParameterDefinition("resizeHeight", tr("Resize height"), this);
			resizeHeight->setTooltip(tr("The new height of the window"));
			resizeHeight->setMinimum(0);
			resizeHeight->setMaximum(INT_MAX);
			resizeGroup->addMember(resizeHeight);

			addElement(resizeGroup);

			addException(WindowInstance::CannotFindWindowException, tr("Cannot find window"));
			addException(WindowInstance::ActionFailedException, tr("Action failed"));
		}

		QString name() const													{ return QObject::tr("Window"); }
		QString id() const														{ return "ActionWindow"; }
		Flag flags() const														{ return ActionDefinition::flags() | Official; }
		QString description() const												{ return QObject::tr("Do some action on a window"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new WindowInstance(this); }
		Category category() const												{ return Windows; }
		QPixmap icon() const													{ return QPixmap(":/icons/msg.png"); }

	private:
		Q_DISABLE_COPY(WindowDefinition)
	};
}

#endif // WINDOWDEFINITION_H
