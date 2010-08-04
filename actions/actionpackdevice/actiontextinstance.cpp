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

void sendCharacter(Display *display, KeySym keySym)
{
	KeyCode keyCode = ActionTools::KeySymHelper::keySymToKeyCode(keySym);
	int shift = ActionTools::KeySymHelper::keySymToModifier(keySym) % 2;
	const char *wrapKey = ActionTools::KeySymHelper::keyModifiers[
			(ActionTools::KeySymHelper::keySymToModifier(keySym) - shift) / 2];

	if(wrapKey)
		XTestFakeKeyEvent(display, keyToKeycode(display, wrapKey), True, CurrentTime);
	if(shift)
		XTestFakeKeyEvent(display, keyToKeycode(display, "Shift_L"), True, CurrentTime);

	XTestFakeKeyEvent(display, keyCode, True, CurrentTime);
	XTestFakeKeyEvent(display, keyCode, False, CurrentTime);

	if(shift)
		XTestFakeKeyEvent(display, keyToKeycode(display, "Shift_L"), False, CurrentTime);
	if(wrapKey)
		XTestFakeKeyEvent(display, keyToKeycode(display, wrapKey), False, CurrentTime);

	XFlush(display);
}

void sendKey(Display *display, const char *key)
{
	XTestFakeKeyEvent(display, keyToKeycode(display, key), True, CurrentTime);
	XTestFakeKeyEvent(display, keyToKeycode(display, key), False, CurrentTime);
}

void sendString(Display *display, const QString &string)
{
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
				sendKey(display, "Multi_key");
				sendCharacter(display, keySym[0]);
				sendCharacter(display, keySym[1]);
			}
			else//Single key
				sendCharacter(display, keySym[0]);
		}
	}
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
#endif
#ifdef Q_WS_WIN

#endif
#ifdef Q_WS_MAC
	//TODO_MAC
#endif

	QTimer::singleShot(1, this, SIGNAL(executionEnded()));
}
