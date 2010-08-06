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
#include "keyinput.h"

#include <QKeyEvent>
#include <QDebug>

namespace ActionTools
{
	KeyEdit::KeyEdit(QWidget *parent)
		: CodeComboBox(parent)
	{
		installEventFilter(this);
	}

	void KeyEdit::setKeySequence(const QKeySequence &keySequence)
	{
		if(keySequence == mKeySequence)
			return;
		
		mKeySequence = keySequence;
		
		codeLineEdit()->setText(mKeySequence.toString(QKeySequence::NativeText));
	}

	void KeyEdit::keyPressEvent(QKeyEvent *event)
	{
		/*int key = event->key();
		
		key |= translateModifiers(event->modifiers(), event->text());
		
		mKeySequence = QKeySequence(key);
		
		codeLineEdit()->setText(mKeySequence.toString(QKeySequence::NativeText));
		
		event->accept();*/
		
		mKeySequence = QKeySequence(event->key());
		
		codeLineEdit()->setText(mKeySequence.toString(QKeySequence::NativeText));
	}

	void KeyEdit::keyReleaseEvent(QKeyEvent *event)
	{
		//event->accept();
		
		return CodeComboBox::keyReleaseEvent(event);
	}
	
	bool KeyEdit::eventFilter(QObject *object, QEvent *event)
	{
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			
			KeyInput keyInput;
			
			if(!keyInput.fromEvent(keyEvent))
				codeLineEdit()->setText("Failed");
			else
				codeLineEdit()->setText(keyInput.toTranslatedText());
			
			return true;
		/*
			int key = keyEvent->key();
			
			qDebug() << key;
			if(key != Qt::Key_Shift)
				key |= translateModifiers(keyEvent->modifiers(), keyEvent->text());
			
			qDebug() << key;
			
			if(key != Qt::Key_Shift)
			mKeySequence = QKeySequence(key);
			else
				mKeySequence = QKeySequence("Shift");
			
			qDebug() << mKeySequence;
			
			codeLineEdit()->setText(mKeySequence.toString(QKeySequence::NativeText));
			*/
		}

		return QObject::eventFilter(object, event);
	}
	
	int KeyEdit::translateModifiers(Qt::KeyboardModifiers state, const QString &text) const
	{
		int result = 0;
		
		qDebug() << text;
		
		if ((state & Qt::ShiftModifier) && (text.size() == 0 || !text.at(0).isPrint() || text.at(0).isLetter() || text.at(0).isSpace()))
			result |= Qt::SHIFT;
		if (state & Qt::ControlModifier)
			result |= Qt::CTRL;
		if (state & Qt::MetaModifier)
			result |= Qt::META;
		if (state & Qt::AltModifier)
			result |= Qt::ALT;
		
		return result;
	}
}
