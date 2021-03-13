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
#include "backend/windowing.hpp"
#include "backend/system.hpp"

#ifdef Q_OS_WIN
#include "backend/mouse-windows.hpp"
#include "backend/keyboard-windows.hpp"
#include "backend/process-windows.hpp"
#include "backend/windowing-windows.hpp"
#include "backend/system-windows.hpp"
#endif
#ifdef Q_OS_UNIX
#include "backend/mouse-x11.hpp"
#include "backend/keyboard-x11.hpp"
#include "backend/keysymhelper-x11.hpp"
#include "backend/process-unix.hpp"
#include "backend/windowing-x11.hpp"
#include "backend/system-unix.hpp"

#include <QX11Info>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif

namespace Backend
{
    Instance::Instance():
        mMouse(std::make_unique<Mouse>()),
        mKeyboard(std::make_unique<Keyboard>()),
        mProcess(std::make_unique<Process>()),
        mWindowing(std::make_unique<Windowing>()),
        mSystem(std::make_unique<System>())
    {
#if defined(Q_OS_WIN)
        mMouse->isButtonPressed = isButtonPressedWindows;
        mMouse->cursorPosition = cursorPositionWindows;
        mMouse->setCursorPosition = setCursorPositionWindows;
        mMouse->pressButton = pressButtonWindows;
        mMouse->rotateWheel = rotateWheelWindows;
        mKeyboard->pressKey = pressKeyWindows;
        mKeyboard->writeText = writeTextWindows;
        mProcess->killProcess = killProcessWindows;
        mProcess->processStatus = processStatusWindows;
        mProcess->runningProcesses = runningProcessesWindows;
        mProcess->parentProcess = parentProcessWindows;
        mProcess->processCommand = processCommandWindows;
        mProcess->processPriority = processPriorityWindows;
        mWindowing->setForegroundWindow = setForegroundWindowWindows;
        mWindowing->title = titleWindows;
        mWindowing->classname = classnameWindows;
        mWindowing->rect = rectWindows;
        mWindowing->processId = processIdWindows;
        mWindowing->close = closeWindows;
        mWindowing->killCreator = killCreatorWindows;
        mWindowing->minimize = minimizeWindows;
        mWindowing->maximize = maximizeWindows;
        mWindowing->move = moveWindows;
        mWindowing->resize = resizeWindows;
        mWindowing->isActive = isActiveWindows;
        mWindowing->foregroundWindow = foregroundWindowWindows;
        mWindowing->windowList = windowListWindows;
        mSystem->logout = logoutWindows;
        mSystem->restart = restartWindows;
        mSystem->shutdown = shutdownWindows;
        mSystem->suspend = suspendWindows;
        mSystem->hibernate = hibernateWindows;
        mSystem->lockScreen = lockScreenWindows;
        mSystem->startScreenSaver = startScreenSaverWindows;
#elif defined(Q_OS_UNIX)
        mProcess->killProcess = killProcessUnix;
        mProcess->processStatus = processStatusUnix;
        mProcess->runningProcesses = runningProcessesUnix;
        mProcess->parentProcess = parentProcessUnix;
        mProcess->processCommand = processCommandUnix;
        mProcess->processPriority = processPriorityUnix;
        mSystem->logout = logoutUnix;
        mSystem->restart = restartUnix;
        mSystem->shutdown = shutdownUnix;
        mSystem->suspend = suspendUnix;
        mSystem->hibernate = hibernateUnix;
        mSystem->lockScreen = lockScreenUnix;
        mSystem->startScreenSaver = startScreenSaverUnix;

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
        mWindowing->setForegroundWindow = setForegroundWindowX11;
        mWindowing->title = titleX11;
        mWindowing->classname = classnameX11;
        mWindowing->rect = rectX11;
        mWindowing->processId = processIdX11;
        mWindowing->close = closeX11;
        mWindowing->killCreator = killCreatorX11;
        mWindowing->minimize = minimizeX11;
        mWindowing->maximize = maximizeX11;
        mWindowing->move = moveX11;
        mWindowing->resize = resizeX11;
        mWindowing->isActive = isActiveX11;
        mWindowing->foregroundWindow = foregroundWindowX11;
        mWindowing->windowList = windowListX11;

        KeySymHelper::loadKeyCodes();
#else
        useDummy();
#endif
        // TODO: use auto-closer class to close HANDLEs

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
        mProcess->killProcess = killProcessDummy;
        mProcess->processStatus = processStatusDummy;
        mProcess->runningProcesses = runningProcessesDummy;
        mProcess->parentProcess = parentProcessDummy;
        mProcess->processCommand = processCommandDummy;
        mProcess->processPriority = processPriorityDummy;
        mWindowing->setForegroundWindow = setForegroundWindowDummy;
        mWindowing->title = titleDummy;
        mWindowing->classname = classnameDummy;
        mWindowing->rect = rectDummy;
        mWindowing->processId = processIdDummy;
        mWindowing->close = closeDummy;
        mWindowing->killCreator = killCreatorDummy;
        mWindowing->minimize = minimizeDummy;
        mWindowing->maximize = maximizeDummy;
        mWindowing->move = moveDummy;
        mWindowing->resize = resizeDummy;
        mWindowing->isActive = isActiveDummy;
        mWindowing->foregroundWindow = foregroundWindowDummy;
        mWindowing->windowList = windowListDummy;
        mSystem->logout = logoutDummy;
        mSystem->restart = restartDummy;
        mSystem->shutdown = shutdownDummy;
        mSystem->suspend = suspendDummy;
        mSystem->hibernate = hibernateDummy;
        mSystem->lockScreen = lockScreenDummy;
        mSystem->startScreenSaver = startScreenSaverDummy;
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
