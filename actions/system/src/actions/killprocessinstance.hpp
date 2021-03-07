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

	Contact: jmgr@jmgr.info
*/

#pragma once

#include "actiontools/actioninstance.hpp"
#include "actiontools/script.hpp"
#include "backend/process.hpp"

namespace Actions
{
	class KillProcessInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		enum KillMode
		{
            Graceful = static_cast<int>(Backend::Process::KillMode::Graceful),
            Forceful = static_cast<int>(Backend::Process::KillMode::Forceful),
            GracefulThenForceful = static_cast<int>(Backend::Process::KillMode::GracefulThenForceful)
		};
        Q_ENUM(KillMode)
        enum Exceptions
        {
            FailedToKillProcessException = ActionTools::ActionException::UserException
        };

		KillProcessInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent)												{}

        static Tools::StringListPair killModes;

		void startExecution() override
		{
			bool ok = true;

			int processId = evaluateInteger(ok, QStringLiteral("processId"));

			if(!ok)
				return;

			if(processId != 0)
            {
                try
                {
                    Backend::Instance::process().killProcess(processId, Backend::Process::KillMode::GracefulThenForceful, 100);
                }
                catch(const Backend::BackendError &e)
                {
                    emit executionException(FailedToKillProcessException, e.what());
                    return;
                }
            }

			executionEnded();
		}

	private:
		Q_DISABLE_COPY(KillProcessInstance)
	};
}

