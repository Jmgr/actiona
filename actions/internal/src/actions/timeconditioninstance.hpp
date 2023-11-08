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
#include "actiontools/ifactionvalue.hpp"

#include <QDateTime>
#include <QTimer>

namespace Actions
{
	class TimeConditionInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		TimeConditionInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
            : ActionTools::ActionInstance(definition, parent)
        {
            mTimer.setTimerType(Qt::PreciseTimer);
        }

		void startExecution() override
		{
			bool ok = true;

			mTestedDateTime = evaluateDateTime(ok, QStringLiteral("date"));

            if(!mTestedDateTime.isValid())
            {
				setCurrentParameter(QStringLiteral("date"));
                emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid date"));
                return;
            }

			ActionTools::IfActionValue ifBefore = evaluateIfAction(ok, QStringLiteral("ifBefore"));
			ActionTools::IfActionValue ifNow = evaluateIfAction(ok, QStringLiteral("ifNow"));
			ActionTools::IfActionValue ifAfter = evaluateIfAction(ok, QStringLiteral("ifAfter"));

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
                    connect(&mTimer, &QTimer::timeout, this, &TimeConditionInstance::checkDateTime);
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

            bool stopScript = false;
			if(action == ActionTools::IfActionValue::GOTO)
				setNextLine(line);
			else if(action == ActionTools::IfActionValue::CALLPROCEDURE)
			{
				if(!callProcedure(line))
					return;
            }
            else if(action == ActionTools::IfActionValue::STOPEXECUTION)
            {
                stopScript = true;
            }

            executionEnded(stopScript);
		}

		void stopExecution() override
		{
			mTimer.stop();
		}

	private slots:
		void checkDateTime()
		{
			if(mTestedDateTime <= QDateTime::currentDateTime())
			{
				mTimer.stop();
				executionEnded();
			}
		}

	private:
		QTimer mTimer;
		QDateTime mTestedDateTime;

		Q_DISABLE_COPY(TimeConditionInstance)
	};
}

