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
	SettingsDialog(QSystemTrayIcon *systemTrayIcon, QWidget *parent = nullptr);
	~SettingsDialog() override;

private slots:
	void enableCustomProxy();
	void disableCustomProxy();
	void onTimeout();
	void on_testConnectivity_clicked();
	void proxyTestFinished();
	void accept() override;
	void done(int result) override;
    void languageChanged();

private:
	void setCustomProxyEnabled(bool enabled);
	int proxyMode() const;

	Ui::SettingsDialog *ui;
	QButtonGroup *mExecutionWindowButtonGroup;
	QButtonGroup *mConsoleWindowButtonGroup;
	QNetworkAccessManager *mNetworkAccessManager;
	QNetworkReply *mNetworkReply;
	QTimer *mTimeoutTimer;
	QSystemTrayIcon *mSystemTrayIcon;
	bool mPreviousASCRAssociation;
	bool mPreviousACODAssociation;
    bool mLocaleChangeWarning;

	Q_DISABLE_COPY(SettingsDialog)
};

