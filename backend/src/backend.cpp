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

#include "backend/backend.hpp"
#include "backend/mouse.hpp"
#include "backend/keyboard.hpp"

#ifdef Q_OS_WIN
#include "backend/mouse-windows.hpp"
#include "backend/keyboard-windows.hpp"
#endif
#ifdef Q_OS_UNIX
#include "backend/mouse-x11.hpp"
#include "backend/keyboard-x11.hpp"
#include "backend/keysymhelper-x11.hpp"

#include <QX11Info>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif

namespace Backend
{
    Instance::Instance():
        mMouse(std::make_unique<Mouse>()),
        mKeyboard(std::make_unique<Keyboard>())
    {
#if defined(Q_OS_WIN)
        mMouse->isButtonPressed = isButtonPressedWindows;
        mMouse->cursorPosition = cursorPositionWindows;
        mMouse->setCursorPosition = setCursorPositionWindows;
        mMouse->pressButton = pressButtonWindows;
        mMouse->rotateWheel = rotateWheelWindows;
        mKeyboard->pressKey = pressKeyWindows;
        mKeyboard->writeText = writeTextWindows;
#elif defined(Q_OS_UNIX)
        auto display = QX11Info::display();

        int unused;
        if(!display || !XTestQueryExtension(display, &unused, &unused, &unused, &unused))
        {
            // TODO: display a messagebox?
            useDummy();
            return;
        }

        mMouse->isButtonPressed = isButtonPressedX11;
        mMouse->cursorPosition = cursorPositionX11;
        mMouse->setCursorPosition = setCursorPositionX11;
        mMouse->pressButton = pressButtonX11;
        mMouse->rotateWheel = rotateWheelX11;
        mKeyboard->pressKey = pressKeyX11;
        mKeyboard->writeText = writeTextX11;

        KeySymHelper::loadKeyCodes();
#else
        useDummy();
#endif

        auto pressButton = mMouse->pressButton;
        mMouse->pressButton = [this, pressButton](Mouse::Button button, bool press)
        {
            pressButton(button, press);

            if(press)
                mPressedButtons.insert(static_cast<int>(button));
            else
                mPressedButtons.erase(static_cast<int>(button));
        };

        auto pressKey = mKeyboard->pressKey;
        mKeyboard->pressKey = [this, pressKey](const QString &key, bool press, bool directX)
        {
            pressKey(key, press, directX);

            if(press)
                mPressedKeys.insert(std::make_pair(key, directX));
            else
                mPressedKeys.erase(std::make_pair(key, directX));
        };
    }

    Instance::~Instance()
    {
    }

    Instance &Instance::get()
    {
        static Instance instance;

        return instance;
    }

    void Instance::useDummy()
    {
        qWarning("Backend: using dummy");
        mMouse->isButtonPressed = isButtonPressedDummy;
        mMouse->cursorPosition = cursorPositionDummy;
        mMouse->setCursorPosition = setCursorPositionDummy;
        mMouse->pressButton = pressButtonDummy;
        mMouse->rotateWheel = rotateWheelDummy;
        mKeyboard->pressKey = pressKeyDummy;
        mKeyboard->writeText = writeTextDummy;
    }

    void Instance::instReleaseAll()
    {
        for(auto buttonIndex: mPressedButtons)
            mMouse->pressButton(static_cast<Mouse::Button>(buttonIndex), false);

        mPressedButtons.clear();

        for(const auto &key: mPressedKeys)
            mKeyboard->pressKey(key.first, false, key.second);

        mPressedKeys.clear();
    }
}
