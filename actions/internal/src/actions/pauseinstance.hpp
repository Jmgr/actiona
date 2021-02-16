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

#include <QTimer>

namespace Actions
{
	class PauseInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		enum Unit
		{
			Milliseconds,
			Seconds,
			Minutes,
			Hours,
			Days
		};
        Q_ENUM(Unit)

		PauseInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent)
		{
            mTimer.setTimerType(Qt::PreciseTimer);
            mTimer.setSingleShot(true);

            connect(&mTimer, &QTimer::timeout, this, [this]{ executionEnded(); });
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

            switch(unit)
            {
            case Milliseconds:
                break;
            case Seconds:
                duration = duration * 1000;
                break;
            case Minutes:
                duration = duration * 1000 * 60;
                break;
            case Hours:
                duration = duration * 1000 * 60 * 60;
                break;
            case Days:
                duration = duration * 1000 * 60 * 60 * 24;
                break;
            default:
                break;
            }

            mTimer.start(duration);
		}

        void pauseExecution() override
        {
            mRemainingTime = mTimer.remainingTime();
            mTimer.stop();
        }

        void resumeExecution() override
        {
            mTimer.start(mRemainingTime);
        }

		void stopExecution() override
		{
            mTimer.stop();
		}

    private:
        QTimer mTimer;
        int mRemainingTime{};

		Q_DISABLE_COPY(PauseInstance)
	};
}

