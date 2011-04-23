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

#ifndef TIMECONDITIONINSTANCE_H
#define TIMECONDITIONINSTANCE_H

#include "actioninstanceexecutionhelper.h"
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
			ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
			QString date;
			ActionTools::IfActionValue ifBefore;
			ActionTools::IfActionValue ifNow;
			ActionTools::IfActionValue ifAfter;

			if(!actionInstanceExecutionHelper.evaluateString(date, "date") ||
			   !actionInstanceExecutionHelper.evaluateIfAction(ifBefore, "ifBefore") ||
			   !actionInstanceExecutionHelper.evaluateIfAction(ifNow, "ifBefore") ||
			   !actionInstanceExecutionHelper.evaluateIfAction(ifAfter, "ifNow"))
				return;

			mTestedDateTime = QDateTime::fromString(date, "dd/MM/yyyy hh:mm:ss");
			if(!mTestedDateTime.isValid())
			{
				actionInstanceExecutionHelper.setCurrentParameter("date");
				emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid date"));
				return;
			}

			QString action;
			QString line;
			if(mTestedDateTime < QDateTime::currentDateTime())//Before
			{
				action = ifBefore.action();

				if(!actionInstanceExecutionHelper.evaluateSubParameter(line, ifBefore.actionParameter()))
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

				if(!actionInstanceExecutionHelper.evaluateSubParameter(line, ifAfter.actionParameter()))
					return;
			}
			else//Now
			{
				action = ifNow.action();

				if(!actionInstanceExecutionHelper.evaluateSubParameter(line, ifNow.actionParameter()))
					return;
			}

			if(action == ActionTools::IfActionValue::GOTO)
				actionInstanceExecutionHelper.setNextLine(line);

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
