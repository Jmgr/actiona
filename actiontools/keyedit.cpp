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

#include "keyedit.h"

#include <QKeyEvent>
#include <QMenu>

namespace ActionTools
{
	KeyEdit::KeyEdit(QWidget *parent)
		: CodeLineEdit(parent)
	{
	}

	void KeyEdit::setKeySequence(QKeySequence keySequence)
	{
		mKeySequence = keySequence;
		setText(mKeySequence.toString(QKeySequence::NativeText));
	}

	void KeyEdit::contextMenuEvent(QContextMenuEvent *event)
	{
		QMenu *menu = createStandardContextMenu();

		menu->addSeparator();

		addShortcuts(menu);

		menu->addSeparator();

		QMenu *keyMenu = menu->addMenu(tr("Special keys"));
		for(int i = 1; i <= 12; ++i)
			addKeyToMenu(keyMenu, QString("Alt+F%1").arg(i));
		addKeyToMenu(keyMenu, "Return");
		addKeyToMenu(keyMenu, "Escape");

		menu->exec(event->globalPos());

		delete menu;

		event->accept();
	}

	void KeyEdit::keyPressEvent(QKeyEvent *event)
	{
		if(isCode())
		{
			CodeLineEdit::keyPressEvent(event);
			return;
		}

		Qt::KeyboardModifiers modifier = event->modifiers();
		if(modifier & Qt::KeypadModifier)
			modifier = Qt::NoModifier;

		if(event->key() == Qt::Key_unknown ||
		   event->key() == Qt::Key_Return ||
		   event->key() == Qt::Key_Close ||
		   event->key() == 0 ||
		   event->text().isEmpty())
		{
			event->ignore();
			return;
		}

		setKeySequence(QKeySequence(modifier + event->key()));
		event->ignore();
	}

	void KeyEdit::menuKeyTriggered()
	{
		QAction *action = qobject_cast<QAction *>(sender());
		if(!action)
			return;

		setKeySequence(QKeySequence(action->text()));
	}

	void KeyEdit::addKeyToMenu(QMenu *menu, const QString &key)
	{
		connect(menu->addAction(key), SIGNAL(triggered()), this, SLOT(menuKeyTriggered()));
	}
}
