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

#include "actiontools/lineparameterdefinition.hpp"
#include "actiontools/linecombobox.hpp"
#include "actiontools/script.hpp"
#include "actiontools/actioninstance.hpp"

#include <QDebug>

namespace ActionTools
{
	void LineParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

        script->updateLineModel();

        mLineComboBox = new LineComboBox(*script, parent);

        addEditor(mLineComboBox);
    }

    void LineParameterDefinition::load(const ActionInstance *actionInstance)
    {
        mLineComboBox->setFromSubParameter(actionInstance->subParameter(name().original(), QStringLiteral("value")));
    }

    void LineParameterDefinition::save(ActionInstance *actionInstance)
    {
        actionInstance->setSubParameter(name().original(), QStringLiteral("value"), mLineComboBox->isCode(), mLineComboBox->currentText());
    }

    void LineParameterDefinition::actionUpdate(Script *script)
	{
        script->updateLineModel();
	}
}
