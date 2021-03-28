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

#include <QDebug>//TMP

#include "backend/backend.hpp"
#include "backend/capabilities.hpp"
#include "backend/mouse.hpp"
#include "backend/keyboard.hpp"
#include "backend/windowing.hpp"
#include "backend/system.hpp"
#include "backend/feature.hpp"

#ifdef Q_OS_WIN
#include "backend/mouse-windows.hpp"
#include "backend/keyboard-windows.hpp"
#include "backend/process-windows.hpp"
#include "backend/windowing-windows.hpp"
#include "backend/system-windows.hpp"
#endif
#ifdef Q_OS_UNIX
#include "backend/mouse-xtest.hpp"
#include "backend/keyboard-xtest.hpp"
#include "backend/keysymhelper-x11.hpp"
#include "backend/process-unix.hpp"
#include "backend/windowing-x11.hpp"
#include "backend/system-unix.hpp"
#endif

namespace Backend
{
    Instance::Instance():
        mCapabilities(std::make_unique<Capabilities>()),
        mMouse(std::make_unique<Mouse>(*mCapabilities)),
        mKeyboard(std::make_unique<Keyboard>(*mCapabilities)),
        mProcess(std::make_unique<Process>()),
        mWindowing(std::make_unique<Windowing>(*mCapabilities)),
        mSystem(std::make_unique<System>(*mCapabilities))
    {
#if defined(Q_OS_WIN)
        // TODO
#elif defined(Q_OS_UNIX)
        //mWindowing->createFullscreenScreenshooter = createFullscreenScreenshooterX11;
        //mWindowing->createAreaScreenshooter = createAreaScreenshooterX11;
        //mWindowing->createWindowScreenshooter = createWindowScreenshooterX11;
        //TODO: choose one
        //mWindowing->createFullscreenScreenshooter = createFullscreenScreenshooterGnome; // TODO
        //mWindowing->createAreaScreenshooter = createAreaScreenshooterGnome; // TODO
        //mWindowing->createWindowScreenshooter = createWindowScreenshooterGnome; // TODO

        KeySymHelper::loadKeyCodes();
#endif

        /*
        //TODO
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
        */
    }

    Instance::~Instance()
    {
    }

    Instance &Instance::get()
    {
        static Instance instance;

        return instance;
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
