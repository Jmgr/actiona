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

#pragma once

#include "actiontools_global.hpp"
#include "tools/stringlistpair.hpp"

class QKeyEvent;

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT KeyInput
	{
	public:
		enum Key
		{
			InvalidKey,
			ShiftLeft,
			ShiftRight,
			ControlLeft,
			ControlRight,
			AltLeft,
			AltRight,
			MetaLeft,
			MetaRight,
			AltGr,
			Numpad0,
			Numpad1,
			Numpad2,
			Numpad3,
			Numpad4,
			Numpad5,
			Numpad6,
			Numpad7,
			Numpad8,
			Numpad9,
			NumpadMultiply,
			NumpadAdd,
			NumpadSeparator,
			NumpadSubstract,
			NumpadDecimal,
			NumpadDivide,

			KeyCount
		};

		KeyInput();

		QString toTranslatedText() const;
		QString toPortableText() const;

		bool fromPortableText(const QString &key);
		bool fromPortableText(const QString &key, bool isQtKey);
		bool fromEvent(QKeyEvent *event);

		bool isQtKey() const						{ return mIsQtKey; }
		int key() const								{ return mKey; }

		static void init();
		static unsigned long nativeKey(int key)		{ return mNativeKey[key]; }

	private:
        static const Tools::StringListPair mKeyNames;
		static bool mInitDone;
		static unsigned long mNativeKey[KeyCount];

		bool mIsQtKey{false};
		int mKey{InvalidKey};
	};
}

