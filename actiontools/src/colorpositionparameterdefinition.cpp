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

#include "actiontools/colorpositionparameterdefinition.hpp"
#include "actiontools/subparameter.hpp"
#include "actiontools/positionedit.hpp"
#include "actiontools/coloredit.hpp"
#include "actiontools/actioninstance.hpp"

namespace ActionTools
{
    ColorPositionParameterDefinition::ColorPositionParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
		mPositionEdit(nullptr),
		mColorEdit(nullptr)
	{
	}

	void ColorPositionParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);
		
		mPositionEdit = new PositionEdit(parent);

		addEditor(mPositionEdit);
	
		mColorEdit = new ColorEdit(parent);
		mColorEdit->setChooseByPositionButtonVisible(false);

		addEditor(mColorEdit);
		
        connect(mPositionEdit, &PositionEdit::positionChosen, mColorEdit, &ColorEdit::setPosition);
        connect(mColorEdit, &ColorEdit::positionChosen, mPositionEdit, &PositionEdit::setPosition);
	}

	void ColorPositionParameterDefinition::load(const ActionInstance *actionInstance)
	{
		mPositionEdit->setFromSubParameter(actionInstance->subParameter(name().original(), QStringLiteral("position")));
		mColorEdit->setFromSubParameter(actionInstance->subParameter(name().original(), QStringLiteral("color")));
	}

	void ColorPositionParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), QStringLiteral("position"), mPositionEdit->isCode(), mPositionEdit->text());
		actionInstance->setSubParameter(name().original(), QStringLiteral("color"), mColorEdit->isCode(), mColorEdit->text());
	}

    void ColorPositionParameterDefinition::applyDefaultValuesTo(ActionInstance *actionInstance)
	{
        auto position = defaultPosition();

		actionInstance->setSubParameter(name().original(), QStringLiteral("position"), QStringLiteral("%1:%2").arg(position.x()).arg(position.y()));

        QColor localDefaultColor = defaultColor();
		actionInstance->setSubParameter(name().original(), QStringLiteral("color"), QStringLiteral("%1:%2:%3").arg(localDefaultColor.red()).arg(localDefaultColor.green()).arg(localDefaultColor.blue()));
	}
}
