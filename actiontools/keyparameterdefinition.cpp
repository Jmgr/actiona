/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

namespace ActionTools
{
	KeyParameterDefinition::KeyParameterDefinition(Category category, const QString &name, const QString &translatedName, QObject *parent)
		: ParameterDefinition(category, name, translatedName, parent),
		mKeyEdit(0)
	{
	}

	void KeyParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mKeyEdit = new KeyEdit(parent);
		mKeyEdit->setObjectName("value");

		addEditor(mKeyEdit);
	}

	void KeyParameterDefinition::load(const ActionInstance *actionInstance)
	{
		//mKeyEdit->codeLineEdit()->setFromSubParameter(actionInstance->subParameter(name(), "value"));
	}

	void KeyParameterDefinition::save(ActionInstance *actionInstance)
	{
		//actionInstance->setSubParameter(name(), "value", mKeyEdit->codeLineEdit()->isCode(), mKeyEdit->codeLineEdit()->text());
	}
}
