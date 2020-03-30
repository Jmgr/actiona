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

#include "actiontools/keyboardkey.hpp"
#include "actiontools/swappairs.hpp"

#include <map>
#include <array>

#include <Windows.h>

namespace ActionTools
{
    using StandardKey = KeyboardKey::StandardKey;

    static std::map<ushort, unsigned int> unicodeCharToVirtualKey;
    static std::map<unsigned int, ushort> virtualKeyToUnicodeChar;

    static const std::map<StandardKey, unsigned int> standardKeyToVirtualKey =
    {
        { StandardKey::Backspace,          VK_BACK },
        { StandardKey::Tabulation,         VK_TAB },
        { StandardKey::Clear,              VK_CLEAR },
        { StandardKey::Enter,              VK_RETURN },
        { StandardKey::LeftShift,          VK_LSHIFT },
        { StandardKey::RightShift,         VK_RSHIFT },
        { StandardKey::LeftControl,        VK_LCONTROL },
        { StandardKey::RightControl,       VK_RCONTROL },
        { StandardKey::LeftAlt,            VK_LMENU },
        { StandardKey::RightAlt,           VK_RMENU },
        { StandardKey::Pause,              VK_PAUSE },
        { StandardKey::CapsLock,           VK_CAPITAL },
        { StandardKey::Escape,             VK_ESCAPE },
        { StandardKey::Space,              VK_SPACE },
        { StandardKey::Prior,              VK_PRIOR },
        { StandardKey::Next,               VK_NEXT },
        { StandardKey::End,                VK_END },
        { StandardKey::Home,               VK_HOME },
        { StandardKey::LeftArrow,          VK_LEFT },
        { StandardKey::UpArrow,            VK_UP },
        { StandardKey::RightArrow,         VK_RIGHT },
        { StandardKey::DownArrow,          VK_DOWN },
        { StandardKey::Select,             VK_SELECT },
        { StandardKey::Execute,            VK_EXECUTE },
        { StandardKey::Snapshot,           VK_SNAPSHOT },
        { StandardKey::Insert,             VK_INSERT },
        { StandardKey::Delete,             VK_DELETE },
        { StandardKey::Help,               VK_HELP },
        { StandardKey::LeftWindows,        VK_LWIN },
        { StandardKey::RightWindows,       VK_RWIN },
        { StandardKey::Applications,       VK_APPS },
        { StandardKey::Sleep,              VK_SLEEP },
        { StandardKey::Numpad0,            VK_NUMPAD0 },
        { StandardKey::Numpad1,            VK_NUMPAD1 },
        { StandardKey::Numpad2,            VK_NUMPAD2 },
        { StandardKey::Numpad3,            VK_NUMPAD3 },
        { StandardKey::Numpad4,            VK_NUMPAD4 },
        { StandardKey::Numpad5,            VK_NUMPAD5 },
        { StandardKey::Numpad6,            VK_NUMPAD6 },
        { StandardKey::Numpad7,            VK_NUMPAD7 },
        { StandardKey::Numpad8,            VK_NUMPAD8 },
        { StandardKey::Numpad9,            VK_NUMPAD9 },
        { StandardKey::Multiply,           VK_MULTIPLY },
        { StandardKey::Add,                VK_ADD },
        { StandardKey::Separator,          VK_SEPARATOR },
        { StandardKey::Subtract,           VK_SUBTRACT },
        { StandardKey::Decimal,            VK_DECIMAL },
        { StandardKey::Divide,             VK_DIVIDE },
        { StandardKey::NumpadEnter,        VK_RETURN },
        { StandardKey::NumpadDelete,       VK_DELETE },
        { StandardKey::F1,                 VK_F1 },
        { StandardKey::F2,                 VK_F2 },
        { StandardKey::F3,                 VK_F3 },
        { StandardKey::F4,                 VK_F4 },
        { StandardKey::F5,                 VK_F5 },
        { StandardKey::F6,                 VK_F6 },
        { StandardKey::F7,                 VK_F7 },
        { StandardKey::F8,                 VK_F8 },
        { StandardKey::F9,                 VK_F9 },
        { StandardKey::F10,                VK_F10 },
        { StandardKey::F11,                VK_F11 },
        { StandardKey::F12,                VK_F12 },
        { StandardKey::F13,                VK_F13 },
        { StandardKey::F14,                VK_F14 },
        { StandardKey::F15,                VK_F15 },
        { StandardKey::F16,                VK_F16 },
        { StandardKey::F17,                VK_F17 },
        { StandardKey::F18,                VK_F18 },
        { StandardKey::F19,                VK_F19 },
        { StandardKey::F20,                VK_F20 },
        { StandardKey::F21,                VK_F21 },
        { StandardKey::F22,                VK_F22 },
        { StandardKey::F23,                VK_F23 },
        { StandardKey::F24,                VK_F24 },
        { StandardKey::NumLock,            VK_NUMLOCK },
        { StandardKey::ScrollLock,         VK_SCROLL },
        { StandardKey::BrowserBack,        VK_BROWSER_BACK },
        { StandardKey::BrowserForward,     VK_BROWSER_FORWARD },
        { StandardKey::BrowserRefresh,     VK_BROWSER_REFRESH },
        { StandardKey::BrowserStop,        VK_BROWSER_STOP },
        { StandardKey::BrowserSearch,      VK_BROWSER_SEARCH },
        { StandardKey::BrowserFavorites,   VK_BROWSER_FAVORITES },
        { StandardKey::BrowserHome,        VK_BROWSER_HOME },
        { StandardKey::VolumeMute,         VK_VOLUME_MUTE },
        { StandardKey::VolumeDown,         VK_VOLUME_DOWN },
        { StandardKey::VolumeUp,           VK_VOLUME_UP },
        { StandardKey::MediaNextTrack,     VK_MEDIA_NEXT_TRACK },
        { StandardKey::MediaPreviousTrack, VK_MEDIA_PREV_TRACK },
        { StandardKey::MediaStop,          VK_MEDIA_STOP },
        { StandardKey::MediaPlayPause,     VK_MEDIA_PLAY_PAUSE },
        { StandardKey::LaunchMail,         VK_LAUNCH_MAIL },
        { StandardKey::LaunchMediaSelect,  VK_LAUNCH_MEDIA_SELECT },
        { StandardKey::LaunchApp1,         VK_LAUNCH_APP1 },
        { StandardKey::LaunchApp2,         VK_LAUNCH_APP2 },
        { StandardKey::Play,               VK_PLAY },
    };

