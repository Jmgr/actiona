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

#ifndef PIXELCOLORDEFINITION_H
#define PIXELCOLORDEFINITION_H

#include "actiondefinition.h"
#include "actions/pixelcolorinstance.h"
#include "colorpositionparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "variableparameterdefinition.h"
#include "ifactionparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class PixelColorDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit PixelColorDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::ColorPositionParameterDefinition *pixel = new ActionTools::ColorPositionParameterDefinition("pixel", tr("Pixel"), this);
			pixel->setTooltip(tr("The pixel position and color to check"));
			addElement(pixel);

			ActionTools::IfActionParameterDefinition *ifEqual = new ActionTools::IfActionParameterDefinition("ifEqual", tr("If equal"), this);
			ifEqual->setTooltip(tr("What to to if the pixel is from the specified color"));
			addElement(ifEqual);

			ActionTools::IfActionParameterDefinition *ifDifferent = new ActionTools::IfActionParameterDefinition("ifDifferent", tr("If different"), this);
			ifDifferent->setTooltip(tr("What to to if the pixel is not from the specified color"));
			addElement(ifDifferent);

			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition("variable", tr("Variable"), this);
			variable->setTooltip(tr("Variable name where to store the pixel color"));
			addElement(variable, 1);
		}

		QString name() const													{ return QObject::tr("Pixel color"); }
		QString id() const														{ return "ActionPixelColor"; }
		Flag flags() const														{ return ActionDefinition::flags() | Official; }
		QString description() const												{ return QObject::tr("Check a pixel color on the screen"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new PixelColorInstance(this); }
		Category category() const												{ return System; }
		QPixmap icon() const													{ return QPixmap(":/icons/clipboard.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(PixelColorDefinition)
	};

	#endif // PIXELCOLORDEFINITION_H
}
