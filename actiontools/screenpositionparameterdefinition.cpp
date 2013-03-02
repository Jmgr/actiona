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

#include "screenpositionparameterdefinition.h"
#include "booleanedit.h"
#include "actioninstance.h"

namespace ActionTools
{
	ScreenPositionParameterDefinition::ScreenPositionParameterDefinition(const Name &name, QObject *parent)
		: ParameterDefinition(name, parent)
	{
	}

	void ScreenPositionParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mBooleanEdit = new BooleanEdit(parent);

		addEditor(mBooleanEdit);

		mWindowPositionEdit = new WindowPositionEdit(parent);

		addEditor(mWindowPositionEdit);

	}

	void ScreenPositionParameterDefinition::load(const ActionInstance *actionInstance)
	{

		mBooleanEdit->setFromSubParameter(actionInstance->subParameter(name().original(), "visible"));
		mWindowPositionEdit->setFromSubParameter(actionInstance->subParameter(name().original(), "windowposition"));

	}

	void ScreenPositionParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), "visible", mBooleanEdit->isCode(), mBooleanEdit->text());
		actionInstance->setSubParameter(name().original(), "windowposition", mWindowPositionEdit->isCode(), mWindowPositionEdit->text());
	}

	void ScreenPositionParameterDefinition::setDefaultValues(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), "visible", QString(defaultWindowVisible()));
		actionInstance->setSubParameter(name().original(), "windowposition", defaultScreenPosition());
	}
}

