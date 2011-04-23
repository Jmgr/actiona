/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#include "clickinstance.h"
#include "consolewidget.h"

#include <QPoint>
#include <QTimer>

namespace Actions
{
	ActionTools::StringListPair ClickInstance::buttons = qMakePair(
			QStringList() << "left" << "middle" << "right",
			QStringList() << QT_TRANSLATE_NOOP("ClickInstance::buttons", "Left") << QT_TRANSLATE_NOOP("ClickInstance::buttons", "Middle") << QT_TRANSLATE_NOOP("ClickInstance::buttons", "Right"));
	ActionTools::StringListPair ClickInstance::actions = qMakePair(
			QStringList() << "pressRelease" << "press" << "release",
			QStringList() << QT_TRANSLATE_NOOP("ClickInstance::actions", "Click (press and release)") << QT_TRANSLATE_NOOP("ClickInstance::actions", "Press") << QT_TRANSLATE_NOOP("ClickInstance::actions", "Release"));
	
	void ClickInstance::startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
	
		Action action;
		MouseDevice::Button button;
		QPoint position;
		int amount;
	
		if(!actionInstanceExecutionHelper.evaluateListElement(action, actions, "action", "value") ||
		   !actionInstanceExecutionHelper.evaluateListElement(button, buttons, "button", "value") ||
		   !actionInstanceExecutionHelper.evaluatePoint(position, "position") ||
		   !actionInstanceExecutionHelper.evaluateInteger(amount, "amount"))
			return;
	
		if(action != ClickAction)
			amount = 1;
	
		if(amount <= 0)
		{
			actionInstanceExecutionHelper.setCurrentParameter("amount");
			emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid click amount"));
			return;
		}
		
		mMouseDevice.setCursorPosition(position);
	
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
	
		QTimer::singleShot(1, this, SIGNAL(executionEnded()));
	}

	void ClickInstance::stopLongTermExecution()
	{
		mMouseDevice.reset();
	}
}
