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
#include "codelineedit.h"

#include <QKeyEvent>
#include <QDebug>

namespace ActionTools
{
	KeyEdit::KeyEdit(QWidget *parent)
		: CodeComboBox(parent)
	{
		installEventFilter(this);
	}

	void KeyEdit::setKeyInput(const KeyInput &keyInput)
	{
		mKeyInput = keyInput;
		
		codeLineEdit()->setText(mKeyInput.toTranslatedText());
	}
	
	bool KeyEdit::eventFilter(QObject *object, QEvent *event)
	{
		if(isCode())
			return QObject::eventFilter(object, event);
		
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			
			if(!mKeyInput.fromEvent(keyEvent))
				return false;
			
			codeLineEdit()->setText(mKeyInput.toTranslatedText());
			
			return true;
		}

		return QObject::eventFilter(object, event);
	}
}
