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

#include "backend/keyboard-output-x11.hpp"
#include "backend/keysymhelper-x11.hpp"
#include "backend/keyinput.hpp"
#include "backend/keymapper.hpp"

#include <QX11Info>
#include <QThread>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

namespace Backend
{
    enum Action
    {
        Press,
        Release,
        Trigger
    };

    KeyCode keyToKeycode(const char *key)
    {
        KeySym keySym = XStringToKeysym(key);

        if(keySym == NoSymbol)
            return keyToKeycode("space");

        return XKeysymToKeycode(QX11Info::display(), keySym);
    }

    bool sendCharacter(KeySym keySym)
    {
        bool result = true;
        KeyCode keyCode = KeySymHelper::keySymToKeyCode(keySym);
        int shift = KeySymHelper::keySymToModifier(keySym) % 2;
        const char *wrapKey = KeySymHelper::keyModifiers[
                (KeySymHelper::keySymToModifier(keySym) - shift) / 2];

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

    bool sendKey(const char *key)
    {
        bool result = true;

        result &= XTestFakeKeyEvent(QX11Info::display(), keyToKeycode(key), True, CurrentTime);
        result &= XTestFakeKeyEvent(QX11Info::display(), keyToKeycode(key), False, CurrentTime);

        return result;
    }

    bool doKeyAction(Action action, int nativeKey)
    {
        bool result = true;

        KeyCode keyCode = XKeysymToKeycode(QX11Info::display(), nativeKey);

        if(action == Press || action == Trigger)
            result &= XTestFakeKeyEvent(QX11Info::display(), keyCode, True, CurrentTime);
        if(action == Release || action == Trigger)
            result &= XTestFakeKeyEvent(QX11Info::display(), keyCode, False, CurrentTime);

        XFlush(QX11Info::display());

        return result;
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

    KeyboardOutputX11::KeyboardOutputX11(QObject *parent):
        KeyboardOutput(parent)
    {
    }

    bool KeyboardOutputX11::pressKey(const QString &key)
    {
        return doKeyAction(Press, stringToNativeKey(key));
    }

    bool KeyboardOutputX11::releaseKey(const QString &key)
    {
        return doKeyAction(Release, stringToNativeKey(key));
    }

    bool KeyboardOutputX11::triggerKey(const QString &key)
    {
        return doKeyAction(Trigger, stringToNativeKey(key));
    }

    bool KeyboardOutputX11::writeText(const QString &text, int delay, bool noUnicodeCharacters)
    {
        Q_UNUSED(noUnicodeCharacters)

        bool result = true;
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
                    result &= sendKey("Multi_key");
                    result &= sendCharacter(keySym[0]);
                    result &= sendCharacter(keySym[1]);
                }
                else//Single key
                    result &= sendCharacter(keySym[0]);
            }

            if(delay > 0)
                QThread::msleep(delay);
        }

        return result;
    }
}
