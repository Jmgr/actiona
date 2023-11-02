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

#include "actiontools/keyinput.hpp"
#include "actiontools/keymapper.hpp"

#include <QKeyEvent>
#include <QKeySequence>
#include <QDebug>

#ifdef Q_OS_UNIX
#define XK_MISCELLANY
#define XK_LATIN1
#define XK_KOREAN
#define XK_XKB_KEYS
#include <X11/keysymdef.h>
#include <X11/XF86keysym.h>
#include "actiontools/x11info.hpp"
#endif
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

namespace ActionTools
{
    const Tools::StringListPair KeyInput::mKeyNames =
    {
        {
            QStringLiteral("invalid"),
            QStringLiteral("shiftLeft"),
            QStringLiteral("shiftRight"),
            QStringLiteral("controlLeft"),
            QStringLiteral("controlRight"),
            QStringLiteral("altLeft"),
            QStringLiteral("altRight"),
            QStringLiteral("metaLeft"),
            QStringLiteral("metaRight"),
            QStringLiteral("altGr"),
            QStringLiteral("numpad0"),
            QStringLiteral("numpad1"),
            QStringLiteral("numpad2"),
            QStringLiteral("numpad3"),
            QStringLiteral("numpad4"),
            QStringLiteral("numpad5"),
            QStringLiteral("numpad6"),
            QStringLiteral("numpad7"),
            QStringLiteral("numpad8"),
            QStringLiteral("numpad9"),
            QStringLiteral("numpadMultiply"),
            QStringLiteral("numpadAdd"),
            QStringLiteral("numpadSeparator"),
            QStringLiteral("numpadSubstract"),
            QStringLiteral("numpadDecimal"),
            QStringLiteral("numpadDivide")
        },
        {
            {},
            QObject::tr("Left Shift"),
            QObject::tr("Right Shift"),
            QObject::tr("Left Control"),
            QObject::tr("Right Control"),
            QObject::tr("Left Alt"),
            QObject::tr("Right Alt"),
            #ifdef Q_OS_WIN
            QObject::tr("Left Windows"),
            QObject::tr("Right Windows"),
            #else
            QObject::tr("Left Meta"),
            QObject::tr("Right Meta"),
            #endif
            QObject::tr("Alt Gr"),
            QObject::tr("Numpad 0"),
            QObject::tr("Numpad 1"),
            QObject::tr("Numpad 2"),
            QObject::tr("Numpad 3"),
            QObject::tr("Numpad 4"),
            QObject::tr("Numpad 5"),
            QObject::tr("Numpad 6"),
            QObject::tr("Numpad 7"),
            QObject::tr("Numpad 8"),
            QObject::tr("Numpad 9"),
            QObject::tr("Numpad *"),
            QObject::tr("Numpad +"),
            QObject::tr("Numpad Separator"),
            QObject::tr("Numpad -"),
            QObject::tr("Numpad ."),
            QObject::tr("Numpad /")
        }
    };

	bool KeyInput::mInitDone = false;
	unsigned long KeyInput::mNativeKey[] = {0};

	KeyInput::KeyInput()
	{
		init();
	}

	QString KeyInput::toTranslatedText() const
	{
		if(mIsQtKey)
		{
			QKeySequence keySequence(mKey);

			return keySequence.toString(QKeySequence::NativeText);
		}

		return mKeyNames.second[mKey];
	}

	QString KeyInput::toPortableText() const
	{
		if(mIsQtKey)
		{
			QKeySequence keySequence(mKey);

			return keySequence.toString(QKeySequence::PortableText);
		}

		return mKeyNames.first[mKey];
	}

	bool KeyInput::fromPortableText(const QString &key)
	{
		mIsQtKey = true;

		for(int i = 0; i < KeyCount; ++i)
		{
			if(mKeyNames.first[i] == key)
			{
				mKey = i;
				mIsQtKey = false;

				return true;
			}
		}

		QKeySequence keySequence(key);

        mKey = keySequence[0].toCombined();
		mKey &= ~(Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier);

		return true;
	}

	bool KeyInput::fromPortableText(const QString &key, bool isQtKey)
	{
		mIsQtKey = isQtKey;

		if(mIsQtKey)
		{
			QKeySequence keySequence(key);

            mKey = keySequence[0].toCombined();
			mKey &= ~(Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier);

			return true;
		}

		for(int i = 0; i < KeyCount; ++i)
		{
			if(mKeyNames.first[i] == key)
			{
				mKey = i;

				return true;
			}
		}

		return false;
	}

	bool KeyInput::fromEvent(QKeyEvent *event)
	{
		mIsQtKey = true;

		for(int i = 0; i < KeyCount; ++i)
		{
			if(event->nativeVirtualKey() == mNativeKey[i])
			{
				mKey = static_cast<Key>(i);
				mIsQtKey = false;
				break;
			}
		}

		switch(event->key())
		{
#ifdef Q_OS_WIN
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
#endif
		default:
			break;
		}

		if(mIsQtKey)
		{
			if(event->modifiers() != Qt::NoModifier)
				return false;

			mKey = event->key();

			if(!KeyMapper::toNativeKey(static_cast<Qt::Key>(mKey)))
				return false;
		}
		else
		{
			if(!nativeKey(mKey))
				return false;
		}

		return true;
	}

	void KeyInput::init()
	{
		if(mInitDone)
			return;

		mInitDone = true;

		mNativeKey[InvalidKey] = 0;

#ifdef Q_OS_UNIX
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
#endif
#ifdef Q_OS_WIN
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
#endif
	}
}
