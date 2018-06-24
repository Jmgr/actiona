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

	Contact : jmgr@jmgr.info
*/

#pragma once

#include "actiondefinition.h"
#include "variableinstance.h"
#include "variableparameterdefinition.h"
#include "textparameterdefinition.h"
#include "listparameterdefinition.h"
#include "colorparameterdefinition.h"
#include "positionparameterdefinition.h"
#include "groupdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class VariableDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit VariableDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("VariableInstance::types", VariableInstance::types);

			auto variable = new ActionTools::VariableParameterDefinition(ActionTools::Name(QStringLiteral("variable"), tr("Variable")), this);
			variable->setCategory(ActionTools::ElementDefinition::INPUT);
			variable->setTooltip(tr("The variable name"));
			addElement(variable);

			auto type = new ActionTools::ListParameterDefinition(ActionTools::Name(QStringLiteral("type"), tr("Type")), this);
			type->setTooltip(tr("The variable type"));
			type->setItems(VariableInstance::types);
			type->setDefaultValue(VariableInstance::types.second.at(VariableInstance::String));
			addElement(type);

			auto singleValueGroup = new ActionTools::GroupDefinition(this);
			singleValueGroup->setMasterList(type);
			singleValueGroup->setMasterValues(QStringList()
											  << VariableInstance::types.first.at(VariableInstance::String)
											  << VariableInstance::types.first.at(VariableInstance::Integer)
											  << VariableInstance::types.first.at(VariableInstance::Float)
											  );

			auto simpleValue = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("value"), tr("Value")), this);
			simpleValue->setTooltip(tr("The variables new value"));
			singleValueGroup->addMember(simpleValue);

			addElement(singleValueGroup);

			auto colorValueGroup = new ActionTools::GroupDefinition(this);
			colorValueGroup->setMasterList(type);
			colorValueGroup->setMasterValues(QStringList()
											  << VariableInstance::types.first.at(VariableInstance::Color)
											  );

			auto colorValue = new ActionTools::ColorParameterDefinition(ActionTools::Name(QStringLiteral("colorValue"), tr("Color")), this);
			colorValue->setTooltip(tr("The variables new value"));
			colorValueGroup->addMember(colorValue);

			addElement(colorValueGroup);

			auto positionValueGroup = new ActionTools::GroupDefinition(this);
			positionValueGroup->setMasterList(type);
			positionValueGroup->setMasterValues(QStringList()
											  << VariableInstance::types.first.at(VariableInstance::Position)
											  );

			auto positionValue = new ActionTools::PositionParameterDefinition(ActionTools::Name(QStringLiteral("positionValue"), tr("Position")), this);
			positionValue->setTooltip(tr("The variables new value"));
			positionValueGroup->addMember(positionValue);

			addElement(positionValueGroup);

			addException(VariableInstance::ConversionFailedException, tr("Conversion failed"));
		}

		QString name() const override													{ return QObject::tr("Variable"); }
		QString id() const override														{ return QStringLiteral("ActionVariable"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Set the value of a variable"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new VariableInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Internal; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/variable.png")); }

	private:
		Q_DISABLE_COPY(VariableDefinition)
	};
}

