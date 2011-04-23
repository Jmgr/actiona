/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#ifndef LOOPINSTANCE_H
#define LOOPINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "actioninstance.h"

namespace Actions
{
	class LoopInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		LoopInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent), mInitialized(false), mCounter(0)		{}

		void startExecution()
		{
			if(mInitialized && mCounter == 0)
			{
				emit executionEnded();
				return;
			}

			ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
			QString line;
			int count;

			if(!actionInstanceExecutionHelper.evaluateString(line, "line"))
				return;

			if(!mInitialized)
			{
				mInitialized = true;

				if(!actionInstanceExecutionHelper.evaluateInteger(count, "count"))
					return;

				if(count <= 0)
				{
					emit executionEnded();
					return;
				}

				mCounter = count;
			}

			actionInstanceExecutionHelper.setNextLine(line);

			--mCounter;

			emit executionEnded();
		}

		void reset()
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

#endif // LOOPINSTANCE_H
