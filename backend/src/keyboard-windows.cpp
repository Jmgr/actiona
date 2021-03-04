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

#include "backend/keyboard-windows.hpp"
#include "backend/keyinput.hpp"
#include "backend/keymapper.hpp"

#include <QThread>

#include <unordered_set>
#include <array>

#include <Windows.h>

namespace Backend
{
    // https://msdn.microsoft.com/en-us/library/windows/desktop/ms646267(v=vs.85).aspx
    const std::unordered_set<int> extendedKeys =
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

    void pressKey(int nativeKey, bool press, bool directX)
    {
        INPUT input;
        ZeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_KEYBOARD;

        if(directX)
        {
            input.ki.wVk = 0;
            input.ki.wScan = KeyMapper::toDirectXKey(nativeKey);
        }
        else
        {
            input.ki.wVk = nativeKey;

            HKL keyboardLayout = GetKeyboardLayout(0);
            input.ki.wScan = MapVirtualKeyEx(nativeKey, MAPVK_VK_TO_VSC, keyboardLayout);

            if(extendedKeys.count(nativeKey) > 0)
                input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
        }

        if(!press)
            input.ki.dwFlags |= KEYEVENTF_KEYUP;

        int res = SendInput(1, &input, sizeof(INPUT));
        if(res == 0)
            throw BackendError(QStringLiteral("failed to press key, error code: %1").arg(GetLastError()));
        if(res != 1)
            throw BackendError(QStringLiteral("failed to press key"));
    }

    int stringToNativeKey(const QString &key)
    {
        KeyInput keyInput;
        keyInput.fromPortableText(key);

        if(keyInput.isQtKey())
            return KeyMapper::toNativeKey(static_cast<Qt::Key>(keyInput.key()));
        else
            return KeyInput::nativeKey(keyInput.key());
    }

    void pressKeyWindows(const QString &key, bool press, bool directX)
    {
        pressKey(stringToNativeKey(key), press, directX);
    }

    void writeTextWindows(const QString &text, int delay, bool noUnicodeCharacters)
    {
        std::array<INPUT, 2> input;
        ZeroMemory(input.data(), input.size() * sizeof(INPUT));

        for(int i = 0; i < 2; ++i)
        {
            input[i].type = INPUT_KEYBOARD;
            if(noUnicodeCharacters)
                input[i].ki.dwFlags = KEYEVENTF_SCANCODE | (i == 0 ? 0 : KEYEVENTF_KEYUP);
            else
                input[i].ki.dwFlags = KEYEVENTF_UNICODE | (i == 0 ? 0 : KEYEVENTF_KEYUP);
        }

        HKL keyboardLayout = GetKeyboardLayout(0);

        auto sendModifiersFunction = [&keyboardLayout](int key, int additionalFlags)
        {
            INPUT modifierInput;
            ZeroMemory(&modifierInput, sizeof(INPUT));

            modifierInput.type = INPUT_KEYBOARD;
            modifierInput.ki.dwFlags = KEYEVENTF_SCANCODE | additionalFlags;
            modifierInput.ki.wScan = MapVirtualKeyEx(key, MAPVK_VK_TO_VSC, keyboardLayout);

            if(extendedKeys.count(key) > 0)
                modifierInput.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;

            int res = SendInput(1, &modifierInput, sizeof(INPUT));
            if(res == 0)
                throw BackendError(QStringLiteral("failed to press key, error code: %1").arg(GetLastError()));
            if(res != 1)
                throw BackendError(QStringLiteral("failed to press key"));
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

            int res = SendInput(2, input.data(), sizeof(INPUT));
            if(res == 0)
                throw BackendError(QStringLiteral("failed to press key, error code: %1").arg(GetLastError()));
            if(res != 2)
                throw BackendError(QStringLiteral("failed to press key"));

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
                QThread::msleep(delay);
        }
    }
}

