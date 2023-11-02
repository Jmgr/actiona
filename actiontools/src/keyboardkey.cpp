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

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QKeyEvent>

namespace ActionTools
{
    bool KeyboardKey::mInitializationDone{false};

    KeyboardKey::KeyboardKey()
    {
        initialize();
    }

    KeyboardKey::KeyboardKey(StandardKey standardKey):
        mKeyType(KeyType::Standard),
        mStandardKey(standardKey)
    {
        initialize();
    }

    KeyboardKey::KeyboardKey(unsigned int nativeKey):
        mKeyType(KeyType::Native),
        mNativeKey(nativeKey)
    {
        initialize();
    }

    KeyboardKey::KeyboardKey(QChar unicodeCharacter):
        mKeyType(KeyType::Character),
        mNativeKey(unicodeCharacter.unicode())
    {
        initialize();
    }

    KeyboardKey::KeyboardKey(KeyboardKey::KeyType keyType, unsigned int key):
        mKeyType(keyType)
    {
        initialize();

        switch(mKeyType)
        {
        case KeyType::Invalid:
            break;
        case KeyType::Standard:
            mStandardKey = static_cast<StandardKey>(key);
            break;
        case KeyType::Native:
        case KeyType::Character:
            mNativeKey = key;
            break;
        }
    }

    KeyboardKey::KeyboardKey(QKeyEvent *keyEvent)
    {
        initialize();

        *this = fromNativeKey(keyEvent->nativeVirtualKey(), keyEvent->nativeScanCode(), keyEvent->text().toLower());
    }

    bool KeyboardKey::isValid() const
    {
        return mKeyType != KeyType::Invalid;
    }

