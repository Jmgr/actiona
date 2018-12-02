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

	Contact : jmgr@jmgr.info
*/

#include "keyboarddevice.h"
#include "keymapper.h"
#include "keyinput.h"
#include "crossplatform.h"

#ifdef Q_OS_UNIX
#include "keysymhelper.h"
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

#ifdef Q_OS_WIN
#include <unordered_set>
#include <Windows.h>

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646267(v=vs.85).aspx
static const std::unordered_set<int> extendedKeys =
{{
    VK_RMENU, // Alt
    VK_RCONTROL,
    VK_INSERT,
    VK_DELETE,
    VK_HOME,
    VK_END,
    VK_PRIOR, // Page Up
    VK_NEXT, // Page Down
    VK_UP,
    VK_DOWN,
    VK_LEFT,
    VK_RIGHT,
    VK_NUMLOCK,
    VK_PRINT
}};
#endif

#include <array>

KeyboardDevice::KeyboardDevice() = default;

KeyboardDevice::~KeyboardDevice()
{
	reset();
}

void KeyboardDevice::reset()
{
    for(int nativeKey: mPressedKeys)
        doKeyAction(Release, nativeKey, false);

    mPressedKeys.clear();
}

bool KeyboardDevice::pressKey(const QString &key)
{
	return doKeyAction(Press, stringToNativeKey(key));
}

bool KeyboardDevice::releaseKey(const QString &key)
{
	return doKeyAction(Release, stringToNativeKey(key));
}

bool KeyboardDevice::triggerKey(const QString &key)
{
	return doKeyAction(Trigger, stringToNativeKey(key));
}

#ifdef Q_OS_UNIX
static KeyCode keyToKeycode(const char *key)
{
	KeySym keySym = XStringToKeysym(key);

	if(keySym == NoSymbol)
		return keyToKeycode("space");

	return XKeysymToKeycode(QX11Info::display(), keySym);
}

static bool sendCharacter(KeySym keySym)
{
	bool result = true;
	KeyCode keyCode = ActionTools::KeySymHelper::keySymToKeyCode(keySym);
	int shift = ActionTools::KeySymHelper::keySymToModifier(keySym) % 2;
	const char *wrapKey = ActionTools::KeySymHelper::keyModifiers[
			(ActionTools::KeySymHelper::keySymToModifier(keySym) - shift) / 2];

	if(wrapKey)
		result &= XTestFakeKeyEvent(QX11Info::display(), keyToKeycode(wrapKey), True, CurrentTime);
	if(shift)
		result &= XTestFakeKeyEvent(QX11Info::display(), keyToKeycode("Shift_L"), True, CurrentTime);

	result &= XTestFakeKeyEvent(QX11Info::display(), keyCode, True, CurrentTime);
	result &= XTestFakeKeyEvent(QX11Info::display(), keyCode, False, CurrentTime);

	if(shift)
		result &= XTestFakeKeyEvent(QX11Info::display(), keyToKeycode("Shift_L"), False, CurrentTime);
	if(wrapKey)
		result &= XTestFakeKeyEvent(QX11Info::display(), keyToKeycode(wrapKey), False, CurrentTime);

	XFlush(QX11Info::display());

	return result;
}

static bool sendKey(const char *key)
{
	bool result = true;

	result &= XTestFakeKeyEvent(QX11Info::display(), keyToKeycode(key), True, CurrentTime);
	result &= XTestFakeKeyEvent(QX11Info::display(), keyToKeycode(key), False, CurrentTime);

	return result;
}
#endif

bool KeyboardDevice::writeText(const QString &text, int delay, bool noUnicodeCharacters) const
{
#ifdef Q_OS_UNIX
	bool result = true;
	KeySym keySym[2];
	std::wstring wideString = text.toStdWString();
	wchar_t wcSinglecharStr[2] = {L'\0'};

	for(unsigned int i = 0; wideString[i] != L'\0' && i < wideString.size(); ++i)
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
				result &= sendKey("Multi_key");
				result &= sendCharacter(keySym[0]);
				result &= sendCharacter(keySym[1]);
			}
			else//Single key
				result &= sendCharacter(keySym[0]);
		}

		if(delay > 0)
			ActionTools::CrossPlatform::sleep(delay);
	}

	return result;
#endif
	
