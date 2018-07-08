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

#include "keyboardkey.hpp"
#include "swappairs.hpp"
#include "keysym2ucs.h"

#include <map>

#include <QtX11Extras/QX11Info>

#include <xkbcommon/xkbcommon-keysyms.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

namespace ActionTools
{
    using StandardKey = KeyboardKey::StandardKey;

    static const std::map<StandardKey, unsigned int> standardKeyToKeySym =
    {
        { StandardKey::Backspace,          XKB_KEY_BackSpace },
        { StandardKey::Tabulation,         XKB_KEY_Tab },
        { StandardKey::Clear,              XKB_KEY_Clear },
        { StandardKey::Enter,              XKB_KEY_Return },
        { StandardKey::LeftShift,          XKB_KEY_Shift_L },
        { StandardKey::RightShift,         XKB_KEY_Shift_R },
        { StandardKey::LeftControl,        XKB_KEY_Control_L },
        { StandardKey::RightControl,       XKB_KEY_Control_R },
        { StandardKey::LeftAlt,            XKB_KEY_Alt_L },
        { StandardKey::RightAlt,           XKB_KEY_Alt_R },
        { StandardKey::AltGr,              XKB_KEY_ISO_Level3_Shift },
        { StandardKey::Pause,              XKB_KEY_Pause },
        { StandardKey::CapsLock,           XKB_KEY_Caps_Lock },
        { StandardKey::Escape,             XKB_KEY_Escape },
        { StandardKey::Space,              XKB_KEY_space },
        { StandardKey::Prior,              XKB_KEY_Page_Up },
        { StandardKey::Next,               XKB_KEY_Page_Down },
        { StandardKey::End,                XKB_KEY_End },
        { StandardKey::Home,               XKB_KEY_Home },
        { StandardKey::LeftArrow,          XKB_KEY_Left },
        { StandardKey::UpArrow,            XKB_KEY_Up },
        { StandardKey::RightArrow,         XKB_KEY_Right },
        { StandardKey::DownArrow,          XKB_KEY_Down },
        { StandardKey::Select,             XKB_KEY_Select },
        { StandardKey::Execute,            XKB_KEY_Execute },
        { StandardKey::Snapshot,           XKB_KEY_Print },
        { StandardKey::Insert,             XKB_KEY_Insert },
        { StandardKey::Delete,             XKB_KEY_Delete },
        { StandardKey::Help,               XKB_KEY_Help },
        { StandardKey::LeftWindows,        XKB_KEY_Super_L },
        { StandardKey::RightWindows,       XKB_KEY_Super_R },
        { StandardKey::Applications,       XKB_KEY_Menu },
        { StandardKey::Sleep,              XKB_KEY_XF86Sleep },
        { StandardKey::Numpad0,            XKB_KEY_KP_0 },
        { StandardKey::Numpad1,            XKB_KEY_KP_1 },
        { StandardKey::Numpad2,            XKB_KEY_KP_2 },
        { StandardKey::Numpad3,            XKB_KEY_KP_3 },
        { StandardKey::Numpad4,            XKB_KEY_KP_4 },
        { StandardKey::Numpad5,            XKB_KEY_KP_5 },
        { StandardKey::Numpad6,            XKB_KEY_KP_6 },
        { StandardKey::Numpad7,            XKB_KEY_KP_7 },
        { StandardKey::Numpad8,            XKB_KEY_KP_8 },
        { StandardKey::Numpad9,            XKB_KEY_KP_9 },
        { StandardKey::Multiply,           XKB_KEY_KP_Multiply },
        { StandardKey::Add,                XKB_KEY_KP_Add },
        { StandardKey::Separator,          XKB_KEY_KP_Separator },
        { StandardKey::Subtract,           XKB_KEY_KP_Subtract },
        { StandardKey::Decimal,            XKB_KEY_KP_Decimal },
        { StandardKey::Divide,             XKB_KEY_KP_Divide },
        { StandardKey::NumpadEnter,        XKB_KEY_KP_Enter },
        { StandardKey::NumpadDelete,       XKB_KEY_KP_Delete },
        { StandardKey::NumpadPrior,        XKB_KEY_KP_Prior },
        { StandardKey::NumpadNext,         XKB_KEY_KP_Next },
        { StandardKey::NumpadEnd,          XKB_KEY_KP_End },
        { StandardKey::NumpadHome,         XKB_KEY_KP_Home },
        { StandardKey::NumpadLeftArrow,    XKB_KEY_KP_Left },
        { StandardKey::NumpadUpArrow,      XKB_KEY_KP_Up },
        { StandardKey::NumpadRightArrow,   XKB_KEY_KP_Right },
        { StandardKey::NumpadDownArrow,    XKB_KEY_KP_Down },
        { StandardKey::NumpadInsert,       XKB_KEY_KP_Insert },
        { StandardKey::NumpadClear,        XKB_KEY_KP_Begin },
        { StandardKey::F1,                 XKB_KEY_F1 },
        { StandardKey::F2,                 XKB_KEY_F2 },
        { StandardKey::F3,                 XKB_KEY_F3 },
        { StandardKey::F4,                 XKB_KEY_F4 },
        { StandardKey::F5,                 XKB_KEY_F5 },
        { StandardKey::F6,                 XKB_KEY_F6 },
        { StandardKey::F7,                 XKB_KEY_F7 },
        { StandardKey::F8,                 XKB_KEY_F8 },
        { StandardKey::F9,                 XKB_KEY_F9 },
        { StandardKey::F10,                XKB_KEY_F10 },
        { StandardKey::F11,                XKB_KEY_F11 },
        { StandardKey::F12,                XKB_KEY_F12 },
        { StandardKey::F13,                XKB_KEY_F13 },
        { StandardKey::F14,                XKB_KEY_F14 },
        { StandardKey::F15,                XKB_KEY_F15 },
        { StandardKey::F16,                XKB_KEY_F16 },
        { StandardKey::F17,                XKB_KEY_F17 },
        { StandardKey::F18,                XKB_KEY_F18 },
        { StandardKey::F19,                XKB_KEY_F19 },
        { StandardKey::F20,                XKB_KEY_F20 },
        { StandardKey::F21,                XKB_KEY_F21 },
        { StandardKey::F22,                XKB_KEY_F22 },
        { StandardKey::F23,                XKB_KEY_F23 },
        { StandardKey::F24,                XKB_KEY_F24 },
        { StandardKey::NumLock,            XKB_KEY_Num_Lock },
        { StandardKey::ScrollLock,         XKB_KEY_Scroll_Lock },
        { StandardKey::BrowserBack,        XKB_KEY_XF86Back },
        { StandardKey::BrowserForward,     XKB_KEY_XF86Forward },
        { StandardKey::BrowserRefresh,     XKB_KEY_XF86Refresh },
        { StandardKey::BrowserStop,        XKB_KEY_XF86Stop },
        { StandardKey::BrowserSearch,      XKB_KEY_XF86Search },
        { StandardKey::BrowserFavorites,   XKB_KEY_XF86Favorites },
        { StandardKey::BrowserHome,        XKB_KEY_XF86HomePage },
        { StandardKey::VolumeMute,         XKB_KEY_XF86AudioMute },
        { StandardKey::VolumeDown,         XKB_KEY_XF86AudioLowerVolume },
        { StandardKey::VolumeUp,           XKB_KEY_XF86AudioRaiseVolume },
        { StandardKey::MediaNextTrack,     XKB_KEY_XF86AudioNext },
        { StandardKey::MediaPreviousTrack, XKB_KEY_XF86AudioPrev },
        { StandardKey::MediaStop,          XKB_KEY_XF86AudioStop },
        { StandardKey::MediaPlayPause,     XKB_KEY_XF86AudioPause },
        { StandardKey::LaunchMail,         XKB_KEY_XF86Mail },
        { StandardKey::LaunchMediaSelect,  XKB_KEY_XF86AudioMedia },
        { StandardKey::LaunchApp1,         XKB_KEY_XF86Launch0 },
        { StandardKey::LaunchApp2,         XKB_KEY_XF86Launch1 },
        { StandardKey::Play,               XKB_KEY_XF86AudioPlay },
    };

