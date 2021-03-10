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

#include "windowinstance.hpp"
#include "actiontools/windowhandle.hpp"
#include "backend/windowing.hpp"

#include <QRegExp>

namespace Actions
{
    Tools::StringListPair WindowInstance::actions =
    {
        {
            QStringLiteral("close"),
            QStringLiteral("killProcess"),
            QStringLiteral("setForeground"),
            QStringLiteral("minimize"),
            QStringLiteral("maximize"),
            QStringLiteral("move"),
            QStringLiteral("resize")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("WindowInstance::actions", "Close")),
            QStringLiteral(QT_TRANSLATE_NOOP("WindowInstance::actions", "Kill process")),
            QStringLiteral(QT_TRANSLATE_NOOP("WindowInstance::actions", "Set foreground")),
            QStringLiteral(QT_TRANSLATE_NOOP("WindowInstance::actions", "Minimize")),
            QStringLiteral(QT_TRANSLATE_NOOP("WindowInstance::actions", "Maximize")),
            QStringLiteral(QT_TRANSLATE_NOOP("WindowInstance::actions", "Move")),
            QStringLiteral(QT_TRANSLATE_NOOP("WindowInstance::actions", "Resize"))
        }
    };

	WindowInstance::WindowInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent)
	{
	}

	void WindowInstance::startExecution()
	{
		bool ok = true;

		QString title = evaluateString(ok, QStringLiteral("title"));
		auto action = evaluateListElement<Action>(ok, actions, QStringLiteral("action"));
		bool useBorders = evaluateBoolean(ok, QStringLiteral("useBorders"));

		if(!ok)
			return;

		QPoint movePosition;
		int resizeWidth = 0;
		int resizeHeight = 0;

		if(action == Move)
		{
			movePosition = evaluatePoint(ok, QStringLiteral("movePosition"));
			if(!ok)
				return;
		}
		else if(action == Resize)
		{
			resizeWidth = evaluateInteger(ok, QStringLiteral("resizeWidth"));
			resizeHeight = evaluateInteger(ok, QStringLiteral("resizeHeight"));
			if(!ok)
				return;
		}

        ActionTools::WindowHandle foundWindow;

        try
        {
            foundWindow = ActionTools::WindowHandle::findWindow(QRegExp(title, Qt::CaseSensitive, QRegExp::WildcardUnix));
        }
        catch(const Backend::BackendError &)
        {
            // ignore errors
        }

		if(!foundWindow.isValid())
		{
			setCurrentParameter(QStringLiteral("title"));
			emit executionException(CannotFindWindowException, tr("Cannot find any window matching \"%1\"").arg(title));
			return;
		}

        try
        {
            switch(action)
            {
            case Close:
                foundWindow.close();
                break;
            case KillProcess:
                foundWindow.killCreator();
                break;
            case SetForeground:
                foundWindow.setForeground();
                break;
            case Minimize:
                foundWindow.minimize();
                break;
            case Maximize:
                foundWindow.maximize();
                break;
            case Move:
                foundWindow.move(movePosition);
                break;
            case Resize:
                foundWindow.resize(QSize(resizeWidth, resizeHeight), useBorders);
                break;
            }
        }
        catch(const Backend::BackendError &e)
        {
            setCurrentParameter(QStringLiteral("action"));
            emit executionException(ActionFailedException, tr("\"%1\" failed: %2").arg(actions.second[action]).arg(e.what()));
            return;
        }

		executionEnded();
	}
}
