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

		CursorPathInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent),
            mCurrentPoint(0),
            mButton(NoButton)
		{
            connect(&mMoveTimer, &QTimer::timeout, this, &CursorPathInstance::moveToNextPosition);
		}

        static Tools::StringListPair buttons;

		void startExecution() override
		{
			bool ok = true;

			mPositionOffset = evaluatePoint(ok, QStringLiteral("positionOffset"));
			mButton = evaluateListElement<Button>(ok, buttons, QStringLiteral("button"));
			mPoints = evaluatePolygon(ok, QStringLiteral("path"));

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

		void stopExecution() override
		{
            releaseButton();

			mMoveTimer.stop();
		}

		void stopLongTermExecution() override
		{
			mMouseDevice.reset();
		}

	private slots:
		void moveToNextPosition()
		{
			if(mCurrentPoint >= mPoints.size())
			{
                releaseButton();

				executionEnded();
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

