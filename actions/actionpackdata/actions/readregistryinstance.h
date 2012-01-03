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

#ifndef READREGISTRYINSTANCE_H
#define READREGISTRYINSTANCE_H

#include "actioninstance.h"
#include "stringlistpair.h"
#include "registry.h"

namespace Actions
{
	class ReadRegistryInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(ActionTools::Registry::Key)
		Q_ENUMS(ActionTools::Registry::ReadResult)

	public:
		enum Exceptions
		{
			CannotFindSubKeyException = ActionTools::ActionException::UserException,
			CannotFindValueException,
			InvalidValueType
		};

		ReadRegistryInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)												{}

		static ActionTools::StringListPair keys;

		void startExecution();

	private:
		Q_DISABLE_COPY(ReadRegistryInstance)
	};
}

#endif // READREGISTRYINSTANCE_H
