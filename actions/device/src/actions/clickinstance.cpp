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

#include "clickinstance.hpp"
#include "actiontools/consolewidget.hpp"

#include <QPoint>
#include <QTimer>

namespace Actions
{
    Tools::StringListPair ClickInstance::buttons =
    {
        {
            QStringLiteral("left"),
            QStringLiteral("middle"),
            QStringLiteral("right")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("ClickInstance::buttons", "Left")),
            QStringLiteral(QT_TRANSLATE_NOOP("ClickInstance::buttons", "Middle")),
            QStringLiteral(QT_TRANSLATE_NOOP("ClickInstance::buttons", "Right"))
        }
    };
    Tools::StringListPair ClickInstance::actions =
    {
        {
            QStringLiteral("pressRelease"),
            QStringLiteral("press"),
            QStringLiteral("release")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("ClickInstance::actions", "Click (press and release)")),
            QStringLiteral(QT_TRANSLATE_NOOP("ClickInstance::actions", "Press")),
            QStringLiteral(QT_TRANSLATE_NOOP("ClickInstance::actions", "Release"))
        }
    };
	
	void ClickInstance::startExecution()
	{
		bool ok = true;
        bool isPositionEmpty = false;
	
		auto action = evaluateListElement<Action>(ok, actions, QStringLiteral("action"), QStringLiteral("value"));
		auto button = evaluateListElement<MouseDevice::Button>(ok, buttons, QStringLiteral("button"), QStringLiteral("value"));
		QPoint position = evaluatePoint(ok, QStringLiteral("position"), QStringLiteral("value"), &isPositionEmpty);
		QPoint positionOffset = evaluatePoint(ok, QStringLiteral("positionOffset"));
		int amount = evaluateInteger(ok, QStringLiteral("amount"));
		bool restoreCursorPosition = evaluateBoolean(ok, QStringLiteral("restoreCursorPosition"));
	
		if(!ok)
			return;
	
		if(action != ClickAction)
			amount = 1;
	
		if(amount <= 0)
		{
			setCurrentParameter(QStringLiteral("amount"));
			emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid click amount"));
			return;
		}

		QPoint previousPosition = mMouseDevice.cursorPosition();
		
        if(!isPositionEmpty)
        {
            position += positionOffset;
            mMouseDevice.setCursorPosition(position);
        }
	
		for(int i = 0; i < amount; ++i)
		{
			if(action == ClickAction || action == PressAction)
			{
				if(!mMouseDevice.pressButton(button))
				{
					emit executionException(FailedToSendInputException, tr("Unable to emulate click: button event failed"));
					return;
				}
			}
			if(action == ClickAction || action == ReleaseAction)
			{
				if(!mMouseDevice.releaseButton(button))
				{
					emit executionException(FailedToSendInputException, tr("Unable to emulate click: button event failed"));
					return;
				}
			}
		}

		if(!isPositionEmpty && restoreCursorPosition)
		{
			mMouseDevice.setCursorPosition(previousPosition);
		}
	
        QTimer::singleShot(1, this, [this]
        {
            executionEnded();
        });
	}

	void ClickInstance::stopLongTermExecution()
	{
		mMouseDevice.reset();
	}
}
