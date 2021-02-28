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
#include <QKeySequence>
#include <QStringList>

namespace Backend
{
    const std::pair<QStringList, QStringList> KeyInput::mKeyNames =
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

		mKey = keySequence[0];
		mKey &= ~(Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier);

		return true;
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

		for(int i = 0; i < KeyCount; ++i)
		{
			if(event->nativeVirtualKey() == mNativeKey[i])
			{
				mKey = static_cast<Key>(i);
				mIsQtKey = false;
				break;
			}
		}

        platformFromEvent(event);

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

        platformInit();
	}
}
