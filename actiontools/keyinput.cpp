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

#include "keyinput.h"

#include <QKeyEvent>
#include <QKeySequence>
#include <QDebug>

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#include <QX11Info>
#endif
#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace ActionTools
{
	const StringListPair KeyInput::mKeyNames = qMakePair(
		QStringList() << "invalid" << "shiftLeft" << "shiftRight" << "controlLeft" << "controlRight" << "altLeft" << "altRight" << "metaLeft" << "metaRight" << "altGr",
		QStringList() << QObject::tr("Invalid") << QObject::tr("Left Shift") << QObject::tr("Right Shift") << QObject::tr("Left Control") << QObject::tr("Right Control")
		<< QObject::tr("Left Alt") << QObject::tr("Right Alt")
#ifdef Q_WS_WIN
		<< QObject::tr("Left Windows") << QObject::tr("Right Windows")
#else
		<< QObject::tr("Left Meta") << QObject::tr("Right Meta")
#endif
		<< QObject::tr("Alt Gr")
	);
	bool KeyInput::mInitDone = false;
	unsigned long KeyInput::mNativeKey[] = {0};

	KeyInput::KeyInput()
		: mIsQtKey(false),
		mKey(InvalidKey)
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

	bool KeyInput::fromPortableText(const QString &key, bool isQtKey)
	{
		mIsQtKey = isQtKey;

		if(mIsQtKey)
		{
			QKeySequence keySequence(key);

			mKey = keySequence[0];
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

#ifdef Q_WS_X11
		for(int i = 0; i < KeyCount; ++i)
		{
			if(event->nativeVirtualKey() == mNativeKey[i])
			{
				mKey = static_cast<Key>(i);
				mIsQtKey = false;
				break;
			}
		}
#endif

		switch(event->key())
		{
#ifdef Q_WS_X11
		case Qt::Key_AltGr:
			mKey = AltGr;
			mIsQtKey = false;
			break;
#endif
#ifdef Q_WS_WIN
		case Qt::Key_Shift:
		case Qt::Key_Control:
		case Qt::Key_Alt:
		case Qt::Key_Meta:
		case Qt::Key_AltGr:
			for(int i = 0; i < KeyCount; ++i)
			{
				if(HIBYTE(GetAsyncKeyState(mNativeKey[i])))
				{
					if(HIBYTE(GetAsyncKeyState(VK_RMENU)) && HIBYTE(GetAsyncKeyState(VK_LCONTROL)))
						mKey = AltGr;
					else
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
			mKey = event->key();

		return true;
	}

	void KeyInput::init()
	{
		if(mInitDone)
			return;

		mInitDone = true;

		mNativeKey[InvalidKey] = 0;

#ifdef Q_WS_X11
		mNativeKey[ShiftLeft] = XStringToKeysym("Shift_L");
		mNativeKey[ShiftRight] = XStringToKeysym("Shift_R");
		mNativeKey[ControlLeft] = XStringToKeysym("Control_L");
		mNativeKey[ControlRight] = XStringToKeysym("Control_R");
		mNativeKey[AltLeft] = XStringToKeysym("Alt_L");
		mNativeKey[AltRight] = XStringToKeysym("Alt_R");
		mNativeKey[MetaLeft] = XStringToKeysym("Super_L");//Hm, meta should be the Windows key...
		mNativeKey[MetaRight] = XStringToKeysym("Super_R");
		mNativeKey[AltGr] = 0;
#endif
#ifdef Q_WS_WIN
		mNativeKey[ShiftLeft] = VK_LSHIFT;
		mNativeKey[ShiftRight] = VK_RSHIFT;
		mNativeKey[ControlLeft] = VK_LCONTROL;
		mNativeKey[ControlRight] = VK_RCONTROL;
		mNativeKey[AltLeft] = VK_LMENU;
		mNativeKey[AltRight] = VK_RMENU;
		mNativeKey[MetaLeft] = VK_LWIN;
		mNativeKey[MetaRight] = VK_RWIN;
		mNativeKey[AltGr] = 0;
#endif
	}
}
