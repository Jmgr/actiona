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

#include "actionkeyinstance.h"
#include "keyinput.h"
#include "keymapper.h"

#include <QKeySequence>
#include <QKeyEvent>
#include <QDebug>

#ifdef Q_WS_X11
#include "xdisplayhelper.h"
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#define XK_MISCELLANY
#define XK_LATIN1
#define XK_KOREAN
#define XK_XKB_KEYS
#include <X11/keysymdef.h>
#include <X11/XF86keysym.h>
#include <QX11Info>
#endif
#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

ActionTools::StringListPair ActionKeyInstance::actions = qMakePair(
		QStringList() << "pressRelease" << "press" << "release",
		QStringList() << tr("Press and release") << tr("Press") << tr("Release"));

QSet<int> ActionKeyInstance::mPressedKeys;
#ifdef Q_WS_WIN
bool ActionKeyInstance::mAltGrPressed = false;
#endif

void ActionKeyInstance::startExecution()
{
	ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());

	QString key;
	Action action;

	if(!actionInstanceExecutionHelper.evaluateString(key, "key", "key") ||
	   !actionInstanceExecutionHelper.evaluateListElement(action, actions, "action"))
		return;

	ActionTools::KeyInput keyInput;
	keyInput.fromPortableText(key);

#ifdef Q_WS_WIN
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
#endif

	int nativeKey;

	if(keyInput.isQtKey())
		nativeKey = ActionTools::KeyMapper::toNativeKey(static_cast<Qt::Key>(keyInput.key()));
	else
		nativeKey = ActionTools::KeyInput::nativeKey(keyInput.key());

	if(mPressedKeys.contains(nativeKey) && action != ReleaseAction)
	{
		actionInstanceExecutionHelper.setCurrentParameter("action");
		emit executionException(InvalidActionException, tr("Cannot press the key because it's already pressed"));
		return;
	}

	if(!mPressedKeys.contains(nativeKey) && action == ReleaseAction)
	{
		actionInstanceExecutionHelper.setCurrentParameter("action");
		emit executionException(InvalidActionException, tr("Cannot release the key because it's not pressed"));
		return;
	}

	bool result = true;

#ifdef Q_WS_X11
	ActionTools::XDisplayHelper xDisplayHelper;
	if(action == PressAction || action == PressReleaseAction)
		result &= XTestFakeKeyEvent(xDisplayHelper.display(), XKeysymToKeycode(xDisplayHelper.display(), nativeKey), True, CurrentTime);
	if(action == ReleaseAction || action == PressReleaseAction)
		result &= XTestFakeKeyEvent(xDisplayHelper.display(), XKeysymToKeycode(xDisplayHelper.display(), nativeKey), False, CurrentTime);

	XFlush(xDisplayHelper.display());
#endif
#ifdef Q_WS_WIN
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = nativeKey;
	input.ki.wScan = 0;
	input.ki.dwFlags = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	if(action == PressAction || action == PressReleaseAction)
		result &= (SendInput(1, &input, sizeof(INPUT)) != 0);
	if(action == ReleaseAction || action == PressReleaseAction)
	{
		input.ki.dwFlags = KEYEVENTF_KEYUP;

		result &= (SendInput(1, &input, sizeof(INPUT)) != 0);
	}
#endif

	if(!result)
	{
		emit executionException(FailedToSendInputException, tr("Unable to emulate key: failed to send input"));
		return;
	}

	if(action == PressAction)
		mPressedKeys.insert(nativeKey);
	else if(action == ReleaseAction)
		mPressedKeys.remove(nativeKey);

	emit executionEnded();
}

void ActionKeyInstance::stopLongTermExecution()
{
	//Release any pressed key, so that we don't end with an invalid state
	foreach(int nativeKey, mPressedKeys)
	{
#ifdef Q_WS_X11
		ActionTools::XDisplayHelper xDisplayHelper;
		if(!XTestFakeButtonEvent(xDisplayHelper.display(), XKeysymToKeycode(xDisplayHelper.display(), nativeKey), False, CurrentTime))
			continue;

		XFlush(xDisplayHelper.display());
#endif
#ifdef Q_WS_WIN
		INPUT input;
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = nativeKey;
		input.ki.wScan = 0;
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;

		SendInput(1, &input, sizeof(INPUT));
#endif
	}

	mPressedKeys.clear();

#ifdef Q_WS_WIN
	if(mAltGrPressed)
	{
		INPUT input;
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = VK_MENU;
		input.ki.wScan = 0;
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;

		SendInput(1, &input, sizeof(INPUT));

		input.ki.wVk = VK_CONTROL;

		SendInput(1, &input, sizeof(INPUT));

		mAltGrPressed = false;
	}
#endif
}
