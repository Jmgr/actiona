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

#include "keyparameterdefinition.h"
#include "keyedit.h"
#include "actioninstance.h"
#include "codelineedit.h"

#include <QDebug>

namespace ActionTools
{
    KeyParameterDefinition::KeyParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
		mKeyEdit(nullptr)
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
		const SubParameter &key = actionInstance->subParameter(name().original(), QStringLiteral("key"));
		if(key.isCode())
			mKeyEdit->codeLineEdit()->setFromSubParameter(key);
		else
		{
			KeyInput keyInput;
			
			keyInput.fromPortableText(key.value(), actionInstance->subParameter(name().original(), QStringLiteral("isQtKey")).value() == QLatin1String("true"));
			
			mKeyEdit->setKeyInput(keyInput);
		}
	}

	void KeyParameterDefinition::save(ActionInstance *actionInstance)
	{
		const QString &originalName = name().original();

		if(mKeyEdit->isCode())
		{
			actionInstance->setSubParameter(originalName, QStringLiteral("key"), mKeyEdit->isCode(), mKeyEdit->codeLineEdit()->text());
			actionInstance->setSubParameter(originalName, QStringLiteral("isQtKey"), QStringLiteral("false"));
		}
		else
		{
			const KeyInput &keyInput = mKeyEdit->keyInput();
			actionInstance->setSubParameter(originalName, QStringLiteral("key"), keyInput.toPortableText());
			actionInstance->setSubParameter(originalName, QStringLiteral("isQtKey"), keyInput.isQtKey() ? QStringLiteral("true") : QStringLiteral("false"));
		}
	}
	
	void KeyParameterDefinition::setDefaultValues(ActionInstance *actionInstance)
	{
		Q_UNUSED(actionInstance)

		//No default values for now
	}
}
