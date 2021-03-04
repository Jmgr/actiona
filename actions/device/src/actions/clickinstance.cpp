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
#include "backend/mouse.hpp"

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

    ClickInstance::ClickInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
        : ActionTools::ActionInstance(definition, parent)
    {
    }
	
	void ClickInstance::startExecution()
	{
        auto &mouse = Backend::Instance::mouse();

		bool ok = true;
        bool isPositionEmpty = false;
	
		auto action = evaluateListElement<Action>(ok, actions, QStringLiteral("action"), QStringLiteral("value"));
        auto button = evaluateListElement<Backend::Mouse::Button>(ok, buttons, QStringLiteral("button"), QStringLiteral("value"));
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

        try
        {
            QPoint previousPosition = mouse.cursorPosition();

            if(!isPositionEmpty)
            {
                position += positionOffset;
                mouse.setCursorPosition(position);
            }

            for(int i = 0; i < amount; ++i)
            {
                bool press = (action == ClickAction || action == PressAction);
                bool release = (action == ClickAction || action == ReleaseAction);
                if(press)
                    mouse.pressButton(button, true);
                if(release)
                    mouse.pressButton(button, false);
            }

            if(!isPositionEmpty && restoreCursorPosition)
                mouse.setCursorPosition(previousPosition);
        }
        catch(const Backend::BackendError &e)
        {
            emit executionException(FailedToSendInputException, e.what());
            return;
        }
	
        QTimer::singleShot(1, this, [this]
        {
            executionEnded();
        });
	}
}
