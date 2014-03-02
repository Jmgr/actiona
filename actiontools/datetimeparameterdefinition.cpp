/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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
#include "actioninstance.h"

namespace ActionTools
{
    DateTimeParameterDefinition::DateTimeParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
		mDateTimeEdit(0)
	{
	}

	void DateTimeParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mDateTimeEdit = new CodeDateTimeEdit(parent);

		addEditor(mDateTimeEdit);
	}

	void DateTimeParameterDefinition::load(const ActionInstance *actionInstance)
	{
		mDateTimeEdit->setFromSubParameter(actionInstance->subParameter(name().original(), "value"));
	}

	void DateTimeParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), "value", mDateTimeEdit->isCode(), mDateTimeEdit->text());
	}

    void DateTimeParameterDefinition::setDefaultValues(ActionInstance *actionInstance)
    {
        actionInstance->setSubParameter(name().original(), "value", QDateTime::currentDateTime());
    }
}
