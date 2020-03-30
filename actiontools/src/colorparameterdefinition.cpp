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

#include "actiontools/colorparameterdefinition.hpp"
#include "actiontools/subparameter.hpp"
#include "actiontools/coloredit.hpp"
#include "actiontools/actioninstance.hpp"

namespace ActionTools
{
    ColorParameterDefinition::ColorParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
		mColorEdit(nullptr)
	{
	}

	void ColorParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mColorEdit = new ColorEdit(parent);

		addEditor(mColorEdit);
	}

	void ColorParameterDefinition::load(const ActionInstance *actionInstance)
	{
		mColorEdit->setFromSubParameter(actionInstance->subParameter(name().original(), QStringLiteral("value")));
	}

	void ColorParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), QStringLiteral("value"), mColorEdit->isCode(), mColorEdit->text());
	}
}
