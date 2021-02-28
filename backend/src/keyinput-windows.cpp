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

#include "backend/keyinput.hpp"
#include "backend/keymapper.hpp"

#include <QKeyEvent>

#include <Windows.h>

namespace Backend
{
    void KeyInput::platformFromEvent(QKeyEvent *event)
    {
        switch(event->key())
        {
        case Qt::Key_Shift:
        case Qt::Key_Control:
        case Qt::Key_Alt:
        case Qt::Key_Meta:
        case Qt::Key_AltGr:
            for(int i = 1; i < KeyCount; ++i)
            {
                if(HIBYTE(GetAsyncKeyState(mNativeKey[i])))
                {
                    mKey = static_cast<Key>(i);

                    mIsQtKey = false;
                    break;
                }
            }
            break;
        default:
            break;
        }
    }

    void KeyInput::platformInit()
	{
		mNativeKey[ShiftLeft] = VK_LSHIFT;
		mNativeKey[ShiftRight] = VK_RSHIFT;
		mNativeKey[ControlLeft] = VK_LCONTROL;
		mNativeKey[ControlRight] = VK_RCONTROL;
		mNativeKey[AltLeft] = VK_LMENU;
		mNativeKey[AltRight] = VK_RMENU;
		mNativeKey[MetaLeft] = VK_LWIN;
		mNativeKey[MetaRight] = VK_RWIN;
		mNativeKey[AltGr] = 0;
		mNativeKey[Numpad0] = VK_NUMPAD0;
		mNativeKey[Numpad1] = VK_NUMPAD1;
		mNativeKey[Numpad2] = VK_NUMPAD2;
		mNativeKey[Numpad3] = VK_NUMPAD3;
		mNativeKey[Numpad4] = VK_NUMPAD4;
		mNativeKey[Numpad5] = VK_NUMPAD5;
		mNativeKey[Numpad6] = VK_NUMPAD6;
		mNativeKey[Numpad7] = VK_NUMPAD7;
		mNativeKey[Numpad8] = VK_NUMPAD8;
		mNativeKey[Numpad9] = VK_NUMPAD9;
		mNativeKey[NumpadMultiply] = VK_MULTIPLY;
		mNativeKey[NumpadAdd] = VK_ADD;
		mNativeKey[NumpadSeparator] = VK_SEPARATOR;
		mNativeKey[NumpadSubstract] = VK_SUBTRACT;
		mNativeKey[NumpadDecimal] = VK_DECIMAL;
		mNativeKey[NumpadDivide] = VK_DIVIDE;
	}
}
