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
        enum Button
        {
            NoButton,
            LeftButton,
            MiddleButton,
            RightButton
        };

		CursorPathInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent),
            mCurrentPoint(0),
            mButton(NoButton)
		{
			connect(&mMoveTimer, SIGNAL(timeout()), this, SLOT(moveToNextPosition()));
		}

        static ActionTools::StringListPair buttons;

		void startExecution()
		{
			bool ok = true;

			mPositionOffset = evaluatePoint(ok, "positionOffset");
            mButton = evaluateListElement<Button>(ok, buttons, "button");
			mPoints = evaluatePolygon(ok, "path");

			if(!ok)
				return;

			mMoveTimer.start(25);

            mCurrentPoint = 0;
            mMouseDevice.setCursorPosition(mPoints.at(mCurrentPoint) + mPositionOffset);
            ++mCurrentPoint;

            switch(mButton)
            {
            case NoButton:
                break;
            case LeftButton:
                mMouseDevice.pressButton(MouseDevice::LeftButton);
                break;
            case MiddleButton:
                mMouseDevice.pressButton(MouseDevice::MiddleButton);
                break;
            case RightButton:
                mMouseDevice.pressButton(MouseDevice::RightButton);
                break;
            }
		}

		void stopExecution()
		{
            releaseButton();

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
                releaseButton();

				emit executionEnded();
				mMoveTimer.stop();
			}
			else
			{
				mMouseDevice.setCursorPosition(mPoints.at(mCurrentPoint) + mPositionOffset);
				++mCurrentPoint;
			}
		}

	private:
        void releaseButton()
        {
            switch(mButton)
            {
            case NoButton:
                break;
            case LeftButton:
                mMouseDevice.releaseButton(MouseDevice::LeftButton);
                break;
            case MiddleButton:
                mMouseDevice.releaseButton(MouseDevice::MiddleButton);
                break;
            case RightButton:
                mMouseDevice.releaseButton(MouseDevice::RightButton);
                break;
            }
        }

		MouseDevice mMouseDevice;
		QTimer mMoveTimer;
		QPoint mPositionOffset;
		QPolygon mPoints;
		int mCurrentPoint;
        Button mButton;

		Q_DISABLE_COPY(CursorPathInstance)
	};
}

#endif // CURSORPATHINSTANCE_H
