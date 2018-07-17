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

#include <QString>
#include <QHash>

#include <functional>

class QKeyEvent;

namespace ActionTools
{
    class ACTIONTOOLSSHARED_EXPORT KeyboardKey final
    {
    public:
        enum class KeyType
        {
            Invalid,
            Standard,
            Native,
            Character
        };

        enum class StandardKey
        {
            Backspace,
            Tabulation,
            Clear,
            Enter,
            LeftShift,
            RightShift,
            LeftControl,
            RightControl,
            LeftAlt,
            RightAlt,
            AltGr,
            Pause,
            CapsLock,
            Escape,
            Space,
            Prior,
            Next,
            End,
            Home,
            LeftArrow,
            UpArrow,
            RightArrow,
            DownArrow,
            Select,
            Execute,
            Snapshot,
            Insert,
            Delete,
            Help,
            LeftWindows,
            RightWindows,
            Applications,
            Sleep,
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
            Multiply,
            Add,
            Separator,
            Subtract,
            Decimal,
            Divide,
            NumpadEnter,
            NumpadDelete,
            NumpadPrior,
            NumpadNext,
            NumpadEnd,
            NumpadHome,
            NumpadLeftArrow,
            NumpadUpArrow,
            NumpadRightArrow,
            NumpadDownArrow,
            NumpadInsert,
            NumpadClear,
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            F13,
            F14,
            F15,
            F16,
            F17,
            F18,
            F19,
            F20,
            F21,
            F22,
            F23,
            F24,
            NumLock,
            ScrollLock,
            BrowserBack,
            BrowserForward,
            BrowserRefresh,
            BrowserStop,
            BrowserSearch,
            BrowserFavorites,
            BrowserHome,
            VolumeMute,
            VolumeDown,
            VolumeUp,
            MediaNextTrack,
            MediaPreviousTrack,
            MediaStop,
            MediaPlayPause,
            LaunchMail,
            LaunchMediaSelect,
            LaunchApp1,
            LaunchApp2,
            Play,

            Count,
        };

        KeyboardKey();
        KeyboardKey(StandardKey standardKey);
        explicit KeyboardKey(unsigned int nativeKey);
        explicit KeyboardKey(QChar unicodeCharacter);
        explicit KeyboardKey(QKeyEvent *keyEvent);

        bool isValid() const;
        QString name() const;
        bool isPressed() const;
        KeyType keyType() const { return mKeyType; }
        StandardKey standardKey() const { Q_ASSERT(mKeyType == KeyType::Standard); return mStandardKey; }
        unsigned int nativeKey() const { Q_ASSERT(mKeyType == KeyType::Native); return mNativeKey; }
        QChar character() const { Q_ASSERT(mKeyType == KeyType::Character); return mNativeKey; }

        void save(std::function<void(const QString &, const QString &)> keyValueCallback) const;
        static KeyboardKey load(std::function<QString(const QString &)> keyValueCallback);

        static QList<KeyboardKey> loadKeyListFromJson(const QString &json);
        static QString saveKeyListToJson(const QList<KeyboardKey> &keyList);

        static QList<KeyboardKey> findPressedKeys();

    private:
        KeyboardKey(KeyType keyType, unsigned int key);

        static void initialize();

        static KeyboardKey fromNativeKey(unsigned int virtualKey, unsigned int scanCode, const QString &text = {});
        static QString nativeKeyName(unsigned int key);

        KeyType mKeyType{KeyType::Invalid};
        StandardKey mStandardKey{StandardKey::Backspace};
        unsigned int mNativeKey{0};

        static bool mInitializationDone;

        friend bool operator==(const KeyboardKey &first, const KeyboardKey &second);
        friend uint qHash(const KeyboardKey &key, uint seed);
    };

    inline bool operator==(const KeyboardKey &first, const KeyboardKey &second)
    {
        return first.mKeyType == second.mKeyType &&
                first.mStandardKey == second.mStandardKey &&
                first.mNativeKey == second.mNativeKey;
    }

    inline uint qHash(const KeyboardKey &key, uint seed)
    {
        return ::qHash(static_cast<int>(key.mKeyType), seed) ^ ::qHash(static_cast<unsigned int>(key.mStandardKey), seed) ^ ::qHash(key.mNativeKey, seed);
    }
}
