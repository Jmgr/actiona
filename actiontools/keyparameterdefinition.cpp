/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#include "keyparameterdefinition.h"
#include "keyedit.h"
#include "actioninstance.h"
#include "codelineedit.h"

#include <QDebug>

namespace ActionTools
{
	KeyParameterDefinition::KeyParameterDefinition(const QString &name, const QString &translatedName, QObject *parent)
		: ParameterDefinition(name, translatedName, parent),
		mKeyEdit(0)
	{
	}

	void KeyParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mKeyEdit = new KeyEdit(parent);

		addEditor(mKeyEdit);
	}

	void KeyParameterDefinition::load(const ActionInstance *actionInstance)
	{
		const SubParameter &key = actionInstance->subParameter(name(), "key");
		if(key.isCode())
			mKeyEdit->codeLineEdit()->setFromSubParameter(key);
		else
		{
			KeyInput keyInput;
			
			keyInput.fromPortableText(key.value().toString(), actionInstance->subParameter(name(), "isQtKey").value().toBool());
			
			mKeyEdit->setKeyInput(keyInput);
		}
	}

	void KeyParameterDefinition::save(ActionInstance *actionInstance)
	{
		if(mKeyEdit->isCode())
		{
			actionInstance->setSubParameter(name(), "key", mKeyEdit->isCode(), mKeyEdit->codeLineEdit()->text());
			actionInstance->setSubParameter(name(), "isQtKey", QVariant(false));
		}
		else
		{
			const KeyInput &keyInput = mKeyEdit->keyInput();
			actionInstance->setSubParameter(name(), "key", keyInput.toPortableText());
			actionInstance->setSubParameter(name(), "isQtKey", QVariant(keyInput.isQtKey()));
		}
	}
	
	void KeyParameterDefinition::setDefaultValues(ActionInstance *actionInstance)
	{
		Q_UNUSED(actionInstance)

		//No default values for now
	}
}
