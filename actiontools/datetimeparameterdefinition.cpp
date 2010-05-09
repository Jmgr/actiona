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

#include "datetimeparameterdefinition.h"
#include "subparameter.h"
#include "codedatetimeedit.h"
#include "action.h"

namespace ActionTools
{
	DateTimeParameterDefinition::DateTimeParameterDefinition(Category category, const QString &name, const QString &translatedName, QObject *parent)
		: ParameterDefinition(category, name, translatedName, parent),
		mDateTimeEdit(0)
	{
	}

	void DateTimeParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mDateTimeEdit = new CodeDateTimeEdit(parent);
		mDateTimeEdit->setObjectName("value");

		addEditor(mDateTimeEdit);
	}

	void DateTimeParameterDefinition::load(const Action *action)
	{
		mDateTimeEdit->setFromSubParameter(action->subParameter(name(), "value"));
	}

	void DateTimeParameterDefinition::save(Action *action)
	{
		action->setSubParameter(name(), "value", mDateTimeEdit->isCode(), mDateTimeEdit->text());
	}

	void DateTimeParameterDefinition::setDefaultValues(Parameter &data)
	{
		data.subParameters()["value"].setValue(option("default"));
	}
}