    static std::map<unsigned int, StandardKey> keySymToStandardKey;
    static std::map<ushort, unsigned int> unicodeCharToKeySym;

    bool KeyboardKey::isPressed() const
    {
        Q_ASSERT(isValid());

        int keyCode;

        switch(mKeyType)
        {
        case KeyType::Invalid:
            return false;
        case KeyType::Native:
            keyCode = XKeysymToKeycode(QX11Info::display(), mNativeKey);
            break;
        case KeyType::Standard:
            keyCode = XKeysymToKeycode(QX11Info::display(), standardKeyToKeySym.at(mStandardKey));
            break;
        case KeyType::Character:
        {
            QChar character{mNativeKey};
            char latin1Character = character.toLatin1();
            if(latin1Character >= 0x20)
                keyCode = XKeysymToKeycode(QX11Info::display(), static_cast<KeySym>(latin1Character));
            else
            {
                auto name = QStringLiteral("U%1").arg(QString::number(mNativeKey, 16));
                auto keySym = XStringToKeysym(name.toLocal8Bit().constData());

                if(keySym == NoSymbol)
                    return false;

                keyCode = XKeysymToKeycode(QX11Info::display(), keySym);
            }
        }
            break;
        }

        static char keymap[32];
        XQueryKeymap(QX11Info::display(), keymap);

        return keymap[keyCode / 8] & (1 << (keyCode % 8));
    }

