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
#include "codelineedit.h"
#include "keyboardkey.hpp"

#include <QList>
#include <QSet>

namespace ActionTools
{
    class ACTIONTOOLSSHARED_EXPORT KeyboardKeyEdit : public CodeLineEdit
    {
        Q_OBJECT

    public:
        explicit KeyboardKeyEdit(QWidget *parent = nullptr);

        const QList<KeyboardKey> &keys() const { return m_keys; }
        void setKeys(const QList<KeyboardKey> &keys);

    protected:
        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;
        QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;

    private slots:
        void onCodeChanged(bool code);

    private:
        void updateKeyText();

        QSet<KeyboardKey> m_pressedKeys;
        QList<KeyboardKey> m_keys;

        Q_DISABLE_COPY(KeyboardKeyEdit)
    };
}
