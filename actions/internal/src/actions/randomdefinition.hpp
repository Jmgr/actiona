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
#include "randominstance.hpp"
#include "actiontools/variableparameterdefinition.hpp"
#include "actiontools/textparameterdefinition.hpp"
#include "actiontools/listparameterdefinition.hpp"
#include "actiontools/numberparameterdefinition.hpp"
#include "actiontools/numberrangeparameterdefinition.hpp"
#include "actiontools/groupdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
    class RandomDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
        explicit RandomDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            translateItems("RandomInstance::types", RandomInstance::types);

            auto &variable = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("variable"), tr("Variable")});
            variable.setCategory(ActionTools::ElementDefinition::INPUT);
            variable.setTooltip(tr("The variable name"));

            auto &type = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("type"), tr("Type")});
            type.setTooltip(tr("The variable type"));
            type.setItems(RandomInstance::types);
            type.setDefaultValue(RandomInstance::types.second.at(RandomInstance::Integer));

            auto &integerGroup = addGroup();
            integerGroup.setMasterList(type);
            integerGroup.setMasterValues({RandomInstance::types.first.at(RandomInstance::Integer)});

            auto &integerValue = integerGroup.addParameter<ActionTools::IntegerRangeParameterDefinition>({QStringLiteral("int_minmax"), tr("Min/max (integer)")});
            integerValue.setDefaultValue(QStringLiteral("0"), QStringLiteral("min"));
            integerValue.setDefaultValue(QStringLiteral("100"), QStringLiteral("max"));
            integerValue.setTooltip(tr("The minimum and maximum values"));

            auto &decimalGroup = addGroup();
            decimalGroup.setMasterList(type);
            decimalGroup.setMasterValues({RandomInstance::types.first.at(RandomInstance::Decimal)});

            auto &decimalValue = decimalGroup.addParameter<ActionTools::DoubleRangeParameterDefinition>({QStringLiteral("decimal_minmax"), tr("Min/max (decimal)")});
            decimalValue.setDefaultValue(QStringLiteral("0.000"), QStringLiteral("min"));
            decimalValue.setDefaultValue(QStringLiteral("100.000"), QStringLiteral("max"));
            decimalValue.setTooltip(tr("The minimum and maximum values"));

            auto &stringGroup = addGroup();
            stringGroup.setMasterList(type);
            stringGroup.setMasterValues({RandomInstance::types.first.at(RandomInstance::String)});

            auto &stringCharacters = stringGroup.addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("string_characters"), tr("Characters")});
            stringCharacters.setDefaultValue(QStringLiteral("abcdefghijklmnopqrstuvwxyz0123456789"));
            stringCharacters.setTooltip(tr("The allowed characters"));

            auto &stringLength = stringGroup.addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("string_length"), tr("Length")});
            stringLength.setMinimum(0);
            stringLength.setMaximum(std::numeric_limits<int>::max());
            stringLength.setDefaultValue(QStringLiteral("10"));
            stringLength.setTooltip(tr("The generated string's length"));

            addException(RandomInstance::RangeException, tr("Invalid range"));
		}

        QString name() const override													{ return QObject::tr("Random"); }
        QString id() const override														{ return QStringLiteral("ActionRandom"); }
        QString description() const override												{ return QObject::tr("Generates random values"); }
        ActionTools::ActionInstance *newActionInstance() const override					{ return new RandomInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Internal; }
        QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/random.png")); }

	private:
        Q_DISABLE_COPY(RandomDefinition)
	};
}

