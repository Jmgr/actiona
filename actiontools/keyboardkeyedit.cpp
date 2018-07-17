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

namespace ActionTools
{
    KeyboardKeyEdit::KeyboardKeyEdit(QWidget *parent):
        CodeLineEdit(parent)
    {
        connect(this, &CodeLineEdit::codeChanged, this, &KeyboardKeyEdit::onCodeChanged);

        setAttribute(Qt::WA_InputMethodEnabled, isCode());
    }

    void KeyboardKeyEdit::setKeys(const QList<KeyboardKey> &keys)
    {
        m_keys = keys;
        m_pressedKeys.clear();

        updateKeyText();
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

        if(m_pressedKeys.isEmpty())
            m_keys.clear();

        m_pressedKeys.insert(key);
        m_keys = m_pressedKeys.toList();

        updateKeyText();

        event->accept();
    }

    void KeyboardKeyEdit::keyReleaseEvent(QKeyEvent *event)
    {
        if(isCode())
        {
            CodeLineEdit::keyReleaseEvent(event);

            return;
        }

        m_pressedKeys.remove(KeyboardKey{event});

        event->accept();
    }

    QVariant KeyboardKeyEdit::inputMethodQuery(Qt::InputMethodQuery query) const
    {
        Q_UNUSED(query)

        // We override this function to skip any dead key
        return {};
    }

    void KeyboardKeyEdit::onCodeChanged(bool code)
    {
        Q_UNUSED(code)

        setAttribute(Qt::WA_InputMethodEnabled, code);

        m_keys.clear();
        m_pressedKeys.clear();

        updateKeyText();
    }

    void KeyboardKeyEdit::updateKeyText()
    {
        QString keys;

        for(auto pressedKey: m_keys)
        {
            if(!keys.isEmpty())
                keys += QStringLiteral(" + ");

            keys += pressedKey.name();
        }

        setText(keys);
    }
}
