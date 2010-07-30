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
#include "codecombobox.h"
#include "codelineedit.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QEvent>
#include <QKeyEvent>

namespace ActionTools
{
	KeyEdit::KeyEdit(QWidget *parent)
		: QWidget(parent),
		mCodeComboBox(new CodeComboBox(this)),
		mCtrlCheckBox(new QCheckBox(tr("Ctrl"), this)),
		mAltCheckBox(new QCheckBox(tr("Alt"), this)),
		mShiftCheckBox(new QCheckBox(tr("Shift"), this)),
		mMetaCheckBox(new QCheckBox(tr("Meta"), this))
	{
		QHBoxLayout *modifiersLayout = new QHBoxLayout;
		modifiersLayout->setMargin(0);
		modifiersLayout->addWidget(mCtrlCheckBox);
		modifiersLayout->addWidget(mAltCheckBox);
		modifiersLayout->addWidget(mShiftCheckBox);
		modifiersLayout->addWidget(mMetaCheckBox);
		
		QVBoxLayout *layout = new QVBoxLayout;
		layout->setMargin(0);
		layout->addWidget(mCodeComboBox);
		layout->addLayout(modifiersLayout);
		
		setLayout(layout);
		
		mCodeComboBox->codeLineEdit()->installEventFilter(this);
		mCodeComboBox->codeLineEdit()->setReadOnly(true);
		mCodeComboBox->codeLineEdit()->setFocusProxy(this);
		mCodeComboBox->installEventFilter(this);
		mCodeComboBox->setFocusProxy(this);
		setAttribute(Qt::WA_InputMethodEnabled);
	}

	void KeyEdit::setKeySequence(const QKeySequence &keySequence)
	{
		if(keySequence == mKeySequence)
			return;
		
		mKeySequence = keySequence;
		
		mCodeComboBox->codeLineEdit()->setText(mKeySequence.toString(QKeySequence::NativeText));
	}
	
	bool KeyEdit::eventFilter(QObject *object, QEvent *event)
	{
		if(object == mCodeComboBox && event->type() == QEvent::ContextMenu)
		{
			//TODO
			
			event->accept();
			return true;
		}
		
		return QWidget::eventFilter(object, event);
	}
	
	void KeyEdit::focusInEvent(QFocusEvent *event)
	{
		mCodeComboBox->event(event);
		mCodeComboBox->codeLineEdit()->selectAll();
		
		QWidget::focusOutEvent(event);
	}

	void KeyEdit::focusOutEvent(QFocusEvent *event)
	{
		mCodeComboBox->event(event);
		
		QWidget::focusOutEvent(event);
	}

	void KeyEdit::keyPressEvent(QKeyEvent *event)
	{
		int key = event->key();
		if (key == Qt::Key_Control || key == Qt::Key_Shift ||
			key == Qt::Key_Meta || key == Qt::Key_Alt ||
			key == Qt::Key_Super_L || key == Qt::Key_AltGr)
			return;
		
		key |= translateModifiers(event->modifiers(), event->text());
		
		mKeySequence = QKeySequence(key);
		
		mCodeComboBox->codeLineEdit()->setText(mKeySequence.toString(QKeySequence::NativeText));
		
		event->accept();
	}

	void KeyEdit::keyReleaseEvent(QKeyEvent *event)
	{
		mCodeComboBox->event(event);
	}
	
	bool KeyEdit::event(QEvent *event)
	{
		if(	event->type() == QEvent::Shortcut ||
			event->type() == QEvent::ShortcutOverride  ||
			event->type() == QEvent::KeyRelease)
		{
			event->accept();
			return true;
		}
		
		return QWidget::event(event);
	}
	
	int KeyEdit::translateModifiers(Qt::KeyboardModifiers state, const QString &text) const
	{
		int result = 0;
		
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
