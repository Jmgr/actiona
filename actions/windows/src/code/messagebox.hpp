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

#include <QMessageBox>

namespace Code
{
	class MessageBox : public BaseWindow
	{
		Q_OBJECT
		Q_PROPERTY(QJSValue onClosed READ onClosed WRITE setOnClosed)

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
        Q_ENUM(StandardButton)
		enum Icon
		{
			NoIcon = QMessageBox::NoIcon,
			Question = QMessageBox::Question,
			Information = QMessageBox::Information,
			Warning = QMessageBox::Warning,
			Critical = QMessageBox::Critical
		};
        Q_ENUM(Icon)

        Q_INVOKABLE MessageBox();
        Q_INVOKABLE MessageBox(const QJSValue &parameters);
		~MessageBox() override;
		
		void setOnClosed(const QJSValue &onClosed)						{ mOnClosed = onClosed; }
		
		QJSValue onClosed() const										{ return mOnClosed; }

        Q_INVOKABLE QString toString() const override					{ return QStringLiteral("MessageBox"); }
        Q_INVOKABLE MessageBox *setText(const QString &text);
        Q_INVOKABLE MessageBox *setDetailedText(const QString &detailedText);
        Q_INVOKABLE MessageBox *setInformativeText(const QString &informativeText);
        Q_INVOKABLE MessageBox *setButtons(StandardButton buttons);
        Q_INVOKABLE MessageBox *setIcon(Icon icon);
        Q_INVOKABLE MessageBox *setIconPixmap(const QJSValue &image);
        Q_INVOKABLE MessageBox *setDefaultButton(StandardButton button);
        Q_INVOKABLE MessageBox *setEscapeButton(StandardButton button);
        Q_INVOKABLE MessageBox *addCustomButton(StandardButton button, const QString &text);
        Q_INVOKABLE MessageBox *show();
        Q_INVOKABLE int showModal();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

	private slots:
		void finished(int result);

	private:
		QMessageBox *mMessageBox;
		QJSValue mOnClosed;
	};
}

