/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef TIMECONDITIONINSTANCE_H
#define TIMECONDITIONINSTANCE_H

#include "actioninstance.h"
#include "ifactionvalue.h"

#include <QDateTime>
#include <QTimer>

namespace Actions
{
	class TimeConditionInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		TimeConditionInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)										{}

		void startExecution()
		{
			bool ok = true;

            mTestedDateTime = evaluateDateTime(ok, "date");

            if(!mTestedDateTime.isValid())
            {
                setCurrentParameter("date");
                emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid date"));
                return;
            }

			ActionTools::IfActionValue ifBefore = evaluateIfAction(ok, "ifBefore");
			ActionTools::IfActionValue ifNow = evaluateIfAction(ok, "ifNow");
			ActionTools::IfActionValue ifAfter = evaluateIfAction(ok, "ifAfter");

			if(!ok)
				return;

			QString action;
			QString line;
			if(mTestedDateTime < QDateTime::currentDateTime())//Before
			{
				action = ifBefore.action();
				line = evaluateSubParameter(ok, ifBefore.actionParameter());

				if(!ok)
					return;
			}
			else if(mTestedDateTime > QDateTime::currentDateTime())//After
			{
				if(ifAfter.action() == ActionTools::IfActionValue::WAIT)
				{
					connect(&mTimer, SIGNAL(timeout()), this, SLOT(checkDateTime()));
					mTimer.setInterval(1000);
					mTimer.start();

					return;
				}

				action = ifAfter.action();
				line = evaluateSubParameter(ok, ifAfter.actionParameter());

				if(!ok)
					return;
			}
			else//Now
			{
				action = ifNow.action();
				line = evaluateSubParameter(ok, ifNow.actionParameter());

				if(!ok)
					return;
			}

			if(action == ActionTools::IfActionValue::GOTO)
				setNextLine(line);
			else if(action == ActionTools::IfActionValue::CALLPROCEDURE)
			{
				if(!callProcedure(line))
					return;
			}

			emit executionEnded();
		}

		void stopExecution()
		{
			mTimer.stop();
		}

	private slots:
		void checkDateTime()
		{
			if(mTestedDateTime <= QDateTime::currentDateTime())
			{
				mTimer.stop();
				emit executionEnded();
			}
		}

	private:
		QTimer mTimer;
		QDateTime mTestedDateTime;

		Q_DISABLE_COPY(TimeConditionInstance)
	};
}

#endif // TIMECONDITIONINSTANCE_H
