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

#include "textparameterdefinition.h"
#include "subparameter.h"
#include "codelineedit.h"
#include "actioninstance.h"

#include <QDebug>

namespace ActionTools
{
    TextParameterDefinition::TextParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
		mTextCodeMode(TextAndCode),
		mLineEdit(nullptr)
	{
	}

	void TextParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mLineEdit = new CodeLineEdit(parent);

		switch(mTextCodeMode)
		{
		case TextOnly:
			mLineEdit->setCode(false);
			mLineEdit->setAllowTextCodeChange(false);
			mLineEdit->update();
			break;
		case CodeOnly:
			mLineEdit->setCode(true);
			mLineEdit->setAllowTextCodeChange(false);
			mLineEdit->update();
			break;
		default:
			break;
		}

		addEditor(mLineEdit);
	}

	void TextParameterDefinition::load(const ActionInstance *actionInstance)
	{
		mLineEdit->setFromSubParameter(actionInstance->subParameter(name().original(), QStringLiteral("value")));
	}

	void TextParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), QStringLiteral("value"), mLineEdit->isCode(), mLineEdit->text());
	}
}
