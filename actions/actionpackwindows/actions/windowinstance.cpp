/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#include "actions/windowinstance.h"
#include "actioninstanceexecutionhelper.h"
#include "windowhandle.h"

#include <QxtWindowSystem>
#include <QRegExp>

namespace Actions
{
	ActionTools::StringListPair WindowInstance::actions = qMakePair(
			QStringList() << "close" << "killProcess" << "setForeground" << "minimize" << "maximize" << "move" << "resize",
			QStringList() << QObject::tr("Close") << QObject::tr("Kill process") << QObject::tr("Set foreground") << QObject::tr("Minimize") << QObject::tr("Maximize") << QObject::tr("Move") << QObject::tr("Resize"));

	WindowInstance::WindowInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent)
	{
	}

	void WindowInstance::startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());

		QString title;
		Action action;
		QPoint movePosition;
		int resizeWidth;
		int resizeHeight;

		if(!actionInstanceExecutionHelper.evaluateString(title, "title") ||
			!actionInstanceExecutionHelper.evaluateListElement(action, actions, "action"))
			return;

		if(action == Move)
		{
			if(!actionInstanceExecutionHelper.evaluatePoint(movePosition, "movePosition"))
				return;
		}
		else if(action == Resize)
		{
			if(!actionInstanceExecutionHelper.evaluateInteger(resizeWidth, "resizeWidth") ||
				!actionInstanceExecutionHelper.evaluateInteger(resizeHeight, "resizeHeight"))
				return;
		}

		QRegExp titleRegExp(title, Qt::CaseSensitive, QRegExp::WildcardUnix);
		ActionTools::WindowHandle foundWindow;

		foreach(const WId &windowId, QxtWindowSystem::windows())
		{
			ActionTools::WindowHandle windowHandle(windowId);
			if(titleRegExp.exactMatch(windowHandle.title()))
			{
				foundWindow = windowHandle;
				break;
			}
		}

		if(!foundWindow.isValid())
		{
			actionInstanceExecutionHelper.setCurrentParameter("title");
			emit executionException(CannotFindWindowException, tr("Cannot find any window matching \"%1\"").arg(title));
			return;
		}

		bool result = true;

		switch(action)
		{
		case Close:
			result = foundWindow.close();
			break;
		case KillProcess:
			result = foundWindow.killCreator();
			break;
		case SetForeground:
			result = foundWindow.setForeground();
			break;
		case Minimize:
			result = foundWindow.minimize();
			break;
		case Maximize:
			result = foundWindow.maximize();
			break;
		case Move:
			result = foundWindow.move(movePosition);
			break;
		case Resize:
			result = foundWindow.resize(QSize(resizeWidth, resizeHeight));
			break;
		}

		if(!result)
		{
			actionInstanceExecutionHelper.setCurrentParameter("action");
			emit executionException(ActionFailedException, tr("\"%1\" failed").arg(actions.second[action]));
			return;
		}

		emit executionEnded();
	}
}
