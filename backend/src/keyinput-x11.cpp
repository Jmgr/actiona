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

#define XK_MISCELLANY // TODO: remove
#define XK_LATIN1
#define XK_KOREAN
#define XK_XKB_KEYS
#include <X11/keysymdef.h>
#include <X11/XF86keysym.h>

namespace Backend
{
    void KeyInput::platformFromEvent(QKeyEvent *event)
    {
        Q_UNUSED(event)
    }

    void KeyInput::platformInit()
    {
        mNativeKey[ShiftLeft] = XK_Shift_L;
        mNativeKey[ShiftRight] = XK_Shift_R;
        mNativeKey[ControlLeft] = XK_Control_L;
        mNativeKey[ControlRight] = XK_Control_R;
        mNativeKey[AltLeft] = XK_Alt_L;
        mNativeKey[AltRight] = XK_Alt_R;
        mNativeKey[MetaLeft] = XK_Super_L;
        mNativeKey[MetaRight] = XK_Super_R;
        mNativeKey[AltGr] = XK_ISO_Level3_Shift;
        mNativeKey[Numpad0] = XK_KP_0;
        mNativeKey[Numpad1] = XK_KP_1;
        mNativeKey[Numpad2] = XK_KP_2;
        mNativeKey[Numpad3] = XK_KP_3;
        mNativeKey[Numpad4] = XK_KP_4;
        mNativeKey[Numpad5] = XK_KP_5;
        mNativeKey[Numpad6] = XK_KP_6;
        mNativeKey[Numpad7] = XK_KP_7;
        mNativeKey[Numpad8] = XK_KP_8;
        mNativeKey[Numpad9] = XK_KP_9;
        mNativeKey[NumpadMultiply] = XK_KP_Multiply;
        mNativeKey[NumpadAdd] = XK_KP_Add;
        mNativeKey[NumpadSeparator] = XK_KP_Separator;
        mNativeKey[NumpadSubstract] = XK_KP_Subtract;
        mNativeKey[NumpadDecimal] = XK_KP_Decimal;
        mNativeKey[NumpadDivide] = XK_KP_Divide;
    }
}
