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

#include "settingsdialog.hpp"
#include "actiontools/screenpositionwidget.hpp"
#include "actiontools/settings.hpp"
#include "ui_settingsdialog.h"
#include "global.hpp"
#include "tools/languages.hpp"
#include "themeselection.hpp"

#ifdef Q_OS_WIN
#include "actiontools/registry.hpp"
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
	mNetworkReply(nullptr),
	mTimeoutTimer(new QTimer(this)),
	mSystemTrayIcon(systemTrayIcon),
	mPreviousASCRAssociation(false),
    mPreviousACODAssociation(false),
    mLocaleChangeWarning(false)
{
	ui->setupUi(this);

#ifdef Q_OS_UNIX
	ui->fileAssociationsLabel->setVisible(false);
	ui->associateASCRCheckBox->setVisible(false);
	ui->associateACODCheckBox->setVisible(false);
#endif

    connect(ui->noProxyRadioButton, &QRadioButton::clicked, this, &SettingsDialog::disableCustomProxy);
    connect(ui->systemProxyRadioButton, &QRadioButton::clicked, this, &SettingsDialog::disableCustomProxy);
    connect(ui->customProxyRadioButton, &QRadioButton::clicked, this, &SettingsDialog::enableCustomProxy);

	QSettings settings;

	ui->settingsTab->setCurrentIndex(settings.value(QStringLiteral("gui/settingsTab"), QVariant(0)).toInt());

	ui->noSysTrayLabel->setVisible(!QSystemTrayIcon::isSystemTrayAvailable());
	ui->noSysTrayMessagesLabel->setVisible(!QSystemTrayIcon::supportsMessages());

    ui->languageComboBox->clear();

    auto languages = Tools::Languages::languagesName();
    for(int languageIndex = 0; languageIndex < languages.first.size(); ++languageIndex)
    {
        const QString languageName = languages.first[languageIndex];
        const QString translatedLanguageName = languages.second[languageIndex];

        if(languageName.isEmpty())
            ui->languageComboBox->addItem(translatedLanguageName, languageIndex);
        else
        {
            QString countryName = languageName.split(QLatin1Char('_')).at(1).toLower();
            QIcon icon{QStringLiteral(":/images/flags/%1.png").arg(countryName)};

            ui->languageComboBox->addItem(icon, translatedLanguageName, languageIndex);
        }
    }

	//GENERAL
    ui->languageComboBox->setCurrentIndex(Tools::Languages::languageNameToIndex(settings.value(QStringLiteral("gui/locale"), Tools::Languages::languagesName().first.first()).toString()));
    ui->themeComboBox->setCurrentIndex(settings.value(QStringLiteral("gui/theme"), QVariant(static_cast<int>(ThemeSelection::Theme::Default))).toInt());
    ui->showLoadingWindow->setChecked(settings.value(QStringLiteral("gui/showLoadingWindow"), QVariant(true)).toBool());
    ui->preloadActionDialogsCheckBox->setChecked(settings.value(QStringLiteral("gui/preloadActionDialogs"), QVariant(false)).toBool());
    ui->showTaskbarIcon->setChecked(settings.value(QStringLiteral("gui/showTaskbarIcon"), QVariant(true)).toBool());
    ui->showWindowAfterExecution->setChecked(settings.value(QStringLiteral("gui/showWindowAfterExecution"), QVariant(true)).toBool());
    ui->addStartEndSeparators->setChecked(settings.value(QStringLiteral("gui/addConsoleStartEndSeparators"), QVariant(true)).toBool());
    ui->consoleMaxEntries->setValue(settings.value(QStringLiteral("gui/consoleMaxEntries"), QVariant(0)).toInt());
    ui->reopenLastScript->setChecked(settings.value(QStringLiteral("gui/reopenLastScript"), QVariant(false)).toBool());
    ui->maxRecentFiles->setValue(settings.value(QStringLiteral("gui/maxRecentFiles"), QVariant(5)).toInt());

	//ACTIONS
	ui->executionWindowGroup->setChecked(settings.value(QStringLiteral("actions/showExecutionWindow"), QVariant(true)).toBool());
	int screen = settings.value(QStringLiteral("actions/executionWindowScreen"), QVariant(0)).toInt();
	int position = settings.value(QStringLiteral("actions/executionWindowPosition"), QVariant(0)).toInt();
	ui->executionWindowPosition->setPosition(screen, position);
	ui->consoleWindowGroup->setChecked(settings.value(QStringLiteral("actions/showConsoleWindow"), QVariant(true)).toBool());
	screen = settings.value(QStringLiteral("actions/consoleWindowScreen"), QVariant(0)).toInt();
	position = settings.value(QStringLiteral("actions/consoleWindowPosition"), QVariant(1)).toInt();
	ui->consoleWindowPosition->setPosition(screen, position);
	ui->stopExecutionHotkey->setKeySequence(
		QKeySequence(settings.value(QStringLiteral("actions/stopExecutionHotkey"), QKeySequence(QStringLiteral("Ctrl+Alt+Q"))).toString()));
	ui->pauseExecutionHotkey->setKeySequence(
		QKeySequence(settings.value(QStringLiteral("actions/pauseExecutionHotkey"), QKeySequence(QStringLiteral("Ctrl+Alt+W"))).toString()));
	ui->switchTextCode->setKeySequence(
		QKeySequence(settings.value(QStringLiteral("actions/switchTextCode"), QKeySequence(QStringLiteral("Ctrl+Shift+C"))).toString()));
	ui->openEditorKey->setKeySequence(
		QKeySequence(settings.value(QStringLiteral("actions/openEditorKey"), QKeySequence(QStringLiteral("Ctrl+Shift+V"))).toString()));
	ui->checkCodeSyntaxAutomatically->setChecked(settings.value(QStringLiteral("actions/checkCodeSyntaxAutomatically"), QVariant(true)).toBool());
    ui->heatmapMinColorPushButton->setColor(settings.value(QStringLiteral("heatmap/minColor"), QColor{Qt::yellow}).value<QColor>());
    ui->heatmapMaxColorPushButton->setColor(settings.value(QStringLiteral("heatmap/maxColor"), QColor{Qt::red}).value<QColor>());

	//NETWORK
#ifndef ACT_UPDATER
	ui->updatesCheck->hide();
	ui->updatesCheckLabel->hide();
#else
	ui->updatesCheck->setCurrentIndex(settings.value(QStringLiteral("network/updatesCheck"), QVariant(ActionTools::Settings::CHECK_FOR_UPDATES_DAY)).toInt());
#endif

	int proxyMode = settings.value(QStringLiteral("network/proxyMode"), QVariant(ActionTools::Settings::PROXY_SYSTEM)).toInt();

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
	ui->proxyHost->setText(settings.value(QStringLiteral("network/proxyHost"), QVariant()).toString());
	ui->proxyPort->setText(settings.value(QStringLiteral("network/proxyPort"), QVariant()).toString());
	ui->proxyUser->setText(settings.value(QStringLiteral("network/proxyUser"), QVariant()).toString());
	ui->proxyPassword->setText(settings.value(QStringLiteral("network/proxyPassword"), QVariant()).toString());
	ui->proxyType->setCurrentIndex(settings.value(QStringLiteral("network/proxyType"), QVariant(ActionTools::Settings::PROXY_TYPE_HTTP)).toInt());

#ifdef Q_OS_WIN
	QVariant result;
	mPreviousASCRAssociation = (ActionTools::Registry::read(result, ActionTools::Registry::ClassesRoot, QStringLiteral(".ascr")) == ActionTools::Registry::ReadOk);
	mPreviousACODAssociation = (ActionTools::Registry::read(result, ActionTools::Registry::ClassesRoot, QStringLiteral(".acod")) == ActionTools::Registry::ReadOk);

	ui->associateASCRCheckBox->setChecked(mPreviousASCRAssociation);
	ui->associateACODCheckBox->setChecked(mPreviousACODAssociation);
#endif

    connect(ui->languageComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SettingsDialog::languageChanged);
    connect(mTimeoutTimer, &QTimer::timeout, this, &SettingsDialog::onTimeout);

	if(systemTrayIcon)
        connect(ui->showTaskbarIcon, &QCheckBox::clicked, systemTrayIcon, &QSystemTrayIcon::setVisible);
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

    connect(mNetworkReply, &QNetworkReply::finished, this, &SettingsDialog::proxyTestFinished);

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
	mNetworkReply = nullptr;
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

	settings.setValue(QStringLiteral("gui/settingsTab"), ui->settingsTab->currentIndex());

	//GENERAL
    settings.setValue(QStringLiteral("gui/locale"), Tools::Languages::languagesName().first[ui->languageComboBox->currentIndex()]);
    settings.setValue(QStringLiteral("gui/theme"), QVariant(ui->themeComboBox->currentIndex()));
    settings.setValue(QStringLiteral("gui/showLoadingWindow"), ui->showLoadingWindow->isChecked());
    settings.setValue(QStringLiteral("gui/preloadActionDialogs"), ui->preloadActionDialogsCheckBox->isChecked());
	settings.setValue(QStringLiteral("gui/showTaskbarIcon"), ui->showTaskbarIcon->isChecked());
	settings.setValue(QStringLiteral("gui/showWindowAfterExecution"), ui->showWindowAfterExecution->isChecked());
	settings.setValue(QStringLiteral("gui/addConsoleStartEndSeparators"), ui->addStartEndSeparators->isChecked());
    settings.setValue(QStringLiteral("gui/consoleMaxEntries"), ui->consoleMaxEntries->value());
	settings.setValue(QStringLiteral("gui/reopenLastScript"), ui->reopenLastScript->isChecked());
	settings.setValue(QStringLiteral("gui/maxRecentFiles"), ui->maxRecentFiles->value());

	//ACTIONS
	settings.setValue(QStringLiteral("actions/showExecutionWindow"), ui->executionWindowGroup->isChecked());
	settings.setValue(QStringLiteral("actions/executionWindowPosition"), ui->executionWindowPosition->position());
	settings.setValue(QStringLiteral("actions/executionWindowScreen"), ui->executionWindowPosition->screen());
	settings.setValue(QStringLiteral("actions/showConsoleWindow"), ui->consoleWindowGroup->isChecked());
	settings.setValue(QStringLiteral("actions/consoleWindowPosition"), ui->consoleWindowPosition->position());
	settings.setValue(QStringLiteral("actions/consoleWindowScreen"), ui->consoleWindowPosition->screen());
	settings.setValue(QStringLiteral("actions/stopExecutionHotkey"), QVariant::fromValue(ui->stopExecutionHotkey->keySequence()));
	settings.setValue(QStringLiteral("actions/pauseExecutionHotkey"), QVariant::fromValue(ui->pauseExecutionHotkey->keySequence()));
	settings.setValue(QStringLiteral("actions/switchTextCode"), QVariant::fromValue(ui->switchTextCode->keySequence()));
	settings.setValue(QStringLiteral("actions/openEditorKey"), QVariant::fromValue(ui->openEditorKey->keySequence()));
    settings.setValue(QStringLiteral("actions/checkCodeSyntaxAutomatically"), ui->checkCodeSyntaxAutomatically->isChecked());
    settings.setValue(QStringLiteral("heatmap/minColor"), ui->heatmapMinColorPushButton->color());
    settings.setValue(QStringLiteral("heatmap/maxColor"), ui->heatmapMaxColorPushButton->color());

	//NETWORK
#ifdef ACT_UPDATER
	settings.setValue(QStringLiteral("network/updatesCheck"), QVariant(ui->updatesCheck->currentIndex()));
#endif
	settings.setValue(QStringLiteral("network/proxyMode"), proxyMode());
	settings.setValue(QStringLiteral("network/proxyHost"), ui->proxyHost->text());
	settings.setValue(QStringLiteral("network/proxyPort"), ui->proxyPort->text());
	settings.setValue(QStringLiteral("network/proxyUser"), ui->proxyUser->text());
	settings.setValue(QStringLiteral("network/proxyPassword"), ui->proxyPassword->text());
	settings.setValue(QStringLiteral("network/proxyType"), ui->proxyType->currentIndex());

#ifdef Q_OS_WIN
	bool associateASCR = (ui->associateASCRCheckBox->checkState() == Qt::Checked);
	bool associateACOD = (ui->associateACODCheckBox->checkState() == Qt::Checked);

	if(mPreviousASCRAssociation != associateASCR)
	{
		if(associateASCR)
		{
            std::wstring valueData = L"ActionaScriptFile";
			SHSetValue(HKEY_CLASSES_ROOT, L".ascr", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));

            valueData = L"Actiona Script File";
            SHSetValue(HKEY_CLASSES_ROOT, L"ActionaScriptFile", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));

            valueData = QStringLiteral("%1,0").arg(QDir::toNativeSeparators(QApplication::applicationFilePath())).toStdWString();
            SHSetValue(HKEY_CLASSES_ROOT, L"ActionaScriptFile\\DefaultIcon", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));

            valueData = QString(QStringLiteral("\"%1\" ").arg(QDir::toNativeSeparators(QApplication::applicationFilePath())) + QStringLiteral("\"%1\"")).toStdWString();
            SHSetValue(HKEY_CLASSES_ROOT, L"ActionaScriptFile\\shell\\open\\command", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));
		}
		else
		{
			RegDeleteKey(HKEY_CLASSES_ROOT, L".ascr");
            SHDeleteKey(HKEY_CLASSES_ROOT, L"ActionaScriptFile");
		}
	}

	if(mPreviousACODAssociation != associateACOD)
	{
		if(associateACOD)
		{
            std::wstring valueData = L"ActionaCodeFile";
			SHSetValue(HKEY_CLASSES_ROOT, L".acod", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));

            valueData = L"Actiona Code File";
            SHSetValue(HKEY_CLASSES_ROOT, L"ActionaCodeFile", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));

            valueData = QStringLiteral("%1,0").arg(QDir::toNativeSeparators(QApplication::applicationFilePath())).toStdWString();
            SHSetValue(HKEY_CLASSES_ROOT, L"ActionaCodeFile\\DefaultIcon", 0, REG_SZ, valueData.c_str(), static_cast<DWORD>(valueData.size() * sizeof(wchar_t)));
		}
		else
		{
			RegDeleteKey(HKEY_CLASSES_ROOT, L".acod");
            SHDeleteKey(HKEY_CLASSES_ROOT, L"ActionaCodeFile");
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
		mSystemTrayIcon->setVisible(settings.value(QStringLiteral("gui/showTaskbarIcon"), QVariant(true)).toBool());

	if(mNetworkReply)
		mNetworkReply->abort();

    QDialog::done(result);
}

void SettingsDialog::languageChanged()
{
    if(mLocaleChangeWarning)
        return;

    mLocaleChangeWarning = true;

    QMessageBox::information(this, tr("Language change"), tr("The language change will be taken into account next time you restart Actiona."));
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