    QString KeyboardKey::name() const
    {
        switch(mKeyType)
        {
        case KeyType::Invalid:      return QCoreApplication::translate("KeyboardKey", "Invalid key");
        case KeyType::Native:       return QCoreApplication::translate("KeyboardKey", "Native key %1").arg(nativeKeyName(mNativeKey));
        case KeyType::Character:    return QChar{mNativeKey};
        case KeyType::Standard:
            switch(mStandardKey)
            {
            case StandardKey::Backspace:           return QCoreApplication::translate("KeyboardKey", "Backspace");
            case StandardKey::Tabulation:          return QCoreApplication::translate("KeyboardKey", "Tabulation");
            case StandardKey::Clear:               return QCoreApplication::translate("KeyboardKey", "Clear");
            case StandardKey::Enter:               return QCoreApplication::translate("KeyboardKey", "Enter");
            case StandardKey::LeftShift:           return QCoreApplication::translate("KeyboardKey", "Left Shift");
            case StandardKey::RightShift:          return QCoreApplication::translate("KeyboardKey", "Right Shift");
            case StandardKey::LeftControl:         return QCoreApplication::translate("KeyboardKey", "Left Control");
            case StandardKey::RightControl:        return QCoreApplication::translate("KeyboardKey", "Right Control");
            case StandardKey::LeftAlt:             return QCoreApplication::translate("KeyboardKey", "Left Alt");
            case StandardKey::RightAlt:            return QCoreApplication::translate("KeyboardKey", "Right Alt");
            case StandardKey::AltGr:               return QCoreApplication::translate("KeyboardKey", "AltGr");
            case StandardKey::Pause:               return QCoreApplication::translate("KeyboardKey", "Pause");
            case StandardKey::CapsLock:            return QCoreApplication::translate("KeyboardKey", "Caps Lock");
            case StandardKey::Escape:              return QCoreApplication::translate("KeyboardKey", "Escape");
            case StandardKey::Space:               return QCoreApplication::translate("KeyboardKey", "Space");
            case StandardKey::Prior:               return QCoreApplication::translate("KeyboardKey", "Prior");
            case StandardKey::Next:                return QCoreApplication::translate("KeyboardKey", "Next");
            case StandardKey::End:                 return QCoreApplication::translate("KeyboardKey", "End");
            case StandardKey::Home:                return QCoreApplication::translate("KeyboardKey", "Home");
            case StandardKey::LeftArrow:           return QCoreApplication::translate("KeyboardKey", "Left Arrow");
            case StandardKey::UpArrow:             return QCoreApplication::translate("KeyboardKey", "Up Arrow");
            case StandardKey::RightArrow:          return QCoreApplication::translate("KeyboardKey", "Right Arrow");
            case StandardKey::DownArrow:           return QCoreApplication::translate("KeyboardKey", "Down Arrow");
            case StandardKey::Select:              return QCoreApplication::translate("KeyboardKey", "Select");
            case StandardKey::Execute:             return QCoreApplication::translate("KeyboardKey", "Execute");
            case StandardKey::Snapshot:            return QCoreApplication::translate("KeyboardKey", "Snapshot");
            case StandardKey::Insert:              return QCoreApplication::translate("KeyboardKey", "Insert");
            case StandardKey::Delete:              return QCoreApplication::translate("KeyboardKey", "Delete");
            case StandardKey::Help:                return QCoreApplication::translate("KeyboardKey", "Help");
            case StandardKey::LeftWindows:         return QCoreApplication::translate("KeyboardKey", "Left Windows");
            case StandardKey::RightWindows:        return QCoreApplication::translate("KeyboardKey", "Right Windows");
            case StandardKey::Applications:        return QCoreApplication::translate("KeyboardKey", "Applications");
            case StandardKey::Sleep:               return QCoreApplication::translate("KeyboardKey", "Sleep");
            case StandardKey::Numpad0:             return QCoreApplication::translate("KeyboardKey", "Numpad 0");
            case StandardKey::Numpad1:             return QCoreApplication::translate("KeyboardKey", "Numpad 1");
            case StandardKey::Numpad2:             return QCoreApplication::translate("KeyboardKey", "Numpad 2");
            case StandardKey::Numpad3:             return QCoreApplication::translate("KeyboardKey", "Numpad 3");
            case StandardKey::Numpad4:             return QCoreApplication::translate("KeyboardKey", "Numpad 4");
            case StandardKey::Numpad5:             return QCoreApplication::translate("KeyboardKey", "Numpad 5");
            case StandardKey::Numpad6:             return QCoreApplication::translate("KeyboardKey", "Numpad 6");
            case StandardKey::Numpad7:             return QCoreApplication::translate("KeyboardKey", "Numpad 7");
            case StandardKey::Numpad8:             return QCoreApplication::translate("KeyboardKey", "Numpad 8");
            case StandardKey::Numpad9:             return QCoreApplication::translate("KeyboardKey", "Numpad 9");
            case StandardKey::Multiply:            return QCoreApplication::translate("KeyboardKey", "Multiply");
            case StandardKey::Add:                 return QCoreApplication::translate("KeyboardKey", "Add");
            case StandardKey::Separator:           return QCoreApplication::translate("KeyboardKey", "Separator");
            case StandardKey::Subtract:            return QCoreApplication::translate("KeyboardKey", "Subtract");
            case StandardKey::Decimal:             return QCoreApplication::translate("KeyboardKey", "Decimal");
            case StandardKey::Divide:              return QCoreApplication::translate("KeyboardKey", "Divide");
            case StandardKey::NumpadEnter:         return QCoreApplication::translate("KeyboardKey", "Numpad Enter");
            case StandardKey::NumpadDelete:        return QCoreApplication::translate("KeyboardKey", "Numpad Delete");
            case StandardKey::NumpadPrior:         return QCoreApplication::translate("KeyboardKey", "Numpad Prior");
            case StandardKey::NumpadNext:          return QCoreApplication::translate("KeyboardKey", "Numpad Next");
            case StandardKey::NumpadEnd:           return QCoreApplication::translate("KeyboardKey", "Numpad End");
            case StandardKey::NumpadHome:          return QCoreApplication::translate("KeyboardKey", "Numpad Home");
            case StandardKey::NumpadLeftArrow:     return QCoreApplication::translate("KeyboardKey", "Numpad Left Arrow");
            case StandardKey::NumpadUpArrow:       return QCoreApplication::translate("KeyboardKey", "Numpad Up Arrow");
            case StandardKey::NumpadRightArrow:    return QCoreApplication::translate("KeyboardKey", "Numpad Right Arrow");
            case StandardKey::NumpadDownArrow:     return QCoreApplication::translate("KeyboardKey", "Numpad Down Arrow");
            case StandardKey::NumpadInsert:        return QCoreApplication::translate("KeyboardKey", "Numpad Insert");
            case StandardKey::NumpadClear:         return QCoreApplication::translate("KeyboardKey", "Numpad Clear");
            case StandardKey::F1:                  return QCoreApplication::translate("KeyboardKey", "F1");
            case StandardKey::F2:                  return QCoreApplication::translate("KeyboardKey", "F2");
            case StandardKey::F3:                  return QCoreApplication::translate("KeyboardKey", "F3");
            case StandardKey::F4:                  return QCoreApplication::translate("KeyboardKey", "F4");
            case StandardKey::F5:                  return QCoreApplication::translate("KeyboardKey", "F5");
            case StandardKey::F6:                  return QCoreApplication::translate("KeyboardKey", "F6");
            case StandardKey::F7:                  return QCoreApplication::translate("KeyboardKey", "F7");
            case StandardKey::F8:                  return QCoreApplication::translate("KeyboardKey", "F8");
            case StandardKey::F9:                  return QCoreApplication::translate("KeyboardKey", "F9");
            case StandardKey::F10:                 return QCoreApplication::translate("KeyboardKey", "F10");
            case StandardKey::F11:                 return QCoreApplication::translate("KeyboardKey", "F11");
            case StandardKey::F12:                 return QCoreApplication::translate("KeyboardKey", "F12");
            case StandardKey::F13:                 return QCoreApplication::translate("KeyboardKey", "F13");
            case StandardKey::F14:                 return QCoreApplication::translate("KeyboardKey", "F14");
            case StandardKey::F15:                 return QCoreApplication::translate("KeyboardKey", "F15");
            case StandardKey::F16:                 return QCoreApplication::translate("KeyboardKey", "F16");
            case StandardKey::F17:                 return QCoreApplication::translate("KeyboardKey", "F17");
            case StandardKey::F18:                 return QCoreApplication::translate("KeyboardKey", "F18");
            case StandardKey::F19:                 return QCoreApplication::translate("KeyboardKey", "F19");
            case StandardKey::F20:                 return QCoreApplication::translate("KeyboardKey", "F20");
            case StandardKey::F21:                 return QCoreApplication::translate("KeyboardKey", "F21");
            case StandardKey::F22:                 return QCoreApplication::translate("KeyboardKey", "F22");
            case StandardKey::F23:                 return QCoreApplication::translate("KeyboardKey", "F23");
            case StandardKey::F24:                 return QCoreApplication::translate("KeyboardKey", "F24");
            case StandardKey::NumLock:             return QCoreApplication::translate("KeyboardKey", "Num Lock");
            case StandardKey::ScrollLock:          return QCoreApplication::translate("KeyboardKey", "Scroll Lock");
            case StandardKey::BrowserBack:         return QCoreApplication::translate("KeyboardKey", "Browser Back");
            case StandardKey::BrowserForward:      return QCoreApplication::translate("KeyboardKey", "Browser Forward");
            case StandardKey::BrowserRefresh:      return QCoreApplication::translate("KeyboardKey", "Browser Refresh");
            case StandardKey::BrowserStop:         return QCoreApplication::translate("KeyboardKey", "Browser Stop");
            case StandardKey::BrowserSearch:       return QCoreApplication::translate("KeyboardKey", "Browser Search");
            case StandardKey::BrowserFavorites:    return QCoreApplication::translate("KeyboardKey", "Browser Favorites");
            case StandardKey::BrowserHome:         return QCoreApplication::translate("KeyboardKey", "Browser Home");
            case StandardKey::VolumeMute:          return QCoreApplication::translate("KeyboardKey", "Volume Mute");
            case StandardKey::VolumeDown:          return QCoreApplication::translate("KeyboardKey", "Volume Down");
            case StandardKey::VolumeUp:            return QCoreApplication::translate("KeyboardKey", "Volume Up");
            case StandardKey::MediaNextTrack:      return QCoreApplication::translate("KeyboardKey", "Media Next Track");
            case StandardKey::MediaPreviousTrack:  return QCoreApplication::translate("KeyboardKey", "Media Previous Track");
            case StandardKey::MediaStop:           return QCoreApplication::translate("KeyboardKey", "Media Stop");
            case StandardKey::MediaPlayPause:      return QCoreApplication::translate("KeyboardKey", "Media Play/Pause");
            case StandardKey::LaunchMail:          return QCoreApplication::translate("KeyboardKey", "Launch Mail");
            case StandardKey::LaunchMediaSelect:   return QCoreApplication::translate("KeyboardKey", "Launch Media Select");
            case StandardKey::LaunchApp1:          return QCoreApplication::translate("KeyboardKey", "Launch App 1");
            case StandardKey::LaunchApp2:          return QCoreApplication::translate("KeyboardKey", "Launch App 2");
            case StandardKey::Play:                return QCoreApplication::translate("KeyboardKey", "Play");
            default:                               return QCoreApplication::translate("KeyboardKey", "Invalid key");
            }
        }

        return {};
    }

