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
#include "variableinstance.hpp"
#include "actiontools/variableparameterdefinition.hpp"
#include "actiontools/textparameterdefinition.hpp"
#include "actiontools/listparameterdefinition.hpp"
#include "actiontools/colorparameterdefinition.hpp"
#include "actiontools/positionparameterdefinition.hpp"
#include "actiontools/groupdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class VariableDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit VariableDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("VariableInstance::types", VariableInstance::types);

            auto &variable = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("variable"), tr("Variable")});
            variable.setCategory(ActionTools::ElementDefinition::INPUT);
            variable.setTooltip(tr("The variable name"));

            auto &type = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("type"), tr("Type")});
            type.setTooltip(tr("The variable type"));
            type.setItems(VariableInstance::types);
            type.setDefaultValue(VariableInstance::types.second.at(VariableInstance::String));

            auto &singleValueGroup = addGroup();
            singleValueGroup.setMasterList(type);
            singleValueGroup.setMasterValues({VariableInstance::types.first.at(VariableInstance::String), VariableInstance::types.first.at(VariableInstance::Integer), VariableInstance::types.first.at(VariableInstance::Float)});

            auto &simpleValue = singleValueGroup.addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("value"), tr("Value")});
            simpleValue.setTooltip(tr("The variables new value"));

            auto &colorValueGroup = addGroup();
            colorValueGroup.setMasterList(type);
            colorValueGroup.setMasterValues({VariableInstance::types.first.at(VariableInstance::Color)});

            auto &colorValue = colorValueGroup.addParameter<ActionTools::ColorParameterDefinition>({QStringLiteral("colorValue"), tr("Color")});
            colorValue.setTooltip(tr("The variables new value"));

            auto &positionValueGroup = addGroup();
            positionValueGroup.setMasterList(type);
            positionValueGroup.setMasterValues({VariableInstance::types.first.at(VariableInstance::Position)});

            auto &positionValue = positionValueGroup.addParameter<ActionTools::PositionParameterDefinition>({QStringLiteral("positionValue"), tr("Position")});
            positionValue.setTooltip(tr("The variables new value"));

			addException(VariableInstance::ConversionFailedException, tr("Conversion failed"));
		}

		QString name() const override													{ return QObject::tr("Variable"); }
		QString id() const override														{ return QStringLiteral("ActionVariable"); }
		QString description() const override												{ return QObject::tr("Set the value of a variable"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new VariableInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Internal; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/variable.png")); }

	private:
		Q_DISABLE_COPY(VariableDefinition)
	};
}

