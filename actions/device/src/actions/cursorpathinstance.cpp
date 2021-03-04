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
#include "backend/mouse.hpp"

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
        auto &mouse = Backend::Instance::mouse();

        bool ok = true;

        mPositionOffset = evaluatePoint(ok, QStringLiteral("positionOffset"));
        mButton = evaluateListElement<Button>(ok, buttons, QStringLiteral("button"));
        mPoints = evaluatePolygon(ok, QStringLiteral("path"));

        if(!ok)
            return;

        try
        {
            mMoveTimer.start(25);

            mCurrentPoint = 0;
            mouse.setCursorPosition(mPoints.at(mCurrentPoint) + mPositionOffset);
            ++mCurrentPoint;

            switch(mButton)
            {
            case NoButton:
                break;
            case LeftButton:
                mouse.pressButton(Backend::Mouse::Button::Left, true);
                break;
            case MiddleButton:
                mouse.pressButton(Backend::Mouse::Button::Middle, true);
                break;
            case RightButton:
                mouse.pressButton(Backend::Mouse::Button::Right, true);
                break;
            }
        }
        catch(const Backend::BackendError &e)
        {
            emit executionException(FailedToSendInputException, e.what());
            return;
        }
    }

    void CursorPathInstance::stopExecution()
    {
        releaseButton();

        mMoveTimer.stop();
    }

    void CursorPathInstance::moveToNextPosition()
    {
        auto &mouse = Backend::Instance::mouse();

        if(mCurrentPoint >= mPoints.size())
        {
            releaseButton();

            executionEnded();
            mMoveTimer.stop();
        }
        else
        {
            try
            {
                mouse.setCursorPosition(mPoints.at(mCurrentPoint) + mPositionOffset);
                ++mCurrentPoint;
            }
            catch(const Backend::BackendError &e)
            {
                emit executionException(FailedToSendInputException, e.what());
                return;
            }
        }
    }

    void CursorPathInstance::releaseButton()
    {
        auto &mouse = Backend::Instance::mouse();

        try
        {
            switch(mButton)
            {
            case NoButton:
                break;
            case LeftButton:
                mouse.pressButton(Backend::Mouse::Button::Left, false);
                break;
            case MiddleButton:
                mouse.pressButton(Backend::Mouse::Button::Middle, false);
                break;
            case RightButton:
                mouse.pressButton(Backend::Mouse::Button::Right, false);
                break;
            }
        }
        catch(const Backend::BackendError &e)
        {
            emit executionException(FailedToSendInputException, e.what());
            return;
        }
    }
}

