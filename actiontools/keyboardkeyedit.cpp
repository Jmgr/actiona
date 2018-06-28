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

#include "keyboardkeyedit.h"

#include <QDebug>

namespace ActionTools
{
    KeyboardKeyEdit::KeyboardKeyEdit(QWidget *parent):
        CodeLineEdit(parent)
    {
    }

    void KeyboardKeyEdit::keyPressEvent(QKeyEvent *event)
    {
        if(isCode())
        {
            CodeLineEdit::keyPressEvent(event);

            return;
        }

        KeyboardKey key{event};

        if(!key.isValid() || key.keyType() == KeyboardKey::KeyType::Native)
        {
            event->ignore();

            return;
        }

        m_pressedKeys.push_back(key);

        QString keys;

        for(auto pressedKey: m_pressedKeys)
        {
            if(!keys.isEmpty())
                keys += QStringLiteral(" + ");

            keys += pressedKey.name();
        }

        m_keys = m_pressedKeys;

        setText(keys);

        event->accept();
    }

    void KeyboardKeyEdit::keyReleaseEvent(QKeyEvent *event)
    {
        if(isCode())
        {
            CodeLineEdit::keyReleaseEvent(event);

            return;
        }

        m_pressedKeys.removeOne(KeyboardKey{event});

        event->accept();
    }
}
