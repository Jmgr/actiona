/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui
{
	class SettingsDialog;
}

class QNetworkReply;
class QNetworkAccessManager;
class QNetworkReply;
class QTimer;
class QSystemTrayIcon;

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	SettingsDialog(QSystemTrayIcon *systemTrayIcon, QWidget *parent = 0);
	~SettingsDialog();

private slots:
	void onTimeout();
	void on_testProxy_clicked();
	void proxyTestFinished(QNetworkReply *reply);
	void accept();
	void done(int result);

private:
	Ui::SettingsDialog *ui;
	QButtonGroup *mExecutionWindowButtonGroup;
	QButtonGroup *mConsoleWindowButtonGroup;
	QNetworkAccessManager *mNetworkAccessManager;
	QNetworkReply *mNetworkReply;
	QTimer *mTimeoutTimer;
	QSystemTrayIcon *mSystemTrayIcon;
	bool mPreviousASCRAssociation;
	bool mPreviousACODAssociation;

	Q_DISABLE_COPY(SettingsDialog)
};

#endif // SETTINGSDIALOG_H

