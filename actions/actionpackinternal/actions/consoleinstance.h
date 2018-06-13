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

#pragma once

#include "actioninstance.h"

#include <QDebug>

namespace Actions
{
	class ConsoleInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Output)

	public:
		enum Output
		{
			Information,
			Warning,
			Error
		};

		ConsoleInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent)										{}

        static Tools::StringListPair outputs;

		void startExecution() override
		{
			bool ok = true;

			QString text = evaluateString(ok, QStringLiteral("text"));
			auto output = evaluateListElement<Output>(ok, outputs, QStringLiteral("output"));

			if(!ok)
				return;

			switch(output)
			{
			case Information:
				emit consolePrint(text);
				break;
			case Warning:
				emit consolePrintWarning(text);
				break;
			case Error:
				emit consolePrintError(text);
				break;
			}

			executionEnded();
		}

	private:
		Q_DISABLE_COPY(ConsoleInstance)
	};
}

