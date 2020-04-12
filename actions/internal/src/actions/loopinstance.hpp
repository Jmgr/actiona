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

namespace Actions
{
	class LoopInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		LoopInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent), mInitialized(false), mCounter(0)		{}

		void startExecution() override
		{
			if(mInitialized && mCounter == 0)
			{
				executionEnded();
				return;
			}

			bool ok = true;

			QString line = evaluateString(ok, QStringLiteral("line"));
			int count;

			if(!ok)
				return;

			if(!mInitialized)
			{
				mInitialized = true;

				count = evaluateInteger(ok, QStringLiteral("count"));

				if(!ok)
					return;

				if(count <= 0)
				{
					executionEnded();
					return;
				}

				mCounter = count;
			}

			setNextLine(line);

			--mCounter;

			executionEnded();
		}

		void reset() override
		{
			mInitialized = false;
			mCounter = 0;
		}

	private:
		bool mInitialized;
		int mCounter;

		Q_DISABLE_COPY(LoopInstance)
	};
}

