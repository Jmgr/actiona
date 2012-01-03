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

#ifndef PAUSEINSTANCE_H
#define PAUSEINSTANCE_H

#include "actioninstance.h"

#include <QTimer>
#include <QDateTime>

namespace Actions
{
	class PauseInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Unit)

	public:
		enum Unit
		{
			Milliseconds,
			Seconds,
			Minutes,
			Hours,
			Days
		};

		PauseInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)
		{
			connect(&mCheckTimer, SIGNAL(timeout()), this, SLOT(checkTime()));
		}

		static ActionTools::StringListPair units;

		void startExecution()
		{
			bool ok = true;

			int duration = evaluateInteger(ok, "duration");
			Unit unit = evaluateListElement<Unit>(ok, units, "unit");

			if(!ok)
				return;

			if(duration < 0)
			{
				emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid pause duration"));
				return;
			}

			mEndTime = QDateTime::currentDateTime();

			if(unit == Milliseconds)
				mCheckTimer.start(duration);
			else
			{
				switch(unit)
				{
				case Seconds:
					mEndTime = mEndTime.addSecs(duration);
					break;
				case Minutes:
					mEndTime = mEndTime.addMSecs(duration * 60000);
					break;
				case Hours:
					mEndTime = mEndTime.addSecs(duration * 3600000);
					break;
				case Days:
					mEndTime = mEndTime.addDays(duration);
					break;
				default:
					break;
				}

				mCheckTimer.start(1000);
			}
		}

		void stopExecution()
		{
			mCheckTimer.stop();
		}

	private slots:
		void checkTime()
		{
			if(QDateTime::currentDateTime() >= mEndTime)
			{
				mCheckTimer.stop();

				emit executionEnded();
			}
		}

	private:
		QDateTime mEndTime;
		QTimer mCheckTimer;

		Q_DISABLE_COPY(PauseInstance)
	};
}

#endif // PAUSEINSTANCE_H
