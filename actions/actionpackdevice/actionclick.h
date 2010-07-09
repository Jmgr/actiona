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

#ifndef ACTIONCLICK_H
#define ACTIONCLICK_H

#include "actioninterface.h"
#include "actionclickinstance.h"
#include "listparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "positionparameterdefinition.h"

#include <climits>

namespace ActionTools
{
	class ActionPackInterface;
	class Action;
}

class ActionClick : public QObject, public ActionTools::ActionInterface
{
   Q_OBJECT

public:
	explicit ActionClick(ActionTools::ActionPackInterface *pack)
	: ActionClick::ActionInterface(pack)
	{
		ActionTools::ListParameterDefinition *button = new ActionTools::ListParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"button",
																								tr("Button"),
																								this);
		button->setTooltip(tr("The button to simulate"));
		button->setItems(ActionClickInstance::buttons);
		button->setOption("default", ActionClickInstance::buttons.second.at(ActionClickInstance::LeftButton));
		addElement(button);

		ActionTools::NumberParameterDefinition *amount = new ActionTools::NumberParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"amount",
																								tr("Amount"),
																								this);
		amount->setTooltip(tr("The amount of clicks to simulate"));
		amount->setMinimum(1);
		amount->setMaximum(INT_MAX);
		amount->setOption("default", 1);
		addElement(amount);

		ActionTools::PositionParameterDefinition *position = new ActionTools::PositionParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"position",
																								 tr("Position"),
																								 this);
		position->setTooltip(tr("The screen position where to simulate a mouse click"));
		addElement(position);
	}

	QString name() const											{ return QObject::tr("Click"); }
	QString id() const												{ return metaObject()->className(); }
	Flag flags() const												{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const										{ return QObject::tr("Emulates a mouse click"); }
	Tools::Version version() const									{ return Tools::Version(0, 0, 1); }
	ActionTools::Action *newAction()								{ return new ActionClickInstance(this, 0); }
	Status status() const											{ return Alpha; }
	Category category() const										{ return Device; }
	QPixmap icon() const											{ return QPixmap(":/icons/code.png"); }
	ActionTools::Action *scriptInit(QScriptEngine *scriptEngine)	{ SCRIPT_INIT(ActionClick) }

private:
	Q_DISABLE_COPY(ActionClick)
};

#endif // ACTIONCLICK_H
