/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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
#include "pixelcolorinstance.h"
#include "colorpositionparameterdefinition.h"
#include "listparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "variableparameterdefinition.h"
#include "ifactionparameterdefinition.h"
#include "positionparameterdefinition.h"

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
			translateItems("PixelColorInstance::comparisons", PixelColorInstance::comparisons);

			ActionTools::ColorPositionParameterDefinition *pixel = new ActionTools::ColorPositionParameterDefinition(ActionTools::Name("pixel", tr("Pixel")), this);
			pixel->setTooltip(tr("The pixel position and color to check"));
			addElement(pixel);

			ActionTools::ListParameterDefinition *comparison = new ActionTools::ListParameterDefinition(ActionTools::Name("comparison", tr("Comparison")), this);
			comparison->setTooltip(tr("The comparison"));
			comparison->setItems(PixelColorInstance::comparisons);
			comparison->setDefaultValue(PixelColorInstance::comparisons.second.at(PixelColorInstance::Equal));
			addElement(comparison);

			ActionTools::IfActionParameterDefinition *ifTrue = new ActionTools::IfActionParameterDefinition(ActionTools::Name("ifTrue", tr("If true")), this);
			ifTrue->setTooltip(tr("What to to if the pixel comparison is true"));
			addElement(ifTrue);

			ActionTools::IfActionParameterDefinition *ifFalse = new ActionTools::IfActionParameterDefinition(ActionTools::Name("ifFalse", tr("If false")), this);
			ifFalse->setTooltip(tr("What to to if the pixel comparison is false"));
			ifFalse->setAllowWait(true);
			addElement(ifFalse);

			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition(ActionTools::Name("variable", tr("Pixel color variable")), this);
			variable->setTooltip(tr("Variable name where to store the pixel color"));
			addElement(variable, 1);

			ActionTools::NumberParameterDefinition *redTolerance = new ActionTools::NumberParameterDefinition(ActionTools::Name("redTolerance", tr("Red tolerance")), this);
			redTolerance->setTooltip(tr("The tolerance percentage for the red color component"));
			redTolerance->setMinimum(0);
			redTolerance->setMaximum(100);
			redTolerance->setDefaultValue(0);
			addElement(redTolerance, 1);

			ActionTools::NumberParameterDefinition *greenTolerance = new ActionTools::NumberParameterDefinition(ActionTools::Name("greenTolerance", tr("Green tolerance")), this);
			greenTolerance->setTooltip(tr("The tolerance percentage for the green color component"));
			greenTolerance->setMinimum(0);
			greenTolerance->setMaximum(100);
			greenTolerance->setDefaultValue(0);
			addElement(greenTolerance, 1);

			ActionTools::NumberParameterDefinition *blueTolerance = new ActionTools::NumberParameterDefinition(ActionTools::Name("blueTolerance", tr("Blue tolerance")), this);
			blueTolerance->setTooltip(tr("The tolerance percentage for the blue color component"));
			blueTolerance->setMinimum(0);
			blueTolerance->setMaximum(100);
			blueTolerance->setDefaultValue(0);
			addElement(blueTolerance, 1);

            ActionTools::PositionParameterDefinition *positionOffset = new ActionTools::PositionParameterDefinition(ActionTools::Name("positionOffset", tr("Offset")), this);
            positionOffset->setTooltip(tr("The offset to apply to the pixel position"));
            addElement(positionOffset, 1);
		}

		QString name() const													{ return QObject::tr("Pixel color"); }
		QString id() const														{ return "ActionPixelColor"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Check a pixel color on the screen"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new PixelColorInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::System; }
		QPixmap icon() const													{ return QPixmap(":/icons/pixelcolor.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(PixelColorDefinition)
	};
}

#endif // PIXELCOLORDEFINITION_H
