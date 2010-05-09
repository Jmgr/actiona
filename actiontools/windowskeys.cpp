/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/
//Parts of this file are taken from the synergy project, see following text
/*
 * synergy -- mouse and keyboard sharing utility
 * Copyright (C) 2003 Chris Schoeneman
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file COPYING that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#if 0
#include "keytypes.h"

const aKey windowsKeyMapping[] =
{
	/* 0x000 */ { aKeyNone },		// reserved
	/* 0x001 */ { aKeyNone },		// VK_LBUTTON
	/* 0x002 */ { aKeyNone },		// VK_RBUTTON
	/* 0x003 */ { aKeyNone },		// VK_CANCEL
	/* 0x004 */ { aKeyNone },		// VK_MBUTTON
	/* 0x005 */ { aKeyNone },		// VK_XBUTTON1
	/* 0x006 */ { aKeyNone },		// VK_XBUTTON2
	/* 0x007 */ { aKeyNone },		// undefined
	/* 0x008 */ { aKeyBackSpace },	// VK_BACK
	/* 0x009 */ { aKeyTab },		// VK_TAB
	/* 0x00a */ { aKeyNone },		// undefined
	/* 0x00b */ { aKeyNone },		// undefined
	/* 0x00c */ { aKeyClear },		// VK_CLEAR
	/* 0x00d */ { aKeyReturn },		// VK_RETURN
	/* 0x00e */ { aKeyNone },		// undefined
	/* 0x00f */ { aKeyNone },		// undefined
	/* 0x010 */ { aKeyShift_L },	// VK_SHIFT
	/* 0x011 */ { aKeyControl_L },	// VK_CONTROL
	/* 0x012 */ { aKeyAlt_L },		// VK_MENU
	/* 0x013 */ { aKeyPause },		// VK_PAUSE
	/* 0x014 */ { aKeyCapsLock },	// VK_CAPITAL
	/* 0x015 */ { aKeyNone },		// VK_KANA
	/* 0x016 */ { aKeyNone },		// VK_HANGUL
	/* 0x017 */ { aKeyNone },		// VK_JUNJA
	/* 0x018 */ { aKeyNone },		// VK_FINAL
	/* 0x019 */ { aKeyZenkaku },	// VK_KANJI
	/* 0x01a */ { aKeyNone },		// undefined
	/* 0x01b */ { aKeyEscape },		// VK_ESCAPE
	/* 0x01c */ { aKeyHenkan },		// VK_CONVERT
	/* 0x01d */ { aKeyNone },		// VK_NONCONVERT
	/* 0x01e */ { aKeyNone },		// VK_ACCEPT
	/* 0x01f */ { aKeyNone },		// VK_MODECHANGE
	/* 0x020 */ { aKeyNone },		// VK_SPACE
	/* 0x021 */ { aKeyKP_PageUp },	// VK_PRIOR
	/* 0x022 */ { aKeyKP_PageDown },// VK_NEXT
	/* 0x023 */ { aKeyKP_End },		// VK_END
	/* 0x024 */ { aKeyKP_Home },	// VK_HOME
	/* 0x025 */ { aKeyKP_Left },	// VK_LEFT
	/* 0x026 */ { aKeyKP_Up },		// VK_UP
	/* 0x027 */ { aKeyKP_Right },	// VK_RIGHT
	/* 0x028 */ { aKeyKP_Down },	// VK_DOWN
	/* 0x029 */ { aKeySelect },		// VK_SELECT
	/* 0x02a */ { aKeyNone },		// VK_PRINT
	/* 0x02b */ { aKeyExecute },	// VK_EXECUTE
	/* 0x02c */ { aKeyPrint },		// VK_SNAPSHOT
	/* 0x02d */ { aKeyKP_Insert },	// VK_INSERT
	/* 0x02e */ { aKeyKP_Delete },	// VK_DELETE
	/* 0x02f */ { aKeyHelp },		// VK_HELP
	/* 0x030 */ { aKeyNone },		// VK_0
	/* 0x031 */ { aKeyNone },		// VK_1
	/* 0x032 */ { aKeyNone },		// VK_2
	/* 0x033 */ { aKeyNone },		// VK_3
	/* 0x034 */ { aKeyNone },		// VK_4
	/* 0x035 */ { aKeyNone },		// VK_5
	/* 0x036 */ { aKeyNone },		// VK_6
	/* 0x037 */ { aKeyNone },		// VK_7
	/* 0x038 */ { aKeyNone },		// VK_8
	/* 0x039 */ { aKeyNone },		// VK_9
	/* 0x03a */ { aKeyNone },		// undefined
	/* 0x03b */ { aKeyNone },		// undefined
	/* 0x03c */ { aKeyNone },		// undefined
	/* 0x03d */ { aKeyNone },		// undefined
	/* 0x03e */ { aKeyNone },		// undefined
	/* 0x03f */ { aKeyNone },		// undefined
	/* 0x040 */ { aKeyNone },		// undefined
	/* 0x041 */ { aKeyNone },		// VK_A
	/* 0x042 */ { aKeyNone },		// VK_B
	/* 0x043 */ { aKeyNone },		// VK_C
	/* 0x044 */ { aKeyNone },		// VK_D
	/* 0x045 */ { aKeyNone },		// VK_E
	/* 0x046 */ { aKeyNone },		// VK_F
	/* 0x047 */ { aKeyNone },		// VK_G
	/* 0x048 */ { aKeyNone },		// VK_H
	/* 0x049 */ { aKeyNone },		// VK_I
	/* 0x04a */ { aKeyNone },		// VK_J
	/* 0x04b */ { aKeyNone },		// VK_K
	/* 0x04c */ { aKeyNone },		// VK_L
	/* 0x04d */ { aKeyNone },		// VK_M
	/* 0x04e */ { aKeyNone },		// VK_N
	/* 0x04f */ { aKeyNone },		// VK_O
	/* 0x050 */ { aKeyNone },		// VK_P
	/* 0x051 */ { aKeyNone },		// VK_Q
	/* 0x052 */ { aKeyNone },		// VK_R
	/* 0x053 */ { aKeyNone },		// VK_S
	/* 0x054 */ { aKeyNone },		// VK_T
	/* 0x055 */ { aKeyNone },		// VK_U
	/* 0x056 */ { aKeyNone },		// VK_V
	/* 0x057 */ { aKeyNone },		// VK_W
	/* 0x058 */ { aKeyNone },		// VK_X
	/* 0x059 */ { aKeyNone },		// VK_Y
	/* 0x05a */ { aKeyNone },		// VK_Z
	/* 0x05b */ { aKeySuper_L },	// VK_LWIN
	/* 0x05c */ { aKeySuper_R },	// VK_RWIN
	/* 0x05d */ { aKeyMenu },		// VK_APPS
	/* 0x05e */ { aKeyNone },		// undefined
	/* 0x05f */ { aKeySleep },		// VK_SLEEP
	/* 0x060 */ { aKeyKP_0 },		// VK_NUMPAD0
	/* 0x061 */ { aKeyKP_1 },		// VK_NUMPAD1
	/* 0x062 */ { aKeyKP_2 },		// VK_NUMPAD2
	/* 0x063 */ { aKeyKP_3 },		// VK_NUMPAD3
	/* 0x064 */ { aKeyKP_4 },		// VK_NUMPAD4
	/* 0x065 */ { aKeyKP_5 },		// VK_NUMPAD5
	/* 0x066 */ { aKeyKP_6 },		// VK_NUMPAD6
	/* 0x067 */ { aKeyKP_7 },		// VK_NUMPAD7
	/* 0x068 */ { aKeyKP_8 },		// VK_NUMPAD8
	/* 0x069 */ { aKeyKP_9 },		// VK_NUMPAD9
	/* 0x06a */ { aKeyKP_Multiply },// VK_MULTIPLY
	/* 0x06b */ { aKeyKP_Add },		// VK_ADD
	/* 0x06c */ { aKeyKP_Separator },// VK_SEPARATOR
	/* 0x06d */ { aKeyKP_Subtract },// VK_SUBTRACT
	/* 0x06e */ { aKeyKP_Decimal },	// VK_DECIMAL
	/* 0x06f */ { aKeyNone },		// VK_DIVIDE
	/* 0x070 */ { aKeyF1 },			// VK_F1
	/* 0x071 */ { aKeyF2 },			// VK_F2
	/* 0x072 */ { aKeyF3 },			// VK_F3
	/* 0x073 */ { aKeyF4 },			// VK_F4
	/* 0x074 */ { aKeyF5 },			// VK_F5
	/* 0x075 */ { aKeyF6 },			// VK_F6
	/* 0x076 */ { aKeyF7 },			// VK_F7
	/* 0x077 */ { aKeyF8 },			// VK_F8
	/* 0x078 */ { aKeyF9 },			// VK_F9
	/* 0x079 */ { aKeyF10 },		// VK_F10
	/* 0x07a */ { aKeyF11 },		// VK_F11
	/* 0x07b */ { aKeyF12 },		// VK_F12
	/* 0x07c */ { aKeyF13 },		// VK_F13
	/* 0x07d */ { aKeyF14 },		// VK_F14
	/* 0x07e */ { aKeyF15 },		// VK_F15
	/* 0x07f */ { aKeyF16 },		// VK_F16
	/* 0x080 */ { aKeyF17 },		// VK_F17
	/* 0x081 */ { aKeyF18 },		// VK_F18
	/* 0x082 */ { aKeyF19 },		// VK_F19
	/* 0x083 */ { aKeyF20 },		// VK_F20
	/* 0x084 */ { aKeyF21 },		// VK_F21
	/* 0x085 */ { aKeyF22 },		// VK_F22
	/* 0x086 */ { aKeyF23 },		// VK_F23
	/* 0x087 */ { aKeyF24 },		// VK_F24
	/* 0x088 */ { aKeyNone },		// unassigned
	/* 0x089 */ { aKeyNone },		// unassigned
	/* 0x08a */ { aKeyNone },		// unassigned
	/* 0x08b */ { aKeyNone },		// unassigned
	/* 0x08c */ { aKeyNone },		// unassigned
	/* 0x08d */ { aKeyNone },		// unassigned
	/* 0x08e */ { aKeyNone },		// unassigned
	/* 0x08f */ { aKeyNone },		// unassigned
	/* 0x090 */ { aKeyNumLock },	// VK_NUMLOCK
	/* 0x091 */ { aKeyScrollLock },	// VK_SCROLL
	/* 0x092 */ { aKeyNone },		// unassigned
	/* 0x093 */ { aKeyNone },		// unassigned
	/* 0x094 */ { aKeyNone },		// unassigned
	/* 0x095 */ { aKeyNone },		// unassigned
	/* 0x096 */ { aKeyNone },		// unassigned
	/* 0x097 */ { aKeyNone },		// unassigned
	/* 0x098 */ { aKeyNone },		// unassigned
	/* 0x099 */ { aKeyNone },		// unassigned
	/* 0x09a */ { aKeyNone },		// unassigned
	/* 0x09b */ { aKeyNone },		// unassigned
	/* 0x09c */ { aKeyNone },		// unassigned
	/* 0x09d */ { aKeyNone },		// unassigned
	/* 0x09e */ { aKeyNone },		// unassigned
	/* 0x09f */ { aKeyNone },		// unassigned
	/* 0x0a0 */ { aKeyShift_L },	// VK_LSHIFT
	/* 0x0a1 */ { aKeyShift_R },	// VK_RSHIFT
	/* 0x0a2 */ { aKeyControl_L },	// VK_LCONTROL
	/* 0x0a3 */ { aKeyControl_R },	// VK_RCONTROL
	/* 0x0a4 */ { aKeyAlt_L },		// VK_LMENU
	/* 0x0a5 */ { aKeyAlt_R },		// VK_RMENU
	/* 0x0a6 */ { aKeyNone },		// VK_BROWSER_BACK
	/* 0x0a7 */ { aKeyNone },		// VK_BROWSER_FORWARD
	/* 0x0a8 */ { aKeyNone },		// VK_BROWSER_REFRESH
	/* 0x0a9 */ { aKeyNone },		// VK_BROWSER_STOP
	/* 0x0aa */ { aKeyNone },		// VK_BROWSER_SEARCH
	/* 0x0ab */ { aKeyNone },		// VK_BROWSER_FAVORITES
	/* 0x0ac */ { aKeyNone },		// VK_BROWSER_HOME
	/* 0x0ad */ { aKeyNone },		// VK_VOLUME_MUTE
	/* 0x0ae */ { aKeyNone },		// VK_VOLUME_DOWN
	/* 0x0af */ { aKeyNone },		// VK_VOLUME_UP
	/* 0x0b0 */ { aKeyNone },		// VK_MEDIA_NEXT_TRACK
	/* 0x0b1 */ { aKeyNone },		// VK_MEDIA_PREV_TRACK
	/* 0x0b2 */ { aKeyNone },		// VK_MEDIA_STOP
	/* 0x0b3 */ { aKeyNone },		// VK_MEDIA_PLAY_PAUSE
	/* 0x0b4 */ { aKeyNone },		// VK_LAUNCH_MAIL
	/* 0x0b5 */ { aKeyNone },		// VK_LAUNCH_MEDIA_SELECT
	/* 0x0b6 */ { aKeyNone },		// VK_LAUNCH_APP1
	/* 0x0b7 */ { aKeyNone },		// VK_LAUNCH_APP2
	/* 0x0b8 */ { aKeyNone },		// unassigned
	/* 0x0b9 */ { aKeyNone },		// unassigned
	/* 0x0ba */ { aKeyNone },		// OEM specific
	/* 0x0bb */ { aKeyNone },		// OEM specific
	/* 0x0bc */ { aKeyNone },		// OEM specific
	/* 0x0bd */ { aKeyNone },		// OEM specific
	/* 0x0be */ { aKeyNone },		// OEM specific
	/* 0x0bf */ { aKeyNone },		// OEM specific
	/* 0x0c0 */ { aKeyNone },		// OEM specific
	/* 0x0c1 */ { aKeyNone },		// unassigned
	/* 0x0c2 */ { aKeyNone },		// unassigned
	/* 0x0c3 */ { aKeyNone },		// unassigned
	/* 0x0c4 */ { aKeyNone },		// unassigned
	/* 0x0c5 */ { aKeyNone },		// unassigned
	/* 0x0c6 */ { aKeyNone },		// unassigned
	/* 0x0c7 */ { aKeyNone },		// unassigned
	/* 0x0c8 */ { aKeyNone },		// unassigned
	/* 0x0c9 */ { aKeyNone },		// unassigned
	/* 0x0ca */ { aKeyNone },		// unassigned
	/* 0x0cb */ { aKeyNone },		// unassigned
	/* 0x0cc */ { aKeyNone },		// unassigned
	/* 0x0cd */ { aKeyNone },		// unassigned
	/* 0x0ce */ { aKeyNone },		// unassigned
	/* 0x0cf */ { aKeyNone },		// unassigned
	/* 0x0d0 */ { aKeyNone },		// unassigned
	/* 0x0d1 */ { aKeyNone },		// unassigned
	/* 0x0d2 */ { aKeyNone },		// unassigned
	/* 0x0d3 */ { aKeyNone },		// unassigned
	/* 0x0d4 */ { aKeyNone },		// unassigned
	/* 0x0d5 */ { aKeyNone },		// unassigned
	/* 0x0d6 */ { aKeyNone },		// unassigned
	/* 0x0d7 */ { aKeyNone },		// unassigned
	/* 0x0d8 */ { aKeyNone },		// unassigned
	/* 0x0d9 */ { aKeyNone },		// unassigned
	/* 0x0da */ { aKeyNone },		// unassigned
	/* 0x0db */ { aKeyNone },		// OEM specific
	/* 0x0dc */ { aKeyNone },		// OEM specific
	/* 0x0dd */ { aKeyNone },		// OEM specific
	/* 0x0de */ { aKeyNone },		// OEM specific
	/* 0x0df */ { aKeyNone },		// OEM specific
	/* 0x0e0 */ { aKeyNone },		// OEM specific
	/* 0x0e1 */ { aKeyNone },		// OEM specific
	/* 0x0e2 */ { aKeyNone },		// OEM specific
	/* 0x0e3 */ { aKeyNone },		// OEM specific
	/* 0x0e4 */ { aKeyNone },		// OEM specific
	/* 0x0e5 */ { aKeyNone },		// unassigned
	/* 0x0e6 */ { aKeyNone },		// OEM specific
	/* 0x0e7 */ { aKeyNone },		// unassigned
	/* 0x0e8 */ { aKeyNone },		// unassigned
	/* 0x0e9 */ { aKeyNone },		// OEM specific
	/* 0x0ea */ { aKeyNone },		// OEM specific
	/* 0x0eb */ { aKeyNone },		// OEM specific
	/* 0x0ec */ { aKeyNone },		// OEM specific
	/* 0x0ed */ { aKeyNone },		// OEM specific
	/* 0x0ee */ { aKeyNone },		// OEM specific
	/* 0x0ef */ { aKeyNone },		// OEM specific
	/* 0x0f0 */ { aKeyNone },		// OEM specific
	/* 0x0f1 */ { aKeyNone },		// OEM specific
	/* 0x0f2 */ { aKeyNone },		// OEM specific
	/* 0x0f3 */ { aKeyNone },		// OEM specific
	/* 0x0f4 */ { aKeyNone },		// OEM specific
	/* 0x0f5 */ { aKeyNone },		// OEM specific
	/* 0x0f6 */ { aKeyNone },		// VK_ATTN
	/* 0x0f7 */ { aKeyNone },		// VK_CRSEL
	/* 0x0f8 */ { aKeyNone },		// VK_EXSEL
	/* 0x0f9 */ { aKeyNone },		// VK_EREOF
	/* 0x0fa */ { aKeyNone },		// VK_PLAY
	/* 0x0fb */ { aKeyNone },		// VK_ZOOM
	/* 0x0fc */ { aKeyNone },		// reserved
	/* 0x0fd */ { aKeyNone },		// VK_PA1
	/* 0x0fe */ { aKeyNone },		// VK_OEM_CLEAR
	/* 0x0ff */ { aKeyNone },		// reserved

	/* 0x100 */ { aKeyNone },		// reserved
	/* 0x101 */ { aKeyNone },		// VK_LBUTTON
	/* 0x102 */ { aKeyNone },		// VK_RBUTTON
	/* 0x103 */ { aKeyBreak },		// VK_CANCEL
	/* 0x104 */ { aKeyNone },		// VK_MBUTTON
	/* 0x105 */ { aKeyNone },		// VK_XBUTTON1
	/* 0x106 */ { aKeyNone },		// VK_XBUTTON2
	/* 0x107 */ { aKeyNone },		// undefined
	/* 0x108 */ { aKeyNone },		// VK_BACK
	/* 0x109 */ { aKeyNone },		// VK_TAB
	/* 0x10a */ { aKeyNone },		// undefined
	/* 0x10b */ { aKeyNone },		// undefined
	/* 0x10c */ { aKeyClear },		// VK_CLEAR
	/* 0x10d */ { aKeyKP_Enter },	// VK_RETURN
	/* 0x10e */ { aKeyNone },		// undefined
	/* 0x10f */ { aKeyNone },		// undefined
	/* 0x110 */ { aKeyShift_R },	// VK_SHIFT
	/* 0x111 */ { aKeyControl_R },	// VK_CONTROL
	/* 0x112 */ { aKeyAlt_R },		// VK_MENU
	/* 0x113 */ { aKeyNone },		// VK_PAUSE
	/* 0x114 */ { aKeyNone },		// VK_CAPITAL
	/* 0x115 */ { aKeyNone },		// VK_KANA
	/* 0x116 */ { aKeyNone },		// VK_HANGUL
	/* 0x117 */ { aKeyNone },		// VK_JUNJA
	/* 0x118 */ { aKeyNone },		// VK_FINAL
	/* 0x119 */ { aKeyNone },		// VK_KANJI
	/* 0x11a */ { aKeyNone },		// undefined
	/* 0x11b */ { aKeyNone },		// VK_ESCAPE
	/* 0x11c */ { aKeyNone },		// VK_CONVERT
	/* 0x11d */ { aKeyNone },		// VK_NONCONVERT
	/* 0x11e */ { aKeyNone },		// VK_ACCEPT
	/* 0x11f */ { aKeyNone },		// VK_MODECHANGE
	/* 0x120 */ { aKeyNone },		// VK_SPACE
	/* 0x121 */ { aKeyPageUp },		// VK_PRIOR
	/* 0x122 */ { aKeyPageDown },	// VK_NEXT
	/* 0x123 */ { aKeyEnd },		// VK_END
	/* 0x124 */ { aKeyHome },		// VK_HOME
	/* 0x125 */ { aKeyLeft },		// VK_LEFT
	/* 0x126 */ { aKeyUp },			// VK_UP
	/* 0x127 */ { aKeyRight },		// VK_RIGHT
	/* 0x128 */ { aKeyDown },		// VK_DOWN
	/* 0x129 */ { aKeySelect },		// VK_SELECT
	/* 0x12a */ { aKeyNone },		// VK_PRINT
	/* 0x12b */ { aKeyExecute },	// VK_EXECUTE
	/* 0x12c */ { aKeyPrint },		// VK_SNAPSHOT
	/* 0x12d */ { aKeyInsert },		// VK_INSERT
	/* 0x12e */ { aKeyDelete },		// VK_DELETE
	/* 0x12f */ { aKeyHelp },		// VK_HELP
	/* 0x130 */ { aKeyNone },		// VK_0
	/* 0x131 */ { aKeyNone },		// VK_1
	/* 0x132 */ { aKeyNone },		// VK_2
	/* 0x133 */ { aKeyNone },		// VK_3
	/* 0x134 */ { aKeyNone },		// VK_4
	/* 0x135 */ { aKeyNone },		// VK_5
	/* 0x136 */ { aKeyNone },		// VK_6
	/* 0x137 */ { aKeyNone },		// VK_7
	/* 0x138 */ { aKeyNone },		// VK_8
	/* 0x139 */ { aKeyNone },		// VK_9
	/* 0x13a */ { aKeyNone },		// undefined
	/* 0x13b */ { aKeyNone },		// undefined
	/* 0x13c */ { aKeyNone },		// undefined
	/* 0x13d */ { aKeyNone },		// undefined
	/* 0x13e */ { aKeyNone },		// undefined
	/* 0x13f */ { aKeyNone },		// undefined
	/* 0x140 */ { aKeyNone },		// undefined
	/* 0x141 */ { aKeyNone },		// VK_A
	/* 0x142 */ { aKeyNone },		// VK_B
	/* 0x143 */ { aKeyNone },		// VK_C
	/* 0x144 */ { aKeyNone },		// VK_D
	/* 0x145 */ { aKeyNone },		// VK_E
	/* 0x146 */ { aKeyNone },		// VK_F
	/* 0x147 */ { aKeyNone },		// VK_G
	/* 0x148 */ { aKeyNone },		// VK_H
	/* 0x149 */ { aKeyNone },		// VK_I
	/* 0x14a */ { aKeyNone },		// VK_J
	/* 0x14b */ { aKeyNone },		// VK_K
	/* 0x14c */ { aKeyNone },		// VK_L
	/* 0x14d */ { aKeyNone },		// VK_M
	/* 0x14e */ { aKeyNone },		// VK_N
	/* 0x14f */ { aKeyNone },		// VK_O
	/* 0x150 */ { aKeyNone },		// VK_P
	/* 0x151 */ { aKeyNone },		// VK_Q
	/* 0x152 */ { aKeyNone },		// VK_R
	/* 0x153 */ { aKeyNone },		// VK_S
	/* 0x154 */ { aKeyNone },		// VK_T
	/* 0x155 */ { aKeyNone },		// VK_U
	/* 0x156 */ { aKeyNone },		// VK_V
	/* 0x157 */ { aKeyNone },		// VK_W
	/* 0x158 */ { aKeyNone },		// VK_X
	/* 0x159 */ { aKeyNone },		// VK_Y
	/* 0x15a */ { aKeyNone },		// VK_Z
	/* 0x15b */ { aKeySuper_L },	// VK_LWIN
	/* 0x15c */ { aKeySuper_R },	// VK_RWIN
	/* 0x15d */ { aKeyMenu },		// VK_APPS
	/* 0x15e */ { aKeyNone },		// undefined
	/* 0x15f */ { aKeyNone },		// VK_SLEEP
	/* 0x160 */ { aKeyNone },		// VK_NUMPAD0
	/* 0x161 */ { aKeyNone },		// VK_NUMPAD1
	/* 0x162 */ { aKeyNone },		// VK_NUMPAD2
	/* 0x163 */ { aKeyNone },		// VK_NUMPAD3
	/* 0x164 */ { aKeyNone },		// VK_NUMPAD4
	/* 0x165 */ { aKeyNone },		// VK_NUMPAD5
	/* 0x166 */ { aKeyNone },		// VK_NUMPAD6
	/* 0x167 */ { aKeyNone },		// VK_NUMPAD7
	/* 0x168 */ { aKeyNone },		// VK_NUMPAD8
	/* 0x169 */ { aKeyNone },		// VK_NUMPAD9
	/* 0x16a */ { aKeyNone },		// VK_MULTIPLY
	/* 0x16b */ { aKeyNone },		// VK_ADD
	/* 0x16c */ { aKeyKP_Separator },// VK_SEPARATOR
	/* 0x16d */ { aKeyNone },		// VK_SUBTRACT
	/* 0x16e */ { aKeyNone },		// VK_DECIMAL
	/* 0x16f */ { aKeyKP_Divide },	// VK_DIVIDE
	/* 0x170 */ { aKeyNone },		// VK_F1
	/* 0x171 */ { aKeyNone },		// VK_F2
	/* 0x172 */ { aKeyNone },		// VK_F3
	/* 0x173 */ { aKeyNone },		// VK_F4
	/* 0x174 */ { aKeyNone },		// VK_F5
	/* 0x175 */ { aKeyNone },		// VK_F6
	/* 0x176 */ { aKeyNone },		// VK_F7
	/* 0x177 */ { aKeyNone },		// VK_F8
	/* 0x178 */ { aKeyNone },		// VK_F9
	/* 0x179 */ { aKeyNone },		// VK_F10
	/* 0x17a */ { aKeyNone },		// VK_F11
	/* 0x17b */ { aKeyNone },		// VK_F12
	/* 0x17c */ { aKeyF13 },		// VK_F13
	/* 0x17d */ { aKeyF14 },		// VK_F14
	/* 0x17e */ { aKeyF15 },		// VK_F15
	/* 0x17f */ { aKeyF16 },		// VK_F16
	/* 0x180 */ { aKeyF17 },		// VK_F17
	/* 0x181 */ { aKeyF18 },		// VK_F18
	/* 0x182 */ { aKeyF19 },		// VK_F19
	/* 0x183 */ { aKeyF20 },		// VK_F20
	/* 0x184 */ { aKeyF21 },		// VK_F21
	/* 0x185 */ { aKeyF22 },		// VK_F22
	/* 0x186 */ { aKeyF23 },		// VK_F23
	/* 0x187 */ { aKeyF24 },		// VK_F24
	/* 0x188 */ { aKeyNone },		// unassigned
	/* 0x189 */ { aKeyNone },		// unassigned
	/* 0x18a */ { aKeyNone },		// unassigned
	/* 0x18b */ { aKeyNone },		// unassigned
	/* 0x18c */ { aKeyNone },		// unassigned
	/* 0x18d */ { aKeyNone },		// unassigned
	/* 0x18e */ { aKeyNone },		// unassigned
	/* 0x18f */ { aKeyNone },		// unassigned
	/* 0x190 */ { aKeyNumLock },	// VK_NUMLOCK
	/* 0x191 */ { aKeyNone },		// VK_SCROLL
	/* 0x192 */ { aKeyNone },		// unassigned
	/* 0x193 */ { aKeyNone },		// unassigned
	/* 0x194 */ { aKeyNone },		// unassigned
	/* 0x195 */ { aKeyNone },		// unassigned
	/* 0x196 */ { aKeyNone },		// unassigned
	/* 0x197 */ { aKeyNone },		// unassigned
	/* 0x198 */ { aKeyNone },		// unassigned
	/* 0x199 */ { aKeyNone },		// unassigned
	/* 0x19a */ { aKeyNone },		// unassigned
	/* 0x19b */ { aKeyNone },		// unassigned
	/* 0x19c */ { aKeyNone },		// unassigned
	/* 0x19d */ { aKeyNone },		// unassigned
	/* 0x19e */ { aKeyNone },		// unassigned
	/* 0x19f */ { aKeyNone },		// unassigned
	/* 0x1a0 */ { aKeyShift_L },	// VK_LSHIFT
	/* 0x1a1 */ { aKeyShift_R },	// VK_RSHIFT
	/* 0x1a2 */ { aKeyControl_L },	// VK_LCONTROL
	/* 0x1a3 */ { aKeyControl_R },	// VK_RCONTROL
	/* 0x1a4 */ { aKeyAlt_L },		// VK_LMENU
	/* 0x1a5 */ { aKeyAlt_R },		// VK_RMENU
	/* 0x1a6 */ { aKeyWWWBack },	// VK_BROWSER_BACK
	/* 0x1a7 */ { aKeyWWWForward },	// VK_BROWSER_FORWARD
	/* 0x1a8 */ { aKeyWWWRefresh },	// VK_BROWSER_REFRESH
	/* 0x1a9 */ { aKeyWWWStop },	// VK_BROWSER_STOP
	/* 0x1aa */ { aKeyWWWSearch },	// VK_BROWSER_SEARCH
	/* 0x1ab */ { aKeyWWWFavorites },// VK_BROWSER_FAVORITES
	/* 0x1ac */ { aKeyWWWHome },	// VK_BROWSER_HOME
	/* 0x1ad */ { aKeyAudioMute },	// VK_VOLUME_MUTE
	/* 0x1ae */ { aKeyAudioDown },	// VK_VOLUME_DOWN
	/* 0x1af */ { aKeyAudioUp },	// VK_VOLUME_UP
	/* 0x1b0 */ { aKeyAudioNext },	// VK_MEDIA_NEXT_TRACK
	/* 0x1b1 */ { aKeyAudioPrev },	// VK_MEDIA_PREV_TRACK
	/* 0x1b2 */ { aKeyAudioStop },	// VK_MEDIA_STOP
	/* 0x1b3 */ { aKeyAudioPlay },	// VK_MEDIA_PLAY_PAUSE
	/* 0x1b4 */ { aKeyAppMail },	// VK_LAUNCH_MAIL
	/* 0x1b5 */ { aKeyAppMedia },	// VK_LAUNCH_MEDIA_SELECT
	/* 0x1b6 */ { aKeyAppUser1 },	// VK_LAUNCH_APP1
	/* 0x1b7 */ { aKeyAppUser2 },	// VK_LAUNCH_APP2
	/* 0x1b8 */ { aKeyNone },		// unassigned
	/* 0x1b9 */ { aKeyNone },		// unassigned
	/* 0x1ba */ { aKeyNone },		// OEM specific
	/* 0x1bb */ { aKeyNone },		// OEM specific
	/* 0x1bc */ { aKeyNone },		// OEM specific
	/* 0x1bd */ { aKeyNone },		// OEM specific
	/* 0x1be */ { aKeyNone },		// OEM specific
	/* 0x1bf */ { aKeyNone },		// OEM specific
	/* 0x1c0 */ { aKeyNone },		// OEM specific
	/* 0x1c1 */ { aKeyNone },		// unassigned
	/* 0x1c2 */ { aKeyNone },		// unassigned
	/* 0x1c3 */ { aKeyNone },		// unassigned
	/* 0x1c4 */ { aKeyNone },		// unassigned
	/* 0x1c5 */ { aKeyNone },		// unassigned
	/* 0x1c6 */ { aKeyNone },		// unassigned
	/* 0x1c7 */ { aKeyNone },		// unassigned
	/* 0x1c8 */ { aKeyNone },		// unassigned
	/* 0x1c9 */ { aKeyNone },		// unassigned
	/* 0x1ca */ { aKeyNone },		// unassigned
	/* 0x1cb */ { aKeyNone },		// unassigned
	/* 0x1cc */ { aKeyNone },		// unassigned
	/* 0x1cd */ { aKeyNone },		// unassigned
	/* 0x1ce */ { aKeyNone },		// unassigned
	/* 0x1cf */ { aKeyNone },		// unassigned
	/* 0x1d0 */ { aKeyNone },		// unassigned
	/* 0x1d1 */ { aKeyNone },		// unassigned
	/* 0x1d2 */ { aKeyNone },		// unassigned
	/* 0x1d3 */ { aKeyNone },		// unassigned
	/* 0x1d4 */ { aKeyNone },		// unassigned
	/* 0x1d5 */ { aKeyNone },		// unassigned
	/* 0x1d6 */ { aKeyNone },		// unassigned
	/* 0x1d7 */ { aKeyNone },		// unassigned
	/* 0x1d8 */ { aKeyNone },		// unassigned
	/* 0x1d9 */ { aKeyNone },		// unassigned
	/* 0x1da */ { aKeyNone },		// unassigned
	/* 0x1db */ { aKeyNone },		// OEM specific
	/* 0x1dc */ { aKeyNone },		// OEM specific
	/* 0x1dd */ { aKeyNone },		// OEM specific
	/* 0x1de */ { aKeyNone },		// OEM specific
	/* 0x1df */ { aKeyNone },		// OEM specific
	/* 0x1e0 */ { aKeyNone },		// OEM specific
	/* 0x1e1 */ { aKeyNone },		// OEM specific
	/* 0x1e2 */ { aKeyNone },		// OEM specific
	/* 0x1e3 */ { aKeyNone },		// OEM specific
	/* 0x1e4 */ { aKeyNone },		// OEM specific
	/* 0x1e5 */ { aKeyNone },		// unassigned
	/* 0x1e6 */ { aKeyNone },		// OEM specific
	/* 0x1e7 */ { aKeyNone },		// unassigned
	/* 0x1e8 */ { aKeyNone },		// unassigned
	/* 0x1e9 */ { aKeyNone },		// OEM specific
	/* 0x1ea */ { aKeyNone },		// OEM specific
	/* 0x1eb */ { aKeyNone },		// OEM specific
	/* 0x1ec */ { aKeyNone },		// OEM specific
	/* 0x1ed */ { aKeyNone },		// OEM specific
	/* 0x1ee */ { aKeyNone },		// OEM specific
	/* 0x1ef */ { aKeyNone },		// OEM specific
	/* 0x1f0 */ { aKeyNone },		// OEM specific
	/* 0x1f1 */ { aKeyNone },		// OEM specific
	/* 0x1f2 */ { aKeyNone },		// OEM specific
	/* 0x1f3 */ { aKeyNone },		// OEM specific
	/* 0x1f4 */ { aKeyNone },		// OEM specific
	/* 0x1f5 */ { aKeyNone },		// OEM specific
	/* 0x1f6 */ { aKeyNone },		// VK_ATTN
	/* 0x1f7 */ { aKeyNone },		// VK_CRSEL
	/* 0x1f8 */ { aKeyNone },		// VK_EXSEL
	/* 0x1f9 */ { aKeyNone },		// VK_EREOF
	/* 0x1fa */ { aKeyNone },		// VK_PLAY
	/* 0x1fb */ { aKeyNone },		// VK_ZOOM
	/* 0x1fc */ { aKeyNone },		// reserved
	/* 0x1fd */ { aKeyNone },		// VK_PA1
	/* 0x1fe */ { aKeyNone },		// VK_OEM_CLEAR
	/* 0x1ff */ { aKeyNone }		// reserved
};

aKey windowsKeyToAKey(int keycode)
{
	if(keycode < 0 || keycode > 0x1ff)
		return aKeyNone;

	return windowsKeyMapping[keycode];
}
#endif
