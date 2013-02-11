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

#ifndef READENVIRONMENTINSTANCE_H
#define READENVIRONMENTINSTANCE_H

#include "actioninstance.h"
#include "script.h"

#include <QApplication>
#include <QProcessEnvironment>
#include <QStringList>

namespace Actions
{
	class ReadEnvironmentVariableInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Mode)

	public:
		enum Mode
		{
			Full,
			Selection
		};

		ReadEnvironmentVariableInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)												{}

		static ActionTools::StringListPair modes;

		void startExecution();

	private:
		Q_DISABLE_COPY(ReadEnvironmentVariableInstance)
	};
}

#endif // READENVIRONMENTINSTANCE_H
