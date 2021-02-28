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

#ifdef Q_OS_WIN
#include "backend/mouse-input-windows.hpp"
#include "backend/mouse-output-windows.hpp"
#include "backend/keyboard-input-windows.hpp"
#include "backend/keyboard-output-windows.hpp"
#endif

#ifdef Q_OS_UNIX
#include "backend/keysymhelper-x11.hpp"
#include "backend/mouse-input-x11.hpp"
#include "backend/mouse-output-x11.hpp"
#include "backend/keyboard-input-x11.hpp"
#include "backend/keyboard-output-x11.hpp"

#include <QX11Info>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif

namespace Backend
{
    Backend *Backend::mBackend = nullptr;

    Backend::Backend(QObject *parent):
        QObject(parent)
    {
#ifdef Q_OS_WIN
        mMouseInput = new MouseInputWindows(this);
        mMouseOutput = new MouseOutputWindows(this);
        mKeyboardInput = new KeyboardInputWindows(this);
        mKeyboardOutput = new KeyboardOutputWindows(this);
#endif
#ifdef Q_OS_UNIX
        auto display = QX11Info::display();

        int unused;
        if(!display || !XTestQueryExtension(display, &unused, &unused, &unused, &unused))
        {
            // TODO: display a messagebox?
            return;
        }

        mMouseInput = new MouseInputX11(this);
        mMouseOutput = new MouseOutputX11(this);
        mKeyboardInput = new KeyboardInputX11(this);
        mKeyboardOutput = new KeyboardOutputX11(this);

        KeySymHelper::loadKeyCodes();
#endif
        if(mBackend)
            qFatal("Global backend already set");

        mBackend = this;
    }

    Backend::~Backend()
    {
    }

    void Backend::releaseAll()
    {
        // TODO
    }

    MouseInput &Backend::mouseInput()
    {
        Q_ASSERT(mMouseInput);

        return *mMouseInput;
    }

    MouseOutput &Backend::mouseOutput()
    {
        Q_ASSERT(mMouseOutput);

        return *mMouseOutput;
    }

    KeyboardInput &Backend::keyboardInput()
    {
        Q_ASSERT(mKeyboardInput);

        return *mKeyboardInput;
    }

    KeyboardOutput &Backend::keyboardOutput()
    {
        Q_ASSERT(mKeyboardOutput);

        return *mKeyboardOutput;
    }

    Backend &Backend::instance()
    {
        return *mBackend;
    }
}
