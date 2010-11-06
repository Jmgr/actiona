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

#include "keyinstance.h"
#include "keyinput.h"

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <QDebug>

namespace Actions
{
	ActionTools::StringListPair KeyInstance::actions = qMakePair(
			QStringList() << "pressRelease" << "press" << "release",
			QStringList() << QObject::tr("Press and release") << QObject::tr("Press") << QObject::tr("Release"));

	void KeyInstance::startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
	
		QString key;
		Action action;
	
		if(!actionInstanceExecutionHelper.evaluateString(key, "key", "key") ||
		   !actionInstanceExecutionHelper.evaluateListElement(action, actions, "action"))
			return;
	
		bool result = true;
	
		switch(action)
		{
		case PressAction:
			result &= mKeyboardDevice.pressKey(key);
			break;
		case ReleaseAction:
			result &= mKeyboardDevice.releaseKey(key);
			break;
		case PressReleaseAction:
			result &= mKeyboardDevice.triggerKey(key);
			break;
		}
		
		if(!result)
		{
			emit executionException(FailedToSendInputException, tr("Unable to emulate key: failed to send input"));
			return;
		}

		emit executionEnded();
	}
}
