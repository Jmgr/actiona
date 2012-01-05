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

#ifndef CURSORPATHINSTANCE_H
#define CURSORPATHINSTANCE_H

#include "actioninstance.h"
#include "../mousedevice.h"

#include <QTimer>
#include <QPolygon>

namespace Actions
{
	class CursorPathInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		CursorPathInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent),
			mCurrentPoint(0)
		{
			connect(&mMoveTimer, SIGNAL(timeout()), this, SLOT(moveToNextPosition()));
		}

		void startExecution()
		{
			bool ok = true;

			mPoints = evaluatePolygon(ok, "path");

			if(!ok)
				return;

			mCurrentPoint = 0;
			mMoveTimer.start(25);
		}

		void stopExecution()
		{
			mMoveTimer.stop();
		}

		void stopLongTermExecution()
		{
			mMouseDevice.reset();
		}

	private slots:
		void moveToNextPosition()
		{
			if(mCurrentPoint >= mPoints.size())
			{
				emit executionEnded();
				mMoveTimer.stop();
			}
			else
			{
				mMouseDevice.setCursorPosition(mPoints.at(mCurrentPoint));
				++mCurrentPoint;
			}
		}

	private:
		MouseDevice mMouseDevice;
		QTimer mMoveTimer;
		QPolygon mPoints;
		int mCurrentPoint;

		Q_DISABLE_COPY(CursorPathInstance)
	};
}

#endif // CURSORPATHINSTANCE_H
