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

#include "windowparameterdefinition.h"
#include "subparameter.h"
#include "windowedit.h"
#include "actioninstance.h"
#include "windowhandle.h"

namespace ActionTools
{
    WindowParameterDefinition::WindowParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
		mWindowEdit(nullptr)
	{
	}

	void WindowParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mWindowEdit = new WindowEdit(parent);

		addEditor(mWindowEdit);
	}

	void WindowParameterDefinition::load(const ActionInstance *actionInstance)
	{
		mWindowEdit->setFromSubParameter(actionInstance->subParameter(name().original(), QStringLiteral("value")));
	}

	void WindowParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), QStringLiteral("value"), mWindowEdit->isCode(), mWindowEdit->text());
	}

    void WindowParameterDefinition::actionUpdate(Script *script)
	{
		Q_UNUSED(script)

        mWindowEdit->setWindowTitles(ActionTools::WindowHandle::windowTitles());
	}
}
