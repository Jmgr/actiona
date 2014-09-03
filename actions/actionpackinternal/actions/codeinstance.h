/*
	Actiona
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef CODEINSTANCE_H
#define CODEINSTANCE_H

#include "actioninstance.h"

namespace Actions
{
	class CodeInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		CodeInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)										{}

		void startExecution()
		{
			bool ok = true;

			evaluateString(ok, "code");

			if(!ok)
				return;

			emit executionEnded();
		}

	private:
		Q_DISABLE_COPY(CodeInstance)
	};
}

#endif // CODEINSTANCE_H
