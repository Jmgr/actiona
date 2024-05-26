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

#pragma once

#include "basewindow.hpp"

#include <QLineEdit>

class QInputDialog;

namespace Code
{
	class InputDialog : public BaseWindow
	{
		Q_OBJECT
		Q_PROPERTY(QJSValue onClosed READ onClosed WRITE setOnClosed)
		Q_PROPERTY(QJSValue onValueChanged READ onValueChanged WRITE setOnValueChanged)
		Q_PROPERTY(QJSValue value READ value WRITE setValue)

	public:
		enum InputType
		{
			Text,
			Integer,
			Float,
            Items,
            MultilineText
		};
        Q_ENUM(InputType)
		enum TextEchoMode
		{
			Normal = QLineEdit::Normal,
			NoEcho = QLineEdit::NoEcho,
			Password = QLineEdit::Password,
			PasswordEchoOnEdit = QLineEdit::PasswordEchoOnEdit
		};
        Q_ENUM(TextEchoMode)
		
        Q_INVOKABLE InputDialog();
        Q_INVOKABLE InputDialog(const QJSValue &parameters);
		~InputDialog() override;
		
		void setOnClosed(const QJSValue &onClosed)						{ mOnClosed = onClosed; }
		void setOnValueChanged(const QJSValue &onValueChanged)			{ mOnValueChanged = onValueChanged; }
		
		QJSValue onClosed() const										{ return mOnClosed; }
		QJSValue onValueChanged() const									{ return mOnValueChanged; }

		QJSValue value() const;
		
        Q_INVOKABLE QString toString() const override					{ return QStringLiteral("InputDialog"); }
        Q_INVOKABLE InputDialog *setLabelText(const QString &labelText);
        Q_INVOKABLE InputDialog *setOkButtonText(const QString &okButtonText);
        Q_INVOKABLE InputDialog *setCancelButtonText(const QString &cancelButtonText);
        Q_INVOKABLE InputDialog *setTextEchoMode(TextEchoMode textEchoMode);
        Q_INVOKABLE InputDialog *setFloatDecimals(int decimals);
        Q_INVOKABLE InputDialog *setIntegerStep(int step);
        Q_INVOKABLE InputDialog *setMaximum(const QJSValue &maximum);
        Q_INVOKABLE InputDialog *setMinimum(const QJSValue &minimum);
        Q_INVOKABLE InputDialog *setRange(const QJSValue &minimum, const QJSValue &maximum);
        Q_INVOKABLE InputDialog *setInputType(InputType inputType);
        Q_INVOKABLE InputDialog *setValue(const QJSValue &value);
        Q_INVOKABLE InputDialog *setItems(const QJSValue &items);
        Q_INVOKABLE InputDialog *setItemsEditable(bool itemsEditable);
        Q_INVOKABLE InputDialog *show();
        Q_INVOKABLE int showModal();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
		
	private slots:
		void finished(int result);
		void doubleValueChanged(double value);
		void intValueChanged(int value);
		void textValueChanged(const QString &value);

	private:
		void setup();

		InputType mInputType{Text};
		QJSValue mValue;
		QJSValue mItems;
		QJSValue mMinimum;
		QJSValue mMaximum;
		QInputDialog *mInputDialog;
		QJSValue mOnClosed;
		QJSValue mOnValueChanged;
	};
}

