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

#include "keyboardkeyparameterdefinition.h"
#include "keyboardkeyedit.h"
#include "actioninstance.h"
#include "codelineedit.h"

namespace ActionTools
{
    KeyboardKeyParameterDefinition::KeyboardKeyParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
        mKeyboardKeyEdit(nullptr)
	{
	}

    void KeyboardKeyParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

        mKeyboardKeyEdit = new KeyboardKeyEdit(parent);

        addEditor(mKeyboardKeyEdit);
	}

    void KeyboardKeyParameterDefinition::load(const ActionInstance *actionInstance)
	{
        const SubParameter &key = actionInstance->subParameter(name().original(), QStringLiteral("value"));

		if(key.isCode())
            mKeyboardKeyEdit->setFromSubParameter(key);
		else
            mKeyboardKeyEdit->setKeys(KeyboardKey::loadKeyListFromJson(key.value()));
	}

    void KeyboardKeyParameterDefinition::save(ActionInstance *actionInstance)
	{
		const QString &originalName = name().original();

        if(mKeyboardKeyEdit->isCode())
            actionInstance->setSubParameter(originalName, QStringLiteral("value"), true, mKeyboardKeyEdit->text());
        else
            actionInstance->setSubParameter(originalName, QStringLiteral("value"), false, KeyboardKey::saveKeyListToJson(mKeyboardKeyEdit->keys()));
	}
	
    void KeyboardKeyParameterDefinition::setDefaultValues(ActionInstance *actionInstance)
	{
		Q_UNUSED(actionInstance)

		//No default values for now
	}
}
