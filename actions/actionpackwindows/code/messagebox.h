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

#include "basewindow.h"

#include <QMessageBox>

namespace Code
{
	class MessageBox : public BaseWindow
	{
		Q_OBJECT
		Q_ENUMS(StandardButton)
		Q_ENUMS(Icon)
		Q_PROPERTY(QScriptValue onClosed READ onClosed WRITE setOnClosed)

	public:
		enum StandardButton
		{
			Ok = QMessageBox::Ok,
			Open = QMessageBox::Open,
			Save = QMessageBox::Save,
			Cancel = QMessageBox::Cancel,
			Close = QMessageBox::Close,
			Discard = QMessageBox::Discard,
			Apply = QMessageBox::Apply,
			Reset = QMessageBox::Reset,
			RestoreDefaults = QMessageBox::RestoreDefaults,
			Help = QMessageBox::Help,
			SaveAll = QMessageBox::SaveAll,
			Yes = QMessageBox::Yes,
			YesToAll = QMessageBox::YesToAll,
			No = QMessageBox::No,
			NoToAll = QMessageBox::NoToAll,
			Abort = QMessageBox::Abort,
			Retry = QMessageBox::Retry,
			Ignore = QMessageBox::Ignore,
			NoButton = QMessageBox::NoButton
		};
		enum Icon
		{
			NoIcon = QMessageBox::NoIcon,
			Question = QMessageBox::Question,
			Information = QMessageBox::Information,
			Warning = QMessageBox::Warning,
			Critical = QMessageBox::Critical
		};

		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

		MessageBox();
		~MessageBox() override;
		
		void setOnClosed(const QScriptValue &onClosed)						{ mOnClosed = onClosed; }
		
		QScriptValue onClosed() const										{ return mOnClosed; }

	public slots:
		QString toString() const override					{ return QStringLiteral("MessageBox"); }
		QScriptValue setText(const QString &text);
		QScriptValue setDetailedText(const QString &detailedText);
		QScriptValue setInformativeText(const QString &informativeText);
		QScriptValue setButtons(StandardButton buttons);
		QScriptValue setIcon(Icon icon);
		QScriptValue setIconPixmap(const QScriptValue &image);
		QScriptValue setDefaultButton(StandardButton button);
		QScriptValue setEscapeButton(StandardButton button);
		QScriptValue addCustomButton(StandardButton button, const QString &text);
		QScriptValue show();
		int showModal();

	private slots:
		void finished(int result);

	private:
		QMessageBox *mMessageBox;
		QScriptValue mOnClosed;
	};
}

