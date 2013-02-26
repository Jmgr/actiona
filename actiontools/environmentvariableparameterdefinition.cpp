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

#include "environmentvariableparameterdefinition.h"
#include "codecombobox.h"
#include "script.h"
#include "subparameter.h"
#include "actioninstance.h"

#include <QProcessEnvironment>

namespace ActionTools
{
	void EnvironmentVariableParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mComboBox = new CodeComboBox(parent);

		mComboBox->addItems(script->procedureNames());

		addEditor(mComboBox);

		emit editorBuilt();
	}
/*
	void EnvironmentVariableParameterDefinition::load(const ActionInstance *actionInstance)
	{
		mWindowEdit->setFromSubParameter(actionInstance->subParameter(name().original(), "value"));
	}

	void EnvironmentVariableParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), "value", mWindowEdit->isCode(), mWindowEdit->text());
	}
*/
	void EnvironmentVariableParameterDefinition::update(Script *script)
	{
		Q_UNUSED(script)

		QStringList environmentVariables = QProcessEnvironment::systemEnvironment().keys();

		environmentVariables.sort();

		mComboBox->clear();
		mComboBox->addItems(environmentVariables);
	}
}
