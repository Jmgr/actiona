/*
	Actiona
	Copyright (C) 2005 Jonathan Mercier-Ganady

	Actiona is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actiona is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact: jmgr@jmgr.info
*/

#pragma once

#include "actiontools/actiondefinition.hpp"
#include "pixelcolorinstance.hpp"
#include "actiontools/colorpositionparameterdefinition.hpp"
#include "actiontools/listparameterdefinition.hpp"
#include "actiontools/numberparameterdefinition.hpp"
#include "actiontools/variableparameterdefinition.hpp"
#include "actiontools/ifactionparameterdefinition.hpp"
#include "actiontools/positionparameterdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class PixelColorDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit PixelColorDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("PixelColorInstance::comparisons", PixelColorInstance::comparisons);

            auto &pixel = addParameter<ActionTools::ColorPositionParameterDefinition>({QStringLiteral("pixel"), tr("Pixel")});
            pixel.setTooltip(tr("The pixel position and color to check"));

            auto &comparison = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("comparison"), tr("Comparison")});
            comparison.setTooltip(tr("The comparison"));
            comparison.setItems(PixelColorInstance::comparisons);
            comparison.setDefaultValue(PixelColorInstance::comparisons.second.at(PixelColorInstance::Equal));

            auto &ifTrue = addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifTrue"), tr("If true")});
            ifTrue.setTooltip(tr("What to to if the pixel comparison is true"));

            auto &ifFalse = addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifFalse"), tr("If false")});
            ifFalse.setTooltip(tr("What to to if the pixel comparison is false"));
            ifFalse.setAllowWait(true);

            auto &variable = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("variable"), tr("Pixel color variable")}, 1);
            variable.setTooltip(tr("Variable name where to store the pixel color"));

            auto &redTolerance = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("redTolerance"), tr("Red tolerance")}, 1);
            redTolerance.setTooltip(tr("The tolerance percentage for the red color component"));
            redTolerance.setMinimum(0);
            redTolerance.setMaximum(100);
            redTolerance.setDefaultValue(QStringLiteral("0"));

            auto &greenTolerance = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("greenTolerance"), tr("Green tolerance")}, 1);
            greenTolerance.setTooltip(tr("The tolerance percentage for the green color component"));
            greenTolerance.setMinimum(0);
            greenTolerance.setMaximum(100);
            greenTolerance.setDefaultValue(QStringLiteral("0"));

            auto &blueTolerance = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("blueTolerance"), tr("Blue tolerance")}, 1);
            blueTolerance.setTooltip(tr("The tolerance percentage for the blue color component"));
            blueTolerance.setMinimum(0);
            blueTolerance.setMaximum(100);
            blueTolerance.setDefaultValue(QStringLiteral("0"));

            auto &positionOffset = addParameter<ActionTools::PositionParameterDefinition>({QStringLiteral("positionOffset"), tr("Offset")}, 1);
            positionOffset.setTooltip(tr("The offset to apply to the pixel position"));
		}

		QString name() const override													{ return QObject::tr("Pixel color"); }
		QString id() const override														{ return QStringLiteral("ActionPixelColor"); }
		QString description() const override												{ return QObject::tr("Check a pixel color on the screen"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new PixelColorInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::System; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/pixelcolor.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(PixelColorDefinition)
	};
}

