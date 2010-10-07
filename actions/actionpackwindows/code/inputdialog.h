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

#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include "basewindow.h"

#include <QLineEdit>

class QInputDialog;

namespace Code
{
	class InputDialog : public BaseWindow
	{
		Q_OBJECT
		Q_ENUMS(InputType)
		Q_ENUMS(TextEchoMode)

	public:
		enum InputType
		{
			Text,
			Integer,
			Float,
			Items
		};
		enum TextEchoMode
		{
			Normal = QLineEdit::Normal,
			NoEcho = QLineEdit::NoEcho,
			Password = QLineEdit::Password,
			PasswordEchoOnEdit = QLineEdit::PasswordEchoOnEdit
		};
		
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

		InputDialog();
		~InputDialog();
		
	public slots:
		QScriptValue setLabelText(const QString &labelText);
		QScriptValue setOkButtonText(const QString &okButtonText);
		QScriptValue setCancelButtonText(const QString &cancelButtonText);
		QScriptValue setTextEchoMode(TextEchoMode textEchoMode);
		QScriptValue setFloatDecimals(int decimals);
		QScriptValue setIntegerStep(int step);
		QScriptValue setMaximum(const QScriptValue &maximum);
		QScriptValue setMinimum(const QScriptValue &minimum);
		QScriptValue setRange(const QScriptValue &minimum, const QScriptValue &maximum);
		QScriptValue setInputType(InputType inputType);
		QScriptValue setValue(const QScriptValue &value);
		QScriptValue setItems(const QScriptValue &items);
		QScriptValue setItemsEditable(bool itemsEditable);
		QScriptValue show();
		int showModal();
		QScriptValue value() const;
		
	private slots:
		void finished(int result);
		void doubleValueChanged(double value);
		void intValueChanged(int value);
		void textValueChanged(const QString &value);

	private:
		void setup();

		InputType mInputType;
		QScriptValue mValue;
		QScriptValue mItems;
		QScriptValue mMinimum;
		QScriptValue mMaximum;
		QInputDialog *mInputDialog;
		QScriptValue mOnClosed;
		QScriptValue mOnValueChanged;
		QScriptValue mThisObject;
	};
}

#endif // INPUTDIALOG_H
