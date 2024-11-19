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

#include "settingskeyedit.hpp"

#include <QKeyEvent>

SettingsKeyEdit::SettingsKeyEdit(QWidget *parent)
	: QLineEdit(parent)
{
}

void SettingsKeyEdit::setKeySequence(QKeySequence keySequence)
{
	mKeySequence = keySequence;
	setText(mKeySequence.toString(QKeySequence::NativeText));
}

void SettingsKeyEdit::keyPressEvent(QKeyEvent *event)
{
    Qt::KeyboardModifiers modifiers = event->modifiers();
    if(modifiers & Qt::KeypadModifier)
        modifiers &= ~Qt::KeypadModifier;

    if(event->key() == Qt::Key_unknown || event->key() == 0)
    {
        event->ignore();
        return;
    }

    // Ignore modifier keys themselves
    if (event->key() == Qt::Key_Control || event->key() == Qt::Key_Shift ||
        event->key() == Qt::Key_Alt || event->key() == Qt::Key_Meta)
    {
        event->ignore();
        return;
    }

    // Combine modifiers and key to create the key sequence
    QKeySequence keySequence(modifiers | event->key());

    setKeySequence(keySequence);
    event->accept();
}
