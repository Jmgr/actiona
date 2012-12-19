/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#include "variableparameterdefinition.h"
#include "codelineedit.h"
#include "codecombobox.h"
#include "script.h"
#include "actioninstance.h"

namespace ActionTools
{
	void VariableParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

        mComboBox = new CodeComboBox(parent);
        mComboBox->codeLineEdit()->setRegexpValidation(ActionInstance::NameRegExp);

        mComboBox->addItems(script->variables());

        addEditor(mComboBox);

        emit editorBuilt();
    }

    void VariableParameterDefinition::update(Script *script)
    {
        mComboBox->clear();
        mComboBox->addItems(script->variables());

        //TODO: Add a button to insert variables
        //TODO: Compute the variable list only once
    }
}