#ifdef Q_OS_WIN
    std::array<INPUT, 2> input;
    SecureZeroMemory(input.data(), input.size() * sizeof(INPUT));
	bool result = true;

	for(int i = 0; i < 2; ++i)
	{
		input[i].type = INPUT_KEYBOARD;
		input[i].ki.wVk = 0;
        if(noUnicodeCharacters)
            input[i].ki.dwFlags = KEYEVENTF_SCANCODE | (i == 0 ? 0 : KEYEVENTF_KEYUP);
        else
            input[i].ki.dwFlags = KEYEVENTF_UNICODE | (i == 0 ? 0 : KEYEVENTF_KEYUP);
		input[i].ki.time = 0;
        input[i].ki.dwExtraInfo = 0;
    }

    HKL keyboardLayout = GetKeyboardLayout(0);

    auto sendModifiersFunction = [&keyboardLayout](int key, int additionalFlags)
    {
        INPUT modifierInput;
        SecureZeroMemory(&modifierInput, sizeof(INPUT));

        modifierInput.type = INPUT_KEYBOARD;
        modifierInput.ki.dwFlags = KEYEVENTF_SCANCODE | additionalFlags;
        modifierInput.ki.wScan = MapVirtualKeyEx(key, MAPVK_VK_TO_VSC, keyboardLayout);

        if(extendedKeys.count(key) > 0)
            modifierInput.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;

        SendInput(1, &modifierInput, sizeof(INPUT));
    };

	for(int i = 0; i < text.length(); ++i)
	{
        SHORT virtualKey = 0;

        if(noUnicodeCharacters)
        {
            virtualKey = VkKeyScanEx(text[i].unicode(), keyboardLayout);
            auto scanCode = MapVirtualKeyEx(LOBYTE(virtualKey), MAPVK_VK_TO_VSC, keyboardLayout);

            if(extendedKeys.count(virtualKey) > 0)
            {
                input[0].ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
                input[1].ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
            }

            if(HIBYTE(virtualKey) & 1) //Shift
                sendModifiersFunction(VK_LSHIFT, 0);

            if(HIBYTE(virtualKey) & 2) //Control
                sendModifiersFunction(VK_LCONTROL, 0);

            if(HIBYTE(virtualKey) & 4) //Alt
                sendModifiersFunction(VK_LMENU, 0);

            input[0].ki.wVk = input[1].ki.wVk = virtualKey;
            input[0].ki.wScan = input[1].ki.wScan = scanCode;
        }
        else
        {
            input[0].ki.wScan = input[1].ki.wScan = text[i].unicode();
        }

        result &= (SendInput(2, input.data(), sizeof(INPUT)) != 0);

        if(noUnicodeCharacters)
        {
            if(HIBYTE(virtualKey) & 4) //Alt
                sendModifiersFunction(VK_LMENU, KEYEVENTF_KEYUP);

            if(HIBYTE(virtualKey) & 2) //Control
                sendModifiersFunction(VK_LCONTROL, KEYEVENTF_KEYUP);

            if(HIBYTE(virtualKey) & 1) //Shift
                sendModifiersFunction(VK_LSHIFT, KEYEVENTF_KEYUP);
        }

		if(delay > 0)
			ActionTools::CrossPlatform::sleep(delay);
	}

	return result;
#endif
}

bool KeyboardDevice::doKeyAction(Action action, int nativeKey, bool alterPressedKeys)
{
	bool result = true;
	
#ifdef Q_OS_UNIX
    KeyCode keyCode = XKeysymToKeycode(QX11Info::display(), nativeKey);
	
	if(action == Press || action == Trigger)
		result &= XTestFakeKeyEvent(QX11Info::display(), keyCode, True, CurrentTime);
	if(action == Release || action == Trigger)
		result &= XTestFakeKeyEvent(QX11Info::display(), keyCode, False, CurrentTime);

	XFlush(QX11Info::display());
#endif
	
#ifdef Q_OS_WIN
	INPUT input;
    SecureZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;

	switch(mType)
	{
	case Win32:
    {
        input.ki.wVk = nativeKey;

        HKL keyboardLayout = GetKeyboardLayout(0);
        input.ki.wScan = MapVirtualKeyEx(nativeKey, MAPVK_VK_TO_VSC, keyboardLayout);

        if(extendedKeys.count(nativeKey) > 0)
            input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
    }
		break;
	case DirectX:
		input.ki.wVk = 0;
		input.ki.wScan = ActionTools::KeyMapper::toDirectXKey(nativeKey);
		break;
	}

	if(action == Press || action == Trigger)
        result &= (SendInput(1, &input, sizeof(INPUT)) != 0);
	if(action == Release || action == Trigger)
	{
		input.ki.dwFlags |= KEYEVENTF_KEYUP;

        result &= (SendInput(1, &input, sizeof(INPUT)) != 0);
	}
#endif
	
    if(alterPressedKeys)
    {
        if(action == Press)
            mPressedKeys.insert(nativeKey);
        else if(action == Release)
            mPressedKeys.remove(nativeKey);
    }

    return result;
}

int KeyboardDevice::stringToNativeKey(const QString &key) const
{
    ActionTools::KeyInput keyInput;
	keyInput.fromPortableText(key);
	
	if(keyInput.isQtKey())
		return ActionTools::KeyMapper::toNativeKey(static_cast<Qt::Key>(keyInput.key()));
	else
		return ActionTools::KeyInput::nativeKey(keyInput.key());
}
