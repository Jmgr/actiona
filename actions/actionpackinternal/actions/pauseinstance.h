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

		PauseInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent)
		{
            connect(&mCheckTimer, &QTimer::timeout, this, &PauseInstance::checkTime);
		}

        static Tools::StringListPair units;

		void startExecution() override
		{
			bool ok = true;

			int duration = evaluateInteger(ok, QStringLiteral("duration"));
			Unit unit = evaluateListElement<Unit>(ok, units, QStringLiteral("unit"));

			if(!ok)
				return;

			if(duration < 0)
			{
				emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid pause duration"));
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

		void stopExecution() override
		{
			mCheckTimer.stop();
		}

	private slots:
		void checkTime()
		{
			if(QDateTime::currentDateTime() >= mEndTime)
			{
				mCheckTimer.stop();

				executionEnded();
			}
		}

	private:
		QDateTime mEndTime;
		QTimer mCheckTimer;

		Q_DISABLE_COPY(PauseInstance)
	};
}