    static std::map<unsigned int, StandardKey> virtualKeyToStandardKey;

    bool isKeyPressed(unsigned int key)
    {
        return GetAsyncKeyState(key) & 0x8000;
    }

    bool KeyboardKey::isPressed() const
    {
        Q_ASSERT(isValid());

        switch(mKeyType)
        {
        case KeyType::Invalid:
            return false;
        case KeyType::Standard:
        {
            if(mStandardKey == StandardKey::AltGr)
                return isKeyPressed(VK_RMENU) && isKeyPressed(VK_LCONTROL);

            return isKeyPressed(standardKeyToVirtualKey.at(mStandardKey));
        }
        case KeyType::Native:
            return isKeyPressed(mNativeKey);
        case KeyType::Character:
            auto it = unicodeCharToVirtualKey.find(static_cast<unsigned short>(mNativeKey));
            if(it != unicodeCharToVirtualKey.cend())
                return isKeyPressed(it->second);
            break;
        }

        return false;
    }

    QList<KeyboardKey> KeyboardKey::findPressedKeys()
    {
        initialize();

        QList<KeyboardKey> result;

        // Standard keys
        for(const auto &standardKeyAndVirtualKey: standardKeyToVirtualKey)
        {
            if(isKeyPressed(standardKeyAndVirtualKey.second))
            {
                result.append(standardKeyAndVirtualKey.first);
            }
        }

        // Unicode chars
        for(const auto &unicodeCharAndVirtualKey: unicodeCharToVirtualKey)
        {
            if(isKeyPressed(unicodeCharAndVirtualKey.second))
            {
                result.append(KeyboardKey{QChar{unicodeCharAndVirtualKey.first}});
            }
        }

        return result;
    }

    void KeyboardKey::initialize()
    {
        if(mInitializationDone)
            return;

        mInitializationDone = true;

        virtualKeyToStandardKey = swapPairs(standardKeyToVirtualKey);

        std::array<BYTE, 256> keys;
        GetKeyboardState(keys.data());

        std::array<wchar_t, 10> buff;

        for(int virtualKey = 0; virtualKey < 256; ++virtualKey)
        {
            auto scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);
            auto result = ToUnicode(virtualKey, scanCode, keys.data(), buff.data(), static_cast<int>(buff.size()), 0);

            if(result < 0)
            {
                auto scanCode = MapVirtualKey(VK_SPACE, MAPVK_VK_TO_VSC);
                ToUnicode(VK_SPACE, scanCode, keys.data(), buff.data(), static_cast<int>(buff.size()), 0);

                continue;
            }

            if(result == 0)
                continue;

            if(virtualKeyToStandardKey.find(virtualKey) != virtualKeyToStandardKey.cend())
                continue;

            unicodeCharToVirtualKey.emplace(QChar{buff[0]}.unicode(), virtualKey);
        }

        virtualKeyToUnicodeChar = swapPairs(unicodeCharToVirtualKey);
    }

    KeyboardKey KeyboardKey::fromNativeKey(unsigned int virtualKey, unsigned int scanCode, const QString &text)
    {
        Q_UNUSED(text)

        switch(virtualKey)
        {
        case VK_SHIFT:
        case VK_CONTROL:
        case VK_MENU:
        {
            auto leftRightVirtualKey = MapVirtualKeyEx(scanCode, MAPVK_VSC_TO_VK_EX, nullptr);

            if(leftRightVirtualKey)
                virtualKey = leftRightVirtualKey;
            else
                return {};
        }
        }

        if(virtualKey == VK_RMENU || virtualKey == VK_LCONTROL)
        {
            if(isKeyPressed(VK_RMENU) && isKeyPressed(VK_LCONTROL))
                return {};
        }

        {
            auto it = virtualKeyToStandardKey.find(virtualKey);
            if(it != virtualKeyToStandardKey.cend())
                return it->second;
        }

        {
            auto it = virtualKeyToUnicodeChar.find(virtualKey);
            if(it != virtualKeyToUnicodeChar.cend())
                return KeyboardKey{QChar{it->second}};
        }

        return KeyboardKey{virtualKey};
    }

    QString KeyboardKey::nativeKeyName(unsigned int key)
    {
        int scanCode = MapVirtualKey(key, MAPVK_VK_TO_VSC_EX);
        if(scanCode == 0)
            return {};

        wchar_t name[256];
        int result = GetKeyNameText(scanCode << 16, name, 256);
        if(result == 0)
            return {};

        return QString::fromWCharArray(name, result);
    }
}