    QList<KeyboardKey> KeyboardKey::findPressedKeys()
    {
        initialize();

        char keys[32];

        XQueryKeymap(QX11Info::display(), keys);

        QList<KeyboardKey> result;

        // Standard keys
        for(const auto &standardKeyAndKeySym: standardKeyToKeySym)
        {
            auto keySym = standardKeyAndKeySym.second;
            auto keyCode = XKeysymToKeycode(QX11Info::display(), keySym);

            if(keySym == XKB_KEY_ISO_Level3_Shift)
                keyCode = 108;

            if(keys[keyCode / 8] & (0x1 << (keyCode % 8)))
            {
                result.append(standardKeyAndKeySym.first);
            }
        }

        // Unicode chars
        for(const auto &unicodeCharAndKeySym: unicodeCharToKeySym)
        {
            auto keySym = unicodeCharAndKeySym.second;
            auto keyCode = XKeysymToKeycode(QX11Info::display(), keySym);

            if(keys[keyCode / 8] & (0x1 << (keyCode % 8)))
            {
                result.append(KeyboardKey{QChar{unicodeCharAndKeySym.first}});
            }
        }

        return result;
    }

    void KeyboardKey::initialize()
    {
        if(mInitializationDone)
            return;

        mInitializationDone = true;

        keySymToStandardKey = swapPairs(standardKeyToKeySym);

        auto keyboardMap = XkbGetMap(QX11Info::display(), XkbAllClientInfoMask, XkbUseCoreKbd);

        if(keyboardMap)
        {
            for(int keyCode = keyboardMap->min_key_code; keyCode <= keyboardMap->max_key_code; ++keyCode)
            {
                auto keySym = XkbKeycodeToKeysym(QX11Info::display(), keyCode, 0, 0);
                if(keySym == NoSymbol)
                    continue;
                if(keySymToStandardKey.find(static_cast<unsigned int>(keySym)) != keySymToStandardKey.cend())
                    continue;

                QChar character = (uint)keysym2ucs(keySym);
                if(character.isNull() || character.isNonCharacter())
                    continue;

                unicodeCharToKeySym.emplace(character.unicode(), keySym);
            }
        }

        XkbFreeClientMap(keyboardMap, 0, True);
    }

    KeyboardKey KeyboardKey::fromNativeKey(unsigned int virtualKey, unsigned int scanCode, const QString &text)
    {
        Q_UNUSED(scanCode)
        Q_UNUSED(text)

        // Transform the virtual key (KeySym) into a key code and then back again to remove the effect of any modifier
        auto keyCode = XKeysymToKeycode(QX11Info::display(), virtualKey);
        virtualKey = XkbKeycodeToKeysym(QX11Info::display(), keyCode, 0, 0);

        {
            auto it = keySymToStandardKey.find(virtualKey);
            if(it != keySymToStandardKey.cend())
                return it->second;
        }

        QChar character = static_cast<uint>(keysym2ucs(virtualKey));

        {
            auto it = unicodeCharToKeySym.find(character.unicode());
            if(it != unicodeCharToKeySym.cend())
                return KeyboardKey{character};
        }

        return KeyboardKey{virtualKey};
    }

    QString KeyboardKey::nativeKeyName(unsigned int key)
    {
        return QString::fromLocal8Bit(XKeysymToString(key));
    }
}
