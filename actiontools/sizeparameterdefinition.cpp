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

#include "sizeparameterdefinition.h"
//TODO move to sizeedit.h
#include "positionedit.h"
#include "actioninstance.h"

namespace ActionTools
{
	SizeParameterDefinition::SizeParameterDefinition(const Name &name, QObject *parent)
		: ParameterDefinition(name, parent),
		//TODO move to mSizeEdit
		mPositionEdit(0)
	{
	}

	void SizeParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		//TODO move to SizeEdit
		mPositionEdit = new PositionEdit(parent);

		addEditor(mPositionEdit);
	}

	void SizeParameterDefinition::load(const ActionInstance *actionInstance)
	{
		mPositionEdit->setFromSubParameter(actionInstance->subParameter(name().original(), "value"));
	}

	void SizeParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), "value", mPositionEdit->isCode(), mPositionEdit->text());
	}
}