    QChar KeyboardKey::character() const
    {
        Q_ASSERT(mKeyType == KeyType::Character);

        QByteArray byteArray = QByteArray::fromHex(QByteArray::number(mNativeKey, 16));
        QString str = QString::fromUtf8(byteArray);

        return str[0];
    }

    void KeyboardKey::save(std::function<void (const QString &, const QString &)> keyValueCallback) const
    {
        keyValueCallback(QStringLiteral("type"), QString::number(static_cast<int>(mKeyType)));

        switch(mKeyType)
        {
        case KeyType::Invalid:
            break;
        case KeyType::Standard:
            keyValueCallback(QStringLiteral("value"), QString::number(static_cast<unsigned int>(mStandardKey)));
            break;
        case KeyType::Native:
        case KeyType::Character:
            keyValueCallback(QStringLiteral("value"), QString::number(mNativeKey));
            break;
        }
    }

    KeyboardKey KeyboardKey::load(std::function<QString(const QString &)> keyValueCallback)
    {
        auto keyType = static_cast<KeyType>(keyValueCallback(QStringLiteral("type")).toUInt());

        if(keyType == KeyType::Invalid)
            return {};

        return {keyType, keyValueCallback(QStringLiteral("value")).toUInt()};
    }

    QList<KeyboardKey> KeyboardKey::loadKeyListFromJson(const QString &json)
    {
        auto jsonDocument = QJsonDocument::fromJson(json.toUtf8());
        if(jsonDocument.isNull())
            return {};

        QList<KeyboardKey> keys;

        const auto jsonElements = jsonDocument.array();
        for(const auto &jsonElement: jsonElements)
        {
            auto jsonObject = jsonElement.toObject();
            auto key = ActionTools::KeyboardKey::load([&jsonObject](const QString &keyName) -> QString
            {
                return jsonObject.value(keyName).toString();
            });

            if(!key.isValid())
                return {};

            keys.append(key);
        }

        return keys;
    }

    QString KeyboardKey::saveKeyListToJson(const QList<KeyboardKey> &keyList)
    {
        QJsonArray jsonPressedKeyArray;

        for(auto pressedKey: keyList)
        {
            QJsonObject keyObject;

            pressedKey.save([&keyObject](const QString &key, const QString &value)
            {
                keyObject[key] = value;
            });

            jsonPressedKeyArray.append(keyObject);
        }

        QJsonDocument document{jsonPressedKeyArray};
        return QString::fromUtf8(document.toJson());
    }
}
