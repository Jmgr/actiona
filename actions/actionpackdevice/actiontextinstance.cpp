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

#include "actiontextinstance.h"
#include "actioninstanceexecutionhelper.h"

#ifdef Q_WS_X11
#include "xdisplayhelper.h"
#include "keysymhelper.h"
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif
#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <QTimer>

#ifdef Q_WS_X11
KeyCode keyToKeycode(Display *display, const char *key)
{
	KeySym keySym = XStringToKeysym(key);

	if(keySym == NoSymbol)
		return keyToKeycode(display, "space");

	return XKeysymToKeycode(display, keySym);
}

bool sendCharacter(Display *display, KeySym keySym)
{
	bool result = true;
	KeyCode keyCode = ActionTools::KeySymHelper::keySymToKeyCode(keySym);
	int shift = ActionTools::KeySymHelper::keySymToModifier(keySym) % 2;
	const char *wrapKey = ActionTools::KeySymHelper::keyModifiers[
			(ActionTools::KeySymHelper::keySymToModifier(keySym) - shift) / 2];

	if(wrapKey)
		result &= (XTestFakeKeyEvent(display, keyToKeycode(display, wrapKey), True, CurrentTime) == Success);
	if(shift)
		result &= (XTestFakeKeyEvent(display, keyToKeycode(display, "Shift_L"), True, CurrentTime) == Success);

	result &= (XTestFakeKeyEvent(display, keyCode, True, CurrentTime) == Success);
	result &= (XTestFakeKeyEvent(display, keyCode, False, CurrentTime) == Success);

	if(shift)
		result &= (XTestFakeKeyEvent(display, keyToKeycode(display, "Shift_L"), False, CurrentTime) == Success);
	if(wrapKey)
		result &= (XTestFakeKeyEvent(display, keyToKeycode(display, wrapKey), False, CurrentTime) == Success);

	XFlush(display);
	
	return result;
}

bool sendKey(Display *display, const char *key)
{
	bool result = true;
	
	result &= (XTestFakeKeyEvent(display, keyToKeycode(display, key), True, CurrentTime) == Success);
	result &= (XTestFakeKeyEvent(display, keyToKeycode(display, key), False, CurrentTime) == Success);
	
	return result;
}

bool sendString(Display *display, const QString &string)
{
	bool result = true;
	KeySym keySym[2];
	wchar_t wideString[string.length()];
	wchar_t wcSinglecharStr[2] = {L'\0'};

	string.toWCharArray(wideString);

	for(int i = 0; wideString[i] != L'\0' && i < string.length(); ++i)
	{
		wcSinglecharStr[0] = wideString[i];

		//KeySym lookup
		keySym[0] = ActionTools::KeySymHelper::wcharToKeySym(wcSinglecharStr[0]);
		keySym[1] = 0;

		if(keySym[0] == 0 || ActionTools::KeySymHelper::keySymToKeyCode(keySym[0]) == 0)
		{
			//No keycode found -> try to find a Multi_key combination for this character
			keySym[0] = 0;

			for(int j = 0; j < ActionTools::KeySymHelper::MULTIKEY_MAP_SIZE; ++j)
			{
				if(wcSinglecharStr[0] == ActionTools::KeySymHelper::multikeyMapChar[j])//Found
				{
					keySym[0] = ActionTools::KeySymHelper::wcharToKeySym(ActionTools::KeySymHelper::multikeyMapFirst[j]);
					keySym[1] = ActionTools::KeySymHelper::wcharToKeySym(ActionTools::KeySymHelper::multikeyMapSecond[j]);
					if((ActionTools::KeySymHelper::keySymToKeyCode(keySym[0]) == 0)
						|| (ActionTools::KeySymHelper::keySymToKeyCode(keySym[1]) == 0))
						keySym[0] = 0;//Character not supported

					break;
				}
			}
		}

		if(keySym[0])
		{
			if(keySym[1])//Multi key sequence
			{
				result &= sendKey(display, "Multi_key");
				result &= sendCharacter(display, keySym[0]);
				result &= sendCharacter(display, keySym[1]);
			}
			else//Single key
				result &= sendCharacter(display, keySym[0]);
		}
	}
	
	return result;
}
#endif

#ifdef Q_WS_WIN
bool sendString(const QString &string)
{
	INPUT input[2];
	wchar_t *wideString = new wchar_t[string.length()];
	bool result = true;

	string.toWCharArray(wideString);

	for(int i = 0; i < 2; ++i)
	{
		input[i].type = INPUT_KEYBOARD;
		input[i].ki.wVk = 0;
		input[i].ki.dwFlags = KEYEVENTF_UNICODE | (i == 0 ? 0 : KEYEVENTF_KEYUP);
		input[i].ki.time = 0;
		input[i].ki.dwExtraInfo = 0;
	}

	for(int i = 0; i < string.length(); ++i)
	{
		input[0].ki.wScan = input[1].ki.wScan = wideString[i];

		result &= (SendInput(2, input, sizeof(INPUT)) != 0);
	}

	delete [] wideString;

	return result;
}
#endif

void ActionTextInstance::startExecution()
{
	ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());

	QString text;

	if(!actionInstanceExecutionHelper.evaluateString(text, "text"))
		return;

#ifdef Q_WS_X11
	ActionTools::XDisplayHelper xDisplayHelper;
	if(!xDisplayHelper.display())
	{
		emit executionException(FailedToSendInputException, tr("Unable to emulate keypress: cannot open display"));
		return;
	}

	sendString(xDisplayHelper.display(), text);
	//We should emit a warning here, but not an error
	//For now, juste silently ignore any character that can't be emulated
#endif
#ifdef Q_WS_WIN
	if(!sendString(text))
	{
		emit executionException(FailedToSendInputException, tr("Unable to send input"));
		return;
	}
#endif
#ifdef Q_WS_MAC
	//TODO_MAC
#endif

	QTimer::singleShot(1, this, SIGNAL(executionEnded()));
}
