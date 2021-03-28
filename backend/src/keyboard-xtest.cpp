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

#include "backend/keyboard-xtest.hpp"
#include "backend/keysymhelper-x11.hpp"
#include "backend/keyinput.hpp"
#include "backend/keymapper.hpp"

#pragma push_macro("Bool")
#undef Bool
#include <QX11Info>
#include <QThread>
#pragma pop_macro("Bool")

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

namespace Backend
{
    KeyCode keyToKeycode(const char *key)
    {
        auto keySym = XStringToKeysym(key);
        if(keySym == NoSymbol)
            return keyToKeycode("space");

        auto keyCode = XKeysymToKeycode(QX11Info::display(), keySym);
        if(keyCode == 0)
            throw BackendError();

        return keyCode;
    }

    void sendCharacter(KeySym keySym)
    {
        auto keyCode = KeySymHelper::keySymToKeyCode(keySym);
        int shift = KeySymHelper::keySymToModifier(keySym) % 2;
        const char *wrapKey = KeySymHelper::keyModifiers[
                (KeySymHelper::keySymToModifier(keySym) - shift) / 2];

        if(wrapKey)
        {
            if(!XTestFakeKeyEvent(QX11Info::display(), keyToKeycode(wrapKey), True, CurrentTime))
                throw BackendError();
        }
        if(shift)
        {
            if(!XTestFakeKeyEvent(QX11Info::display(), keyToKeycode("Shift_L"), True, CurrentTime))
                throw BackendError();
        }

        if(!XTestFakeKeyEvent(QX11Info::display(), keyCode, True, CurrentTime))
            throw BackendError();
        if(!XTestFakeKeyEvent(QX11Info::display(), keyCode, False, CurrentTime))
            throw BackendError();

        if(shift)
        {
            if(!XTestFakeKeyEvent(QX11Info::display(), keyToKeycode("Shift_L"), False, CurrentTime))
                throw BackendError();
        }
        if(wrapKey)
        {
            if(!XTestFakeKeyEvent(QX11Info::display(), keyToKeycode(wrapKey), False, CurrentTime))
                throw BackendError();
        }

        if(!XFlush(QX11Info::display()))
            throw BackendError();
    }

    void sendKey(const char *key)
    {
        if(!XTestFakeKeyEvent(QX11Info::display(), keyToKeycode(key), True, CurrentTime))
            throw BackendError();

        if(!XTestFakeKeyEvent(QX11Info::display(), keyToKeycode(key), False, CurrentTime))
            throw BackendError();
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

    void pressKeyXTest(const QString &key, bool press, bool directX)
    {
        Q_UNUSED(directX)

        auto nativeKey = stringToNativeKey(key);
        auto keyCode = XKeysymToKeycode(QX11Info::display(), nativeKey);
        if(keyCode == 0)
            throw BackendError();

        if(!XTestFakeKeyEvent(QX11Info::display(), keyCode, press, CurrentTime))
            throw BackendError();

        if(!XFlush(QX11Info::display()))
            throw BackendError();
    }

    void writeTextXTest(const QString &text, int delay, bool noUnicodeCharacters)
    {
        Q_UNUSED(noUnicodeCharacters)

        KeySym keySym[2];
        std::wstring wideString = text.toStdWString();
        wchar_t wcSinglecharStr[2] = {L'\0'};

        for(unsigned int i = 0; wideString[i] != L'\0' && i < wideString.size(); ++i)
        {
            wcSinglecharStr[0] = wideString[i];

            //KeySym lookup
            keySym[0] = KeySymHelper::wcharToKeySym(wcSinglecharStr[0]);
            keySym[1] = 0;

            if(keySym[0] == 0 || KeySymHelper::keySymToKeyCode(keySym[0]) == 0)
            {
                //No keycode found -> try to find a Multi_key combination for this character
                keySym[0] = 0;

                for(int j = 0; j < KeySymHelper::MULTIKEY_MAP_SIZE; ++j)
                {
                    if(wcSinglecharStr[0] == KeySymHelper::multikeyMapChar[j])//Found
                    {
                        keySym[0] = KeySymHelper::wcharToKeySym(KeySymHelper::multikeyMapFirst[j]);
                        keySym[1] = KeySymHelper::wcharToKeySym(KeySymHelper::multikeyMapSecond[j]);
                        if((KeySymHelper::keySymToKeyCode(keySym[0]) == 0)
                            || (KeySymHelper::keySymToKeyCode(keySym[1]) == 0))
                            keySym[0] = 0;//Character not supported

                        break;
                    }
                }
            }

            if(keySym[0])
            {
                if(keySym[1])//Multi key sequence
                {
                    sendKey("Multi_key");
                    sendCharacter(keySym[0]);
                    sendCharacter(keySym[1]);
                }
                else//Single key
                    sendCharacter(keySym[0]);
            }

            if(delay > 0)
                QThread::msleep(delay);
        }
    }
}

