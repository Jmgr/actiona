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

#include "textparameterdefinition.h"
#include "subparameter.h"
#include "codelineedit.h"
#include "action.h"

#include <QDebug>

namespace ActionTools
{
	TextParameterDefinition::TextParameterDefinition(Category category, const QString &name, const QString &translatedName, QObject *parent)
		: ParameterDefinition(category, name, translatedName, parent),
		mCodeOnly(false),
		mLineEdit(0)
	{
	}

	void TextParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mLineEdit = new CodeLineEdit(parent);
		mLineEdit->setObjectName("value");

		if(mCodeOnly)
		{
			mLineEdit->setCode(true);
			mLineEdit->setAllowTextCodeChange(false);
			mLineEdit->update();
		}

		addEditor(mLineEdit);
	}

	void TextParameterDefinition::load(const Action *action)
	{
		mLineEdit->setFromSubParameter(action->subParameter(name(), "value"));
	}

	void TextParameterDefinition::save(Action *action)
	{
		action->setSubParameter(name(), "value", mLineEdit->isCode(), mLineEdit->text());
	}

	void TextParameterDefinition::setDefaultValues(Parameter &data)
	{
		data.subParameters()["value"].setValue(option("default"));
	}
}
