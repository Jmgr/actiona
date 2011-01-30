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

#include "windowconditioninstance.h"
#include "actioninstanceexecutionhelper.h"

#include <QRegExp>

namespace Actions
{
	ActionTools::StringListPair WindowConditionInstance::conditions = qMakePair(
			QStringList() << "exists" << "dontexists",
			QStringList()
			<< QT_TRANSLATE_NOOP("WindowConditionInstance::conditions", "Exists")
			<< QT_TRANSLATE_NOOP("WindowConditionInstance::conditions", "Don't exists"));

	WindowConditionInstance::WindowConditionInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent), mCondition(Exists)
	{
	}

	void WindowConditionInstance::startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
		QString title;
		ActionTools::IfActionValue ifFalse;

		if(!actionInstanceExecutionHelper.evaluateString(title, "title") ||
			!actionInstanceExecutionHelper.evaluateListElement(mCondition, conditions, "condition") ||
			!actionInstanceExecutionHelper.evaluateIfAction(mIfTrue, "ifTrue") ||
			!actionInstanceExecutionHelper.evaluateIfAction(ifFalse, "ifFalse") ||
			!actionInstanceExecutionHelper.evaluateVariable(mXCoordinate, "xCoordinate") ||
			!actionInstanceExecutionHelper.evaluateVariable(mYCoordinate, "yCoordinate") ||
			!actionInstanceExecutionHelper.evaluateVariable(mWidth, "width") ||
			!actionInstanceExecutionHelper.evaluateVariable(mHeight, "height") ||
			!actionInstanceExecutionHelper.evaluateVariable(mProcessId, "processId"))
			return;

		mTitleRegExp = QRegExp(title, Qt::CaseSensitive, QRegExp::WildcardUnix);

		ActionTools::WindowHandle foundWindow = findWindow();
		if((foundWindow.isValid() && mCondition == Exists) ||
		   (!foundWindow.isValid() && mCondition == DontExists))
		{
			if(mIfTrue.action() == ActionTools::IfActionValue::GOTO)
				actionInstanceExecutionHelper.setNextLine(mIfTrue.line());

			emit executionEnded();
		}
		else
		{
			if(ifFalse.action() == ActionTools::IfActionValue::GOTO)
				actionInstanceExecutionHelper.setNextLine(ifFalse.line());
			else if(ifFalse.action() == ActionTools::IfActionValue::WAIT)
			{
				connect(&mTimer, SIGNAL(timeout()), this, SLOT(checkWindow()));
				mTimer.setInterval(100);
				mTimer.start();
			}
			else
				emit executionEnded();
		}
	}

	void WindowConditionInstance::stopExecution()
	{
		mTimer.stop();
	}

	void WindowConditionInstance::checkWindow()
	{
		ActionTools::WindowHandle foundWindow = findWindow();
		if((foundWindow.isValid() && mCondition == Exists) ||
		   (!foundWindow.isValid() && mCondition == DontExists))
		{
			if(mIfTrue.action() == ActionTools::IfActionValue::GOTO)
			{
				ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
				actionInstanceExecutionHelper.setNextLine(mIfTrue.line());
			}

			mTimer.stop();
			emit executionEnded();
		}
	}

	ActionTools::WindowHandle WindowConditionInstance::findWindow()
	{
		foreach(const ActionTools::WindowHandle &windowHandle, ActionTools::WindowHandle::windowList())
		{
			if(mTitleRegExp.exactMatch(windowHandle.title()))
			{
				QRect windowRect = windowHandle.rect();

				ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
				actionInstanceExecutionHelper.setVariable(mXCoordinate, windowRect.x());
				actionInstanceExecutionHelper.setVariable(mYCoordinate, windowRect.y());
				actionInstanceExecutionHelper.setVariable(mWidth, windowRect.width());
				actionInstanceExecutionHelper.setVariable(mHeight, windowRect.height());
				actionInstanceExecutionHelper.setVariable(mProcessId, windowHandle.processId());

				return windowHandle;
			}
		}

		return ActionTools::WindowHandle();
	}
}
