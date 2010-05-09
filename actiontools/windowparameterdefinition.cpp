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

#include "windowparameterdefinition.h"
#include "subparameter.h"
#include "windowedit.h"
#include "action.h"

namespace ActionTools
{
	WindowParameterDefinition::WindowParameterDefinition(Category category, const QString &name, const QString &translatedName, QObject *parent)
		: ParameterDefinition(category, name, translatedName, parent),
		mWindowEdit(0)
	{
	}

	void WindowParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mWindowEdit = new WindowEdit(parent);
		mWindowEdit->setObjectName("value");

		addEditor(mWindowEdit);
	}

	void WindowParameterDefinition::load(const Action *action)
	{
		mWindowEdit->setFromSubParameter(action->subParameter(name(), "value"));
	}

	void WindowParameterDefinition::save(Action *action)
	{
		action->setSubParameter(name(), "value", mWindowEdit->isCode(), mWindowEdit->text());
	}

	void WindowParameterDefinition::setDefaultValues(Parameter &data)
	{
		data.subParameters()["value"].setValue(option("default"));
	}
}
