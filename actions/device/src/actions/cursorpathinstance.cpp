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

#include "cursorpathinstance.hpp"
#include "backend/mouse-output.hpp"

namespace Actions
{
    Tools::StringListPair CursorPathInstance::buttons =
    {
        {
            QStringLiteral("none"),
            QStringLiteral("left"),
            QStringLiteral("middle"),
            QStringLiteral("right")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("CursorPathInstance::buttons", "None")),
            QStringLiteral(QT_TRANSLATE_NOOP("CursorPathInstance::buttons", "Left")),
            QStringLiteral(QT_TRANSLATE_NOOP("CursorPathInstance::buttons", "Middle")),
            QStringLiteral(QT_TRANSLATE_NOOP("CursorPathInstance::buttons", "Right"))
        }
    };

    CursorPathInstance::CursorPathInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
        : ActionTools::ActionInstance(definition, parent),
        mCurrentPoint(0),
        mButton(NoButton)
    {
        mMoveTimer.setTimerType(Qt::PreciseTimer);

        connect(&mMoveTimer, &QTimer::timeout, this, &CursorPathInstance::moveToNextPosition);
    }

    void CursorPathInstance::startExecution()
    {
        auto &mouseOutput = Backend::Backend::instance().mouseOutput();

        bool ok = true;

        mPositionOffset = evaluatePoint(ok, QStringLiteral("positionOffset"));
        mButton = evaluateListElement<Button>(ok, buttons, QStringLiteral("button"));
        mPoints = evaluatePolygon(ok, QStringLiteral("path"));

        if(!ok)
            return;

        mMoveTimer.start(25);

        mCurrentPoint = 0;
        mouseOutput.setCursorPosition(mPoints.at(mCurrentPoint) + mPositionOffset);
        ++mCurrentPoint;

        switch(mButton)
        {
        case NoButton:
            break;
        case LeftButton:
            mouseOutput.pressButton(Backend::Mouse::LeftButton);
            break;
        case MiddleButton:
            mouseOutput.pressButton(Backend::Mouse::MiddleButton);
            break;
        case RightButton:
            mouseOutput.pressButton(Backend::Mouse::RightButton);
            break;
        }
    }

    void CursorPathInstance::stopExecution()
    {
        releaseButton();

        mMoveTimer.stop();
    }

    void CursorPathInstance::moveToNextPosition()
    {
        auto &mouseOutput = Backend::Backend::instance().mouseOutput();

        if(mCurrentPoint >= mPoints.size())
        {
            releaseButton();

            executionEnded();
            mMoveTimer.stop();
        }
        else
        {
            mouseOutput.setCursorPosition(mPoints.at(mCurrentPoint) + mPositionOffset);
            ++mCurrentPoint;
        }
    }

    void CursorPathInstance::releaseButton()
    {
        auto &mouseOutput = Backend::Backend::instance().mouseOutput();

        switch(mButton)
        {
        case NoButton:
            break;
        case LeftButton:
            mouseOutput.releaseButton(Backend::Mouse::LeftButton);
            break;
        case MiddleButton:
            mouseOutput.releaseButton(Backend::Mouse::MiddleButton);
            break;
        case RightButton:
            mouseOutput.releaseButton(Backend::Mouse::RightButton);
            break;
        }
    }
}

