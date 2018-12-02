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

	Contact : jmgr@jmgr.info
*/

#pragma once

#include "actiontools_global.h"

#ifdef Q_OS_UNIX
#include <X11/Xlib.h>

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT KeySymHelper
	{
	public:
		static void loadKeyCodes();
		static KeySym wcharToKeySym(wchar_t c);
		static int keySymToModifier(KeySym keySym);
		static KeyCode keySymToKeyCode(KeySym keySym);
		static const char *keyModifiers[];

		static const int NUM_KEY_MODIFIERS = 3;
		static const int MAP_SIZE = 12285;
		static const unsigned long MAX_KEYSYM = 65536;
		static const int MULTIKEY_MAP_SIZE = 1195;

		static const quint16 multikeyMapChar[];
		static const quint16 multikeyMapFirst[];
		static const quint16 multikeyMapSecond[];

	private:
		static const quint16 mWCharToKeySym[];
		static int mKeySymToModifier[MAX_KEYSYM];
		static KeyCode mKeySymToKeyCode[MAX_KEYSYM];
	};
}

#endif

