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

#include "settingsdialog.h"
#include "screenpositionwidget.h"
#include "settings.h"
#include "ui_settingsdialog.h"
#include "global.h"

#ifdef Q_WS_WIN
#include "registry.h"
#include <Shlwapi.h>
#include <ShlObj.h>
#include <QDir>
#endif

#include <QSettings>
#include <QNetworkProxy>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QTimer>
#include <QSystemTrayIcon>

SettingsDialog::SettingsDialog(QSystemTrayIcon *systemTrayIcon, QWidget *parent)
	: QDialog(parent),
	ui(new Ui::SettingsDialog),
	mExecutionWindowButtonGroup(new QButtonGroup(this)),
	mConsoleWindowButtonGroup(new QButtonGroup(this)),
	mNetworkAccessManager(new QNetworkAccessManager(this)),
	mNetworkReply(0),
	mTimeoutTimer(new QTimer(this)),
	mSystemTrayIcon(systemTrayIcon),
	mPreviousASCRAssociation(false),
	mPreviousACODAssociation(false)
{
	ui->setupUi(this);

#ifdef Q_WS_X11
	ui->fileAssociationsLabel->setVisible(false);
	ui->associateASCRCheckBox->setVisible(false);
	ui->associateACODCheckBox->setVisible(false);
#endif

	connect(ui->noProxyRadioButton, SIGNAL(clicked()), this, SLOT(disableCustomProxy()));
	connect(ui->systemProxyRadioButton, SIGNAL(clicked()), this, SLOT(disableCustomProxy()));
	connect(ui->customProxyRadioButton, SIGNAL(clicked()), this, SLOT(enableCustomProxy()));

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
	ui->pauseExecutionHotkey->setKeySequence(
		QKeySequence(settings.value("actions/pauseExecutionHotkey", QKeySequence("Ctrl+Alt+W")).toString()));
	ui->switchTextCode->setKeySequence(
		QKeySequence(settings.value("actions/switchTextCode", QKeySequence("Ctrl+Shift+C")).toString()));
	ui->openEditorKey->setKeySequence(
		QKeySequence(settings.value("actions/openEditorKey", QKeySequence("Ctrl+Shift+V")).toString()));
	ui->checkCodeSyntaxAutomatically->setChecked(settings.value("actions/checkCodeSyntaxAutomatically", QVariant(true)).toBool());

	//NETWORK
#ifndef ACT_NO_UPDATER
	ui->updatesCheck->setCurrentIndex(settings.value("network/updatesCheck", QVariant(ActionTools::Settings::CHECK_FOR_UPDATES_DAY)).toInt());
#endif

	int proxyMode = settings.value("network/proxyMode", QVariant(ActionTools::Settings::PROXY_SYSTEM)).toInt();

	switch(proxyMode)
	{
	case ActionTools::Settings::PROXY_NONE:
		ui->noProxyRadioButton->setChecked(true);
		break;
	case ActionTools::Settings::PROXY_SYSTEM:
		ui->systemProxyRadioButton->setChecked(true);
		break;
	case ActionTools::Settings::PROXY_CUSTOM:
		ui->customProxyRadioButton->setChecked(true);
		break;
	}

	setCustomProxyEnabled(ui->customProxyRadioButton->isChecked());
	ui->proxyHost->setText(settings.value("network/proxyHost", QVariant()).toString());
	ui->proxyPort->setText(settings.value("network/proxyPort", QVariant()).toString());
	ui->proxyUser->setText(settings.value("network/proxyUser", QVariant()).toString());
	ui->proxyPassword->setText(settings.value("network/proxyPassword", QVariant()).toString());
	ui->proxyType->setCurrentIndex(settings.value("network/proxyType", QVariant(ActionTools::Settings::PROXY_TYPE_HTTP)).toInt());

#ifdef Q_WS_WIN
	QVariant result;
	mPreviousASCRAssociation = (ActionTools::Registry::read(result, ActionTools::Registry::ClassesRoot, ".ascr") == ActionTools::Registry::ReadOk);
	mPreviousACODAssociation = (ActionTools::Registry::read(result, ActionTools::Registry::ClassesRoot, ".acod") == ActionTools::Registry::ReadOk);

	ui->associateASCRCheckBox->setChecked(mPreviousASCRAssociation);
	ui->associateACODCheckBox->setChecked(mPreviousACODAssociation);
#endif

	connect(mTimeoutTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

	if(systemTrayIcon)
		connect(ui->showTaskbarIcon, SIGNAL(clicked(bool)), systemTrayIcon, SLOT(setVisible(bool)));
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::enableCustomProxy()
{
	setCustomProxyEnabled(true);
}

void SettingsDialog::disableCustomProxy()
{
	setCustomProxyEnabled(false);
}

void SettingsDialog::onTimeout()
{
	if(!mNetworkReply)
		return;

	QMessageBox::warning(this, tr("Connectivity test ended"), tr("The connectivity test failed: maximum time exceeded."));

	mNetworkReply->abort();
}

void SettingsDialog::on_testConnectivity_clicked()
{
	QNetworkProxy proxy;

	switch(proxyMode())
	{
	case ActionTools::Settings::PROXY_NONE:
		proxy.setType(QNetworkProxy::NoProxy);
		break;
	case ActionTools::Settings::PROXY_SYSTEM:
		{
			QUrl url(Global::CONNECTIVITY_URL);
			QNetworkProxyQuery networkProxyQuery(url);
			QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery(networkProxyQuery);
			if(!listOfProxies.isEmpty())
				proxy = listOfProxies.first();
			else
				proxy.setType(QNetworkProxy::NoProxy);
		}
		break;
	case ActionTools::Settings::PROXY_CUSTOM:
		if(ui->proxyType->currentIndex() == ActionTools::Settings::PROXY_TYPE_HTTP)
			proxy.setType(QNetworkProxy::HttpProxy);
		else
			proxy.setType(QNetworkProxy::Socks5Proxy);
		proxy.setHostName(ui->proxyHost->text());
		proxy.setPort(ui->proxyPort->text().toInt());
		proxy.setUser(ui->proxyUser->text());
		proxy.setPassword(ui->proxyPassword->text());
		break;
	}

	QNetworkProxy::setApplicationProxy(proxy);
	mNetworkReply = mNetworkAccessManager->get(QNetworkRequest(QUrl(Global::CONNECTIVITY_URL)));
	connect(mNetworkReply, SIGNAL(finished()), this, SLOT(proxyTestFinished()));

	setEnabled(false);

	mTimeoutTimer->start(5000);
}

void SettingsDialog::proxyTestFinished()
{
	mTimeoutTimer->stop();

	QNetworkReply::NetworkError error = mNetworkReply->error();
	switch(error)
	{
	case QNetworkReply::NoError:
		QMessageBox::information(this, tr("Connectivity test ended"), tr("The connectivity test succeeded."));
		break;
	case QNetworkReply::ProxyAuthenticationRequiredError:
		QMessageBox::warning(this, tr("Connectivity test ended"), tr("The connectivity test failed: invalid username or password."));
		break;
	case QNetworkReply::OperationCanceledError:
		break;
	default:
		QMessageBox::warning(this, tr("Connectivity test ended"), tr("The connectivity test failed: incorrect proxy settings."));
		break;
	}

	mNetworkReply->deleteLater();
	mNetworkReply = 0;
	setEnabled(true);
}

void SettingsDialog::accept()
{
	if(ui->switchTextCode->keySequence() == ui->openEditorKey->keySequence() && ui->switchTextCode->keySequence().toString() != QString())
	{
		QMessageBox::warning(this, tr("Settings"), tr("You have set the same key sequence for switching text/code mode and opening the editor, please choose a different one."));
		return;
	}

	QSettings settings;

	//GENERAL
	settings.setValue("general/showLoadingWindow", ui->showLoadingWindow->isChecked());
	settings.setValue("general/showTaskbarIcon", ui->showTaskbarIcon->isChecked());
	settings.setValue("general/showWindowAfterExecution", ui->showWindowAfterExecution->isChecked());
	settings.setValue("general/addConsoleStartEndSeparators", ui->addStartEndSeparators->isChecked());
	settings.setValue("general/reopenLastScript", ui->reopenLastScript->isChecked());
	settings.setValue("general/maxRecentFiles", ui->maxRecentFiles->value());

	//ACTIONS
	settings.setValue("actions/showExecutionWindow", ui->executionWindowGroup->isChecked());
	settings.setValue("actions/executionWindowPosition", ui->executionWindowPosition->position());
	settings.setValue("actions/executionWindowScreen", ui->executionWindowPosition->screen());
	settings.setValue("actions/showConsoleWindow", ui->consoleWindowGroup->isChecked());
	settings.setValue("actions/consoleWindowPosition", ui->consoleWindowPosition->position());
	settings.setValue("actions/consoleWindowScreen", ui->consoleWindowPosition->screen());
	settings.setValue("actions/stopExecutionHotkey", QVariant::fromValue(ui->stopExecutionHotkey->keySequence()));
	settings.setValue("actions/pauseExecutionHotkey", QVariant::fromValue(ui->pauseExecutionHotkey->keySequence()));
	settings.setValue("actions/switchTextCode", QVariant::fromValue(ui->switchTextCode->keySequence()));
	settings.setValue("actions/openEditorKey", QVariant::fromValue(ui->openEditorKey->keySequence()));
	settings.setValue("actions/checkCodeSyntaxAutomatically", ui->checkCodeSyntaxAutomatically->isChecked());

	//NETWORK
#ifndef ACT_NO_UPDATER
	settings.setValue("network/updatesCheck", QVariant(ui->updatesCheck->currentIndex()));
#endif
	settings.setValue("network/proxyMode", proxyMode());
	settings.setValue("network/proxyHost", ui->proxyHost->text());
	settings.setValue("network/proxyPort", ui->proxyPort->text());
	settings.setValue("network/proxyUser", ui->proxyUser->text());
	settings.setValue("network/proxyPassword", ui->proxyPassword->text());
	settings.setValue("network/proxyType", ui->proxyType->currentIndex());

#ifdef Q_WS_WIN
	bool associateASCR = (ui->associateASCRCheckBox->checkState() == Qt::Checked);
	bool associateACOD = (ui->associateACODCheckBox->checkState() == Qt::Checked);

	if(mPreviousASCRAssociation != associateASCR)
	{
		if(associateASCR)
		{
			std::wstring valueData = L"ActionazScriptFile";
			SHSetValue(HKEY_CLASSES_ROOT, L".ascr", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));

			valueData = L"Actionaz Script File";
			SHSetValue(HKEY_CLASSES_ROOT, L"ActionazScriptFile", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));

			valueData = QString("%1,0").arg(QDir::toNativeSeparators(QApplication::applicationFilePath())).toStdWString();
			SHSetValue(HKEY_CLASSES_ROOT, L"ActionazScriptFile\\DefaultIcon", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));

			valueData = QString(QString("\"%1\" ").arg(QDir::toNativeSeparators(QApplication::applicationFilePath())) + "\"%1\"").toStdWString();
			SHSetValue(HKEY_CLASSES_ROOT, L"ActionazScriptFile\\shell\\open\\command", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));
		}
		else
		{
			RegDeleteKey(HKEY_CLASSES_ROOT, L".ascr");
			SHDeleteKey(HKEY_CLASSES_ROOT, L"ActionazScriptFile");
		}
	}

	if(mPreviousACODAssociation != associateACOD)
	{
		if(associateACOD)
		{
			std::wstring valueData = L"ActionazCodeFile";
			SHSetValue(HKEY_CLASSES_ROOT, L".acod", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));

			valueData = L"Actionaz Code File";
			SHSetValue(HKEY_CLASSES_ROOT, L"ActionazCodeFile", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));

			valueData = QString("%1,0").arg(QDir::toNativeSeparators(QApplication::applicationFilePath())).toStdWString();
			SHSetValue(HKEY_CLASSES_ROOT, L"ActionazCodeFile\\DefaultIcon", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));
		}
		else
		{
			RegDeleteKey(HKEY_CLASSES_ROOT, L".acod");
			SHDeleteKey(HKEY_CLASSES_ROOT, L"ActionazCodeFile");
		}
	}

	if(mPreviousASCRAssociation != associateASCR || mPreviousACODAssociation != associateACOD)
		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);
#endif

	QDialog::accept();
}

void SettingsDialog::done(int result)
{
	QSettings settings;

	if(mSystemTrayIcon)
		mSystemTrayIcon->setVisible(settings.value("general/showTaskbarIcon", QVariant(true)).toBool());

	if(mNetworkReply)
		mNetworkReply->abort();

	QDialog::done(result);
}

void SettingsDialog::setCustomProxyEnabled(bool enabled)
{
	ui->proxySettings->setEnabled(enabled);
}

int SettingsDialog::proxyMode() const
{
	if(ui->noProxyRadioButton->isChecked())
		return ActionTools::Settings::PROXY_NONE;
	else if(ui->systemProxyRadioButton->isChecked())
		return ActionTools::Settings::PROXY_SYSTEM;
	else
		return ActionTools::Settings::PROXY_CUSTOM;
}
