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

#include "actiontools/numberrangeparameterdefinition.hpp"
#include "actiontools/subparameter.hpp"
#include "actiontools/codespinbox.hpp"
#include "actiontools/codedoublespinbox.hpp"
#include "actiontools/actioninstance.hpp"

namespace ActionTools
{
    IntegerRangeParameterDefinition::IntegerRangeParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
        mMinSpinBox(nullptr),
        mMaxSpinBox(nullptr)
	{
	}

    void IntegerRangeParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

        mMinSpinBox = new CodeSpinBox(parent);
        mMaxSpinBox = new CodeSpinBox(parent);

        mMinSpinBox->setMinimum(std::numeric_limits<int>::min());
        mMinSpinBox->setMaximum(std::numeric_limits<int>::max());
        mMaxSpinBox->setMinimum(std::numeric_limits<int>::min());
        mMaxSpinBox->setMaximum(std::numeric_limits<int>::max());

        addEditor(mMinSpinBox);
        addEditor(mMaxSpinBox);
	}

    void IntegerRangeParameterDefinition::load(const ActionInstance *actionInstance)
	{
        auto subParameter = actionInstance->subParameter(name().original(), QStringLiteral("min"));
        mMinSpinBox->setFromSubParameter(subParameter);

        subParameter = actionInstance->subParameter(name().original(), QStringLiteral("max"));
        mMaxSpinBox->setFromSubParameter(subParameter);
	}

    void IntegerRangeParameterDefinition::save(ActionInstance *actionInstance)
	{
        QString text = mMinSpinBox->text();
        actionInstance->setSubParameter(name().original(), QStringLiteral("min"), mMinSpinBox->isCode(), text);

        text = mMaxSpinBox->text();
        actionInstance->setSubParameter(name().original(), QStringLiteral("max"), mMaxSpinBox->isCode(), text);
	}

    DoubleRangeParameterDefinition::DoubleRangeParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
        mMinSpinBox(nullptr),
        mMaxSpinBox(nullptr)
    {
    }

    void DoubleRangeParameterDefinition::buildEditors(Script *script, QWidget *parent)
    {
        ParameterDefinition::buildEditors(script, parent);

        mMinSpinBox = new CodeDoubleSpinBox(parent);
        mMaxSpinBox = new CodeDoubleSpinBox(parent);

        mMinSpinBox->setMinimum(std::numeric_limits<double>::lowest());
        mMinSpinBox->setMaximum(std::numeric_limits<double>::max());
        mMinSpinBox->setDecimals(3);
        mMaxSpinBox->setMinimum(std::numeric_limits<double>::lowest());
        mMaxSpinBox->setMaximum(std::numeric_limits<double>::max());
        mMaxSpinBox->setDecimals(3);

        addEditor(mMinSpinBox);
        addEditor(mMaxSpinBox);
    }

    void DoubleRangeParameterDefinition::load(const ActionInstance *actionInstance)
    {
        auto subParameter = actionInstance->subParameter(name().original(), QStringLiteral("min"));
        mMinSpinBox->setFromSubParameter(subParameter);

        subParameter = actionInstance->subParameter(name().original(), QStringLiteral("max"));
        mMaxSpinBox->setFromSubParameter(subParameter);
    }

    void DoubleRangeParameterDefinition::save(ActionInstance *actionInstance)
    {
        QString text = mMinSpinBox->text();
        actionInstance->setSubParameter(name().original(), QStringLiteral("min"), mMinSpinBox->isCode(), text);

        text = mMaxSpinBox->text();
        actionInstance->setSubParameter(name().original(), QStringLiteral("max"), mMaxSpinBox->isCode(), text);
    }
}
