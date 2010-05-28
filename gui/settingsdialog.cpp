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

#include "settingsdialog.h"
#include "screenpositionwidget.h"
#include "settings.h"
#include "ui_settingsdialog.h"

#include <QSettings>
#include <QNetworkProxy>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QxtGlobalShortcut>

SettingsDialog::SettingsDialog(QSystemTrayIcon *systemTrayIcon, QWidget *parent)
	: QDialog(parent),
	ui(new Ui::SettingsDialog),
	mExecutionWindowButtonGroup(new QButtonGroup(this)),
	mConsoleWindowButtonGroup(new QButtonGroup(this)),
	mNetworkAccessManager(new QNetworkAccessManager(this)),
	mNetworkReply(0),
	mTimeoutTimer(new QTimer(this)),
	mSystemTrayIcon(systemTrayIcon)
{
	ui->setupUi(this);

	connect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(proxyTestFinished(QNetworkReply*)));

	QSettings settings;

	ui->noSysTrayLabel->setVisible(!QSystemTrayIcon::isSystemTrayAvailable());
	ui->noSysTrayMessagesLabel->setVisible(!QSystemTrayIcon::supportsMessages());

	//GENERAL
	ui->showLoadingWindow->setChecked(settings.value("general/showLoadingWindow", QVariant(true)).toBool());
	ui->showTaskbarIcon->setChecked(settings.value("general/showTaskbarIcon", QVariant(true)).toBool());
	ui->showWindowAfterExecution->setChecked(settings.value("general/showWindowAfterExecution", QVariant(true)).toBool());
	ui->addStartEndSeparators->setChecked(settings.value("general/addConsoleStartEndSeparators", QVariant(true)).toBool());
	ui->reopenLastScript->setChecked(settings.value("general/reopenLastScript", QVariant(false)).toBool());
	ui->maxRecentFiles->setValue(settings.value("general/maxRecentFiles", QVariant(5)).toInt());

	//ACTIONS
	ui->executionWindowGroup->setChecked(settings.value("actions/showExecutionWindow", QVariant(true)).toBool());
	int screen = settings.value("actions/executionWindowScreen", QVariant(0)).toInt();
	int position = settings.value("actions/executionWindowPosition", QVariant(0)).toInt();
	ui->executionWindowPosition->setPosition(screen, position);
	ui->consoleWindowGroup->setChecked(settings.value("actions/showConsoleWindow", QVariant(true)).toBool());
	screen = settings.value("actions/consoleWindowScreen", QVariant(0)).toInt();
	position = settings.value("actions/consoleWindowPosition", QVariant(1)).toInt();
	ui->consoleWindowPosition->setPosition(screen, position);
	ui->stopExecutionHotkey->setKeySequence(
		QKeySequence(settings.value("actions/stopExecutionHotkey", QKeySequence("Ctrl+Alt+Q")).toString()));
	ui->switchTextCode->setKeySequence(
		QKeySequence(settings.value("actions/switchTextCode", QKeySequence("Ctrl+Shift+C")).toString()));
	ui->openEditorKey->setKeySequence(
		QKeySequence(settings.value("actions/openEditorKey", QKeySequence("Ctrl+Shift+V")).toString()));
	ui->checkCodeSyntaxAutomatically->setChecked(settings.value("actions/checkCodeSyntaxAutomatically", QVariant(true)).toBool());

	//NETWORK
	ui->updatesCheck->setCurrentIndex(settings.value("network/updatesCheck", QVariant(ActionTools::Settings::CHECK_FOR_UPDATES_DAY)).toInt());
	ui->useAProxy->setChecked(settings.value("network/useAProxy", QVariant(false)).toBool());
	ui->proxySettings->setEnabled(ui->useAProxy->isChecked());
	ui->testProxy->setEnabled(ui->useAProxy->isChecked());
	ui->proxyHost->setText(settings.value("network/proxyHost", QVariant()).toString());
	ui->proxyPort->setText(settings.value("network/proxyPort", QVariant()).toString());
	ui->proxyUser->setText(settings.value("network/proxyUser", QVariant()).toString());
	ui->proxyPassword->setText(settings.value("network/proxyPassword", QVariant()).toString());
	ui->proxyType->setCurrentIndex(settings.value("network/proxyType", QVariant(ActionTools::Settings::PROXY_TYPE_HTTP)).toInt());

	connect(mTimeoutTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
	connect(ui->showTaskbarIcon, SIGNAL(clicked(bool)), systemTrayIcon, SLOT(setVisible(bool)));
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::onTimeout()
{
	if(!mNetworkReply)
		return;

	mNetworkReply->abort();
}

void SettingsDialog::on_testProxy_clicked()
{
	QNetworkProxy proxy;

	if(ui->proxyType->currentIndex() == ActionTools::Settings::PROXY_TYPE_HTTP)
		proxy.setType(QNetworkProxy::HttpProxy);
	else
		proxy.setType(QNetworkProxy::Socks5Proxy);
	proxy.setHostName(ui->proxyHost->text());
	proxy.setPort(ui->proxyPort->text().toInt());
	proxy.setUser(ui->proxyUser->text());
	proxy.setPassword(ui->proxyPassword->text());

	mNetworkAccessManager->setProxy(proxy);
	mNetworkReply = mNetworkAccessManager->get(QNetworkRequest(QUrl("http://www.jmgr.info")));

	setEnabled(false);

	mTimeoutTimer->start(3000);
}

void SettingsDialog::proxyTestFinished(QNetworkReply *reply)
{
	mTimeoutTimer->stop();

	QNetworkReply::NetworkError error = reply->error();
	switch(error)
	{
	case QNetworkReply::NoError:
		QMessageBox::information(this, tr("Proxy test ended"), tr("The proxy test succeeded."));
		break;
	case QNetworkReply::ProxyAuthenticationRequiredError:
		QMessageBox::warning(this, tr("Proxy test ended"), tr("The proxy test failed : invalid username or password."));
		break;
	default:
		QMessageBox::warning(this, tr("Proxy test ended"), tr("The proxy test failed : incorrect proxy settings."));
		break;
	}

	reply->deleteLater();
	setEnabled(true);
}

void SettingsDialog::accept()
{
	if(ui->switchTextCode->keySequence() == ui->openEditorKey->keySequence() && ui->switchTextCode->keySequence().toString() != QString())
	{
		QMessageBox::warning(this, tr("Settings"), tr("You have set the same key sequence for switching text/code mode and opening the editor, please choose a different one."));
		return;
	}

	QxtGlobalShortcut *globalShortcut = new QxtGlobalShortcut(this);
	if(!globalShortcut->setShortcut(ui->stopExecutionHotkey->keySequence()))
	{
		//TODO
		QMessageBox::question(this, tr("Execution shortcut"), tr("Unable to set the execution shortcut"));
	}
	globalShortcut->deleteLater();

	QSettings settings;

	//GENERAL
	settings.setValue("general/showLoadingWindow", QVariant(ui->showLoadingWindow->isChecked()));
	settings.setValue("general/showTaskbarIcon", QVariant(ui->showTaskbarIcon->isChecked()));
	settings.setValue("general/showWindowAfterExecution", QVariant(ui->showWindowAfterExecution->isChecked()));
	settings.setValue("general/addConsoleStartEndSeparators", QVariant(ui->addStartEndSeparators->isChecked()));
	settings.setValue("general/reopenLastScript", QVariant(ui->reopenLastScript->isChecked()));
	settings.setValue("general/maxRecentFiles", QVariant(ui->maxRecentFiles->value()));

	//ACTIONS
	settings.setValue("actions/showExecutionWindow", QVariant(ui->executionWindowGroup->isChecked()));
	settings.setValue("actions/executionWindowPosition", ui->executionWindowPosition->position());
	settings.setValue("actions/executionWindowScreen", ui->executionWindowPosition->screen());
	settings.setValue("actions/showConsoleWindow", QVariant(ui->consoleWindowGroup->isChecked()));
	settings.setValue("actions/consoleWindowPosition", ui->consoleWindowPosition->position());
	settings.setValue("actions/consoleWindowScreen", ui->consoleWindowPosition->screen());
	settings.setValue("actions/stopExecutionHotkey", QVariant::fromValue(ui->stopExecutionHotkey->keySequence()));
	settings.setValue("actions/switchTextCode", QVariant::fromValue(ui->switchTextCode->keySequence()));
	settings.setValue("actions/openEditorKey", QVariant::fromValue(ui->openEditorKey->keySequence()));
	settings.setValue("actions/checkCodeSyntaxAutomatically", QVariant(ui->checkCodeSyntaxAutomatically->isChecked()));

	//NETWORK
	settings.setValue("network/updatesCheck", QVariant(ui->updatesCheck->currentIndex()));
	settings.setValue("network/useAProxy", QVariant(ui->useAProxy->isChecked()));
	settings.setValue("network/proxyHost", QVariant(ui->proxyHost->text()));
	settings.setValue("network/proxyPort", QVariant(ui->proxyPort->text()));
	settings.setValue("network/proxyUser", QVariant(ui->proxyUser->text()));
	settings.setValue("network/proxyPassword", QVariant(ui->proxyPassword->text()));
	settings.setValue("network/proxyType", QVariant(ui->proxyType->currentIndex()));

	QDialog::accept();
}

void SettingsDialog::done(int result)
{
	QSettings settings;

	mSystemTrayIcon->setVisible(settings.value("general/showTaskbarIcon", QVariant(true)).toBool());

	QDialog::done(result);
}
