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
	
	#ifdef Q_WS_WIN
		//TODO : Fix the AltGr problem under Windows
		/*
		if(keyInput.key() == ActionTools::KeyInput::AltGr)
		{
			if(mAltGrPressed && action != ReleaseAction)
			{
				actionInstanceExecutionHelper.setCurrentParameter("action");
				emit executionException(InvalidActionException, tr("Cannot press the key because it's already pressed"));
				return;
			}
	
			if(!mAltGrPressed && action == ReleaseAction)
			{
				actionInstanceExecutionHelper.setCurrentParameter("action");
				emit executionException(InvalidActionException, tr("Cannot press the key because it's already pressed"));
				return;
			}
	
			bool result = true;
	
			INPUT input[2];
	
			for(int i = 0; i < 2; ++i)
			{
				input[i].type = INPUT_KEYBOARD;
				input[i].ki.dwFlags = 0;
				input[i].ki.time = 0;
				input[i].ki.dwExtraInfo = 0;
			}
	
			if(action == PressAction || action == PressReleaseAction)
			{
				//Press right alt
				input[0].ki.wVk = VK_RMENU;
	
				//Press left control
				input[1].ki.wVk = VK_LCONTROL;
	
				result &= (SendInput(2, input, sizeof(INPUT)) != 0);
			}
			if(action == ReleaseAction || action == PressReleaseAction)
			{
				input[0].ki.dwFlags = KEYEVENTF_KEYUP;
				input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	
				//Release left control
				input[0].ki.wVk = VK_LCONTROL;
	
				//Release right alt
				input[1].ki.wVk = VK_RMENU;
	
				result &= (SendInput(2, input, sizeof(INPUT)) != 0);
			}
	
			if(!result)
			{
				emit executionException(FailedToSendInputException, tr("Unable to emulate key: failed to send input"));
				return;
			}
	
			if(action == PressAction)
				mAltGrPressed = true;
			else if(action == ReleaseAction)
				mAltGrPressed = false;
	
			emit executionEnded();
	
			return;
		}
		*/
	#endif
	
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
