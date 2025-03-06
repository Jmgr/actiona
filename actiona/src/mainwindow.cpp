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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "actiontools/actionfactory.hpp"
#include "actiontools/actiondefinition.hpp"
#include "actiontools/actioninstancebuffer.hpp"
#include "actiontools/script.hpp"
#include "scriptmodel.hpp"
#include "global.hpp"
#include "actiondialog.hpp"
#include "actiontools/codeeditordialog.hpp"
#include "aboutdialog.hpp"
#include "settingsdialog.hpp"
#include "actiontools/settings.hpp"
#include "scriptparametersdialog.hpp"
#include "actiontools/crossplatform.hpp"
#include "execution/execution.hpp"
#include "newactiondialog.hpp"
#include "scriptcontentdialog.hpp"
#include "actiontools/keywords.hpp"
#include "QHotkey/QHotkey"
#ifdef ACT_UPDATER
#include "changelogdialog.hpp"
#include "tools/updater.hpp"
#endif
#ifdef Q_OS_WIN
#include "tools/sevenziparchivewrite.hpp"
#endif
#include "actiontools/actionpack.hpp"
#ifdef Q_OS_WIN
#include "sfxscriptdialog.hpp"
#endif
#include "progresssplashscreen.hpp"
#include "execution/codeinitializer.hpp"
#include "scriptsettingsdialog.hpp"
#include "resourcedialog.hpp"
#include "actiontools/screenshotwizard.hpp"
#include "newactionmodel.hpp"
#include "newactionproxymodel.hpp"
#include "scriptproxymodel.hpp"
#include "tools/languages.hpp"
#include "actionpackdata.hpp"
#include "actionpackdevice.hpp"
#include "actionpackinternal.hpp"
#include "actionpacksystem.hpp"
#include "actionpackwindows.hpp"
#include "themeselection.hpp"

#ifdef ACT_PROFILE
#include "tools/highresolutiontimer.hpp"
#endif

#include <QSystemTrayIcon>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QTimer>
#include <QUndoGroup>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QColorDialog>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QBuffer>
#include <QNetworkProxy>
#include <QCloseEvent>
#include <QProgressDialog>
#include <QProcess>
#include <QTemporaryFile>
#include <QListWidget>
#include <QJSValueIterator>
#include <QStandardPaths>
#include <QCommandLineParser>

#ifdef Q_OS_UNIX
#include <QProcessEnvironment>
#include "actiontools/x11info.hpp"
#endif

#include <algorithm>

MainWindow::MainWindow(QCommandLineParser &commandLineParser, ProgressSplashScreen *splashScreen, const QString &startScript, const QString &usedLocale)
	: QMainWindow(nullptr),
	ui(new Ui::MainWindow),
	mScriptModified(false),
	mActionFactory(new ActionTools::ActionFactory(this)),
	mScript(new ActionTools::Script(mActionFactory, this)),
	mScriptModel(new ScriptModel(mScript, mActionFactory, this)),
	mSystemTrayIcon(commandLineParser.isSet(QStringLiteral("notrayicon")) ? nullptr : new QSystemTrayIcon(QIcon(QStringLiteral(":/icons/logo.png")), this)),
	mSplashScreen(splashScreen),
	mWasNewActionDockShown(false),
	mWasConsoleDockShown(false),
	mUndoGroup(new QUndoGroup(this)),
	mCompletionModel(new QStandardItemModel(this)),
	mStartScript(startScript),
	mCommandLineParser(commandLineParser),
	mAddActionRow(0),
	mStopExecutionAction(new QAction(tr("S&top execution"), this)),
    mUsedLocale(usedLocale),
    mNewActionProxyModel(new NewActionProxyModel(this)),
    mScriptProxyModel(new ScriptProxyModel(mScript, this)),
    mNewActionModel(new NewActionModel(this)),
    mStartStopExecutionHotkey(new QHotkey(this)),
    mPauseExecutionHotkey(new QHotkey(this))
#ifdef ACT_UPDATER
	,mNetworkAccessManager(new QNetworkAccessManager(this)),
	mUpdateDownloadNetworkReply(nullptr),
	mUpdater(new Tools::Updater(mNetworkAccessManager, Global::UPDATE_URL, Global::UPDATE_TIMEOUT, this)),
	mUpdaterProgressDialog(new QProgressDialog(this)),
	mHashCalculator(QCryptographicHash::Md5)
#endif
{
#ifdef ACT_PROFILE
    Tools::HighResolutionTimer timer(QStringLiteral("MainWindow constructor"));
#endif

    setEnabled(false);

	ui->setupUi(this);

    ui->scriptFilterCriteriaFlagsComboBox->addFlag(tr("Label"), static_cast<unsigned int>(ActionFilteringFlag::Label));
    ui->scriptFilterCriteriaFlagsComboBox->addFlag(tr("Action name"), static_cast<unsigned int>(ActionFilteringFlag::ActionName));
    ui->scriptFilterCriteriaFlagsComboBox->addFlag(tr("Comment"), static_cast<unsigned int>(ActionFilteringFlag::Comment));
    ui->scriptFilterCriteriaFlagsComboBox->addFlag(tr("Code parameters"), static_cast<unsigned int>(ActionFilteringFlag::CodeParameters));
    ui->scriptFilterCriteriaFlagsComboBox->addFlag(tr("Text parameters"), static_cast<unsigned int>(ActionFilteringFlag::TextParameters));
    ui->scriptFilterCriteriaFlagsComboBox->setAllCheckedText(tr("Everything"));
    ui->scriptFilterCriteriaFlagsComboBox->setNoneCheckedText(tr("Everything"));

    mNewActionProxyModel->setDynamicSortFilter(false);
    mScriptProxyModel->setDynamicSortFilter(false);

#ifdef Q_OS_UNIX
    auto environment = QProcessEnvironment::systemEnvironment();
	auto sessionType = environment.value(QStringLiteral("XDG_SESSION_TYPE"), QStringLiteral("x11")); // Consider an empty value as being X11
	auto x11Session = (sessionType == QLatin1String("x11"));

    connect(ui->x11NotDetectedLabel, &QLabel::linkActivated, [](const QString &link)
    {
        if(link == QLatin1String("x11notdetected"))
            QDesktopServices::openUrl(QUrl(QStringLiteral("https://wiki.actiona.tools/doku.php?id=%1:x11notdetected").arg(Tools::Languages::locale().mid(0, 2))));
    });

    ui->x11NotDetectedLabel->setVisible(!x11Session);
    ui->x11NotDetectedIconLabel->setVisible(!x11Session);
#else
    ui->x11NotDetectedLabel->setVisible(false);
    ui->x11NotDetectedIconLabel->setVisible(false);
#endif

#ifdef ACT_UPDATER
    mUpdaterProgressDialog->close();
#endif

	ui->consoleWidget->setup();

	if(mSplashScreen)
	{
		mSplashScreen->showMessage(tr("Creating main window..."));
		mSplashScreen->setValue(0);
		mSplashScreen->setMaximum(1);
	}

	setUnifiedTitleAndToolBarOnMac(true);

#ifndef ACT_UPDATER
	const QList<QAction*> &actionList = ui->menuTools->actions();
	for(int i= 0; i <= 1 && i < actionList.count(); ++i)
	{
		actionList.at(i)->setVisible(false);
	}
#endif

	mStopExecutionAction->setEnabled(false);

	if(mSystemTrayIcon)
	{
		auto trayMenu = new QMenu(this);
		trayMenu->addAction(ui->actionExecute);
		trayMenu->addAction(ui->actionExecute_selection);
		trayMenu->addSeparator();
		trayMenu->addAction(mStopExecutionAction);
		trayMenu->addSeparator();
		trayMenu->addAction(ui->actionQuit);

        mSystemTrayIcon->setToolTip(tr("Actiona - ready"));
		mSystemTrayIcon->setContextMenu(trayMenu);
	}

	mUndoGroup->addStack(mScriptModel->undoStack());
	mScriptModel->undoStack()->setActive(true);

#ifndef Q_OS_WIN
	ui->actionExport_executable->setEnabled(false);
	ui->actionExport_executable->setVisible(false);
#endif

#ifdef Q_OS_WIN
    {
		QFileInfo sfxFileInfo(QDir(QApplication::applicationDirPath()).filePath(QStringLiteral("sfx/7zsd.sfx")));
		QFileInfo sfxBaseArchiveInfo(QDir(QApplication::applicationDirPath()).filePath(QStringLiteral("sfx/sfx.7z")));
        QFileInfo sfx64BitArchiveInfo(QDir(QApplication::applicationDirPath()).filePath(QStringLiteral("sfx/sfx64.7z")));

        ui->actionExport_executable->setEnabled(sfxFileInfo.isReadable() &&
                                                sfxBaseArchiveInfo.isReadable() &&
                                                sfx64BitArchiveInfo.isReadable());
    }
#endif

	ui->scriptView->setIconSize(QSize(16, 16));
    ui->scriptView->header()->setResizeContentsPrecision(0);

    {
        QItemSelectionModel *oldModel = ui->newActionTreeView->selectionModel();
        mNewActionProxyModel->setSourceModel(mNewActionModel);
        ui->newActionTreeView->setModel(mNewActionProxyModel);
        delete oldModel;
    }

    {
        QItemSelectionModel *oldModel = ui->scriptView->selectionModel();
        mScriptProxyModel->setSourceModel(mScriptModel);
        ui->scriptView->setModel(mScriptProxyModel);
        delete oldModel;
    }

	mScriptModel->setSelectionModel(ui->scriptView->selectionModel());
    mScriptModel->setProxyModel(mScriptProxyModel);

    ui->scriptView->header()->setSectionsMovable(true);

	ui->actionQuit->setShortcut(QKeySequence(tr("Alt+F4")));
	readSettings();

	updateProxySettings();

	for(int i=0; i<mMaxRecentFiles; ++i)
	{
		auto newAction = new QAction(this);
		mRecentFileActs.append(newAction);
		newAction->setVisible(false);
        connect(newAction, &QAction::triggered, this, &MainWindow::openRecentFile);


		ui->menuRecent_scripts->addAction(newAction);
	}
	updateRecentFileActions();

    connect(ui->deleteDropTarget, &DeleteActionPushButton::actionsDropped, mScriptModel, &ScriptModel::removeActions);
    connect(ui->scriptView, &ScriptTreeView::doubleClicked, this, static_cast<void (MainWindow::*)(const QModelIndex &)>(&MainWindow::editAction));
    connect(mScriptModel, &ScriptModel::wantToAddAction, this, static_cast<void (MainWindow::*)(int, const QString &)>(&MainWindow::wantToAddAction));
    connect(mScriptModel, &ScriptModel::scriptFileDropped, this, &MainWindow::scriptFileDropped);
    connect(mScriptModel, &ScriptModel::scriptContentDropped, this, &MainWindow::scriptContentDropped);
    connect(ui->scriptView->selectionModel(), &QItemSelectionModel::selectionChanged, this, static_cast<void (MainWindow::*)()>(&MainWindow::actionSelectionChanged));
    connect(mScriptModel, &ScriptModel::scriptEdited, this, &MainWindow::scriptEdited);
    connect(ui->newActionTreeView, &QTreeView::doubleClicked, this, &MainWindow::newActionDoubleClicked);
    if(mSystemTrayIcon)
        connect(mSystemTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::systemTrayIconActivated);
    connect(mActionFactory, &ActionTools::ActionFactory::actionPackLoadError, this, &MainWindow::packLoadError);
    connect(ui->consoleWidget, &ActionTools::ConsoleWidget::itemDoubleClicked, this, &MainWindow::logItemDoubleClicked);
    connect(ui->consoleWidget, &ActionTools::ConsoleWidget::itemClicked, this, static_cast<void (MainWindow::*)(int)>(&MainWindow::logItemClicked));
    connect(mStopExecutionAction, &QAction::triggered, this, &MainWindow::stopExecution);
    connect(&mExecuter, &Execution::Executer::executionStopped, this, &MainWindow::scriptExecutionStopped);
    connect(ui->heatmapModeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this](int index)
    {
        mScriptModel->setHeatmapMode(static_cast<HeatmapMode>(index));

        ui->scriptView->viewport()->update();
        ui->scriptView->header()->viewport()->update();
    });
    connect(mStartStopExecutionHotkey, &QHotkey::activated, this, &MainWindow::startOrStopExecution);
    connect(mPauseExecutionHotkey, &QHotkey::activated, this, &MainWindow::pauseOrResumeExecution);
#ifdef ACT_UPDATER
    connect(mUpdater, &Tools::Updater::error, this, &MainWindow::updateError);
    connect(mUpdater, &Tools::Updater::noResult, this, &MainWindow::updateNoResult);
    connect(mUpdater, &Tools::Updater::success, this, &MainWindow::updateSuccess);
#endif

	setWindowTitle(QStringLiteral("Actiona[*]"));//Set this to fix some warnings about the [*] placeholder

    QTimer::singleShot(0, this, SLOT(postInit()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::postInit()
{
    QApplication::processEvents();

#ifdef ACT_PROFILE
    Tools::HighResolutionTimer timer(QStringLiteral("postInit"));
#endif

	mPackLoadErrors.clear();

	if(mSplashScreen)
        mSplashScreen->showMessage(tr("Loading actions..."));

    mActionFactory->loadActionPacks({
                                        new ActionPackData(),
                                        new ActionPackDevice(),
                                        new ActionPackInternal(),
                                        new ActionPackSystem(),
                                        new ActionPackWindows(),
                                    }, mUsedLocale);

	QSettings settings;

	{
#ifdef ACT_PROFILE
        Tools::HighResolutionTimer timer(QStringLiteral("building completion model"));
#endif

        ActionTools::ScriptEngine engine;
        Execution::CodeInitializer::initialize(engine, mActionFactory, mCurrentFile);

		mCompletionModel->appendRow(new QStandardItem(QIcon(QStringLiteral(":/icons/class.png")), QStringLiteral("include")));
		mCompletionModel->appendRow(new QStandardItem(QIcon(QStringLiteral(":/icons/class.png")), QStringLiteral("loadUI")));
		
		QJSValueIterator it(engine.globalObject());
		while(it.hasNext())
		{
			it.next();
			
			if(!it.value().isQMetaObject())
				continue;

			mCompletionModel->appendRow(new QStandardItem(QIcon(QStringLiteral(":/icons/class.png")), it.name()));
		}
	}

    if(settings.value(QStringLiteral("gui/preloadActionDialogs"), false).toBool())
	{
#ifdef ACT_PROFILE
        Tools::HighResolutionTimer timer(QStringLiteral("action dialogs creation"));
#endif
		if(mSplashScreen)
			mSplashScreen->setMaximum(mActionFactory->actionDefinitionCount() - 1);

		for(int actionDefinitionIndex = 0; actionDefinitionIndex < mActionFactory->actionDefinitionCount(); ++actionDefinitionIndex)
		{
			ActionTools::ActionDefinition *actionDefinition = mActionFactory->actionDefinition(actionDefinitionIndex);

			if(mSplashScreen)
			{
				mSplashScreen->showMessage(tr("Creating action dialog %1...").arg(actionDefinition->name()));
				mSplashScreen->setValue(actionDefinitionIndex);
			}

            getOrCreateActionDialog(actionDefinition);

            QApplication::processEvents();
		}
	}

	if(mSplashScreen)
	{
		mSplashScreen->showMessage(tr("Please wait..."));
		mSplashScreen->setValue(0);
		mSplashScreen->setMaximum(1);
	}

	{
#ifdef ACT_PROFILE
        Tools::HighResolutionTimer timer(QStringLiteral("adding Ecmascript stuff"));
#endif
		//Add Ecmascript stuff
		ActionTools::addEcmaScriptObjectsKeywords(mCompletionModel);
	}

	{
#ifdef ACT_PROFILE
        Tools::HighResolutionTimer timer(QStringLiteral("filling NewActionModel"));
#endif
        fillNewActionModel();
	}

	statusBar()->showMessage(tr("Ready, loaded %1 actions from %2 packs").arg(mActionFactory->actionDefinitionCount()).arg(mActionFactory->actionPackCount()));

	ui->actionActions_window->setChecked(ui->actionsDockWidget->isVisible());
	ui->actionConsole_window->setChecked(ui->consoleDockWidget->isVisible());
	ui->actionToolbar->setChecked(ui->toolBar->isVisible());

	updateUndoRedoStatus();
	actionCountChanged();

	if(mSplashScreen)
	{
		mSplashScreen->fadeOut();
		mSplashScreen = nullptr;
	}

#ifdef Q_OS_UNIX
	ActionTools::CrossPlatform::setForegroundWindow(this);
#endif

	setCurrentFile(QString());

    QApplication::processEvents();

	{
#ifdef ACT_PROFILE
        Tools::HighResolutionTimer timer(QStringLiteral("loading last file"));
#endif
		if(!mStartScript.isEmpty())
		{
			if(!loadFile(mStartScript))
			{
				if(mCommandLineParser.isSet(QStringLiteral("execute")))
					QApplication::quit();
			}
		}
		else
		{
			if(settings.value(QStringLiteral("gui/reopenLastScript"), QVariant(false)).toBool())
			{
				QString lastFilename = settings.value(QStringLiteral("gui/lastScript"), QString()).toString();

				if(!lastFilename.isEmpty())
				{
					if(!loadFile(lastFilename, false))
						settings.setValue(QStringLiteral("gui/lastScript"), QString());
				}
			}
		}
	}

	if(mPackLoadErrors.count() > 0)
	{
		QString message = tr("<b>Unable to load %n action(s):</b>\n", "", mPackLoadErrors.count());
		message += QStringLiteral("<ul>");

		for(const QString &error: qAsConst(mPackLoadErrors))
		{
			message += QStringLiteral("<li>") + error + QStringLiteral("</li>");
		}

		message += QStringLiteral("</ul>");

		QMessageBox::critical(this, tr("Error while loading actions"), message);
	}

#ifdef ACT_UPDATER
	if(!mCommandLineParser.isSet(QStringLiteral("execute")) && settings.value(QStringLiteral("network/updatesCheck"), QVariant(ActionTools::Settings::CHECK_FOR_UPDATES_UNKNOWN)) == ActionTools::Settings::CHECK_FOR_UPDATES_UNKNOWN)
	{
		if(QMessageBox::question(this,
								 tr("Automatic updates"),
                                 tr("Do you want Actiona to check once per day if a new version is available ?\nYou can change this setting later in the settings dialog."),
								 QMessageBox::Yes | QMessageBox::No,
								 QMessageBox::Yes) == QMessageBox::Yes)
			settings.setValue(QStringLiteral("network/updatesCheck"), QVariant(ActionTools::Settings::CHECK_FOR_UPDATES_DAY));
		else
			settings.setValue(QStringLiteral("network/updatesCheck"), QVariant(ActionTools::Settings::CHECK_FOR_UPDATES_NEVER));
	}

	int checkFrequency = settings.value(QStringLiteral("network/updatesCheck"), QVariant(ActionTools::Settings::CHECK_FOR_UPDATES_NEVER)).toInt();
	if(!mCommandLineParser.isSet(QStringLiteral("execute")) && checkFrequency != ActionTools::Settings::CHECK_FOR_UPDATES_NEVER)
	{
		QDateTime lastCheck = settings.value(QStringLiteral("network/lastCheck"), QDateTime()).toDateTime();
		bool check = false;
		if(lastCheck == QDateTime())
			check = true;
		else
		{
			switch(checkFrequency)
			{
			case ActionTools::Settings::CHECK_FOR_UPDATES_DAY:
				if(lastCheck.daysTo(QDateTime::currentDateTime()) >= 1)
					check = true;
				break;
			case ActionTools::Settings::CHECK_FOR_UPDATES_WEEK:
				if(lastCheck.daysTo(QDateTime::currentDateTime()) >= 7)
					check = true;
				break;
			case ActionTools::Settings::CHECK_FOR_UPDATES_MONTH:
				if(lastCheck.daysTo(QDateTime::currentDateTime()) >= 30)
					check = true;
				break;
			default:
				break;
			}
		}

		if(check)
		{
			settings.setValue(QStringLiteral("network/lastCheck"), QDateTime::currentDateTime());

			checkForUpdate(true);
		}
	}
#endif

	const QString &startStopExecutionHotkey = settings.value(QStringLiteral("actions/stopExecutionHotkey"), QKeySequence(QStringLiteral("Ctrl+Alt+Q"))).toString();
	if(!startStopExecutionHotkey.isEmpty())
        mStartStopExecutionHotkey->setShortcut(QKeySequence(startStopExecutionHotkey), true);
	const QString &pauseExecutionHotkey = settings.value(QStringLiteral("actions/pauseExecutionHotkey"), QKeySequence(QStringLiteral("Ctrl+Alt+W"))).toString();
	if(!pauseExecutionHotkey.isEmpty())
        mPauseExecutionHotkey->setShortcut(QKeySequence(pauseExecutionHotkey), true);

	if(mCommandLineParser.isSet(QStringLiteral("execute")))
		execute(false);
    else if(Global::ACTIONA_VERSION < QVersionNumber(1, 0, 0))
	{
		if(!settings.value(QStringLiteral("hasGotPreVersionMessage"), false).toBool())
		{
			settings.setValue(QStringLiteral("hasGotPreVersionMessage"), true);

			QMessageBox::information(this, tr("Beta test"),
                                     tr("Thank you for beta-testing this new version of Actiona !<br>"
										"<br>Please test as many features as you can, and remember that any comments are welcome !<br><br>To report a bug or write a comment please use the <b>Report a bug</b> button.<br>"
										"<br>Remember that this is a <b>beta</b> version so please do not write any critical scripts with it since the script format may change before the final release.<br>"));
		}
	}

	actionSelectionChanged();

    setEnabled(true);

    ui->scriptView->setFocus();
}

void MainWindow::on_actionSave_triggered()
{
	save();
}

void MainWindow::on_actionSave_as_triggered()
{
	saveAs();
}

void MainWindow::on_actionSave_copy_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save copy"), QString(), tr("Actiona script (*.ascr)"));
	if(fileName.isEmpty())
		return;

	QFileInfo fileInfo(fileName);
	if(fileInfo.suffix().isEmpty())
		fileName += QStringLiteral(".ascr");

	saveFile(fileName, true);
}

void MainWindow::on_actionOpen_triggered()
{
	if(maybeSave())
	{
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open script"), QString(), tr("Actiona script (*.ascr)"));
		if(!fileName.isEmpty())
			loadFile(fileName);
	}
}

void MainWindow::on_actionNew_triggered()
{
	if(maybeSave())
	{
		mScriptModel->reset();
		mScript->removeAllParameters();
        mScript->clearResources();
        mScript->setPauseBefore(0);
        mScript->setPauseAfter(0);
		setCurrentFile(QString());

		actionCountChanged();
	}
}

void MainWindow::on_actionQuit_triggered()
{
	QApplication::quit();
}

void MainWindow::on_deleteDropTarget_clicked()
{
	deleteSelection();
}

void MainWindow::on_actionDelete_action_triggered()
{
	deleteSelection();
}

void MainWindow::on_actionDelete_all_actions_triggered()
{
	if(QMessageBox::question(this, tr("Delete all actions"),
		tr("Do you really want to delete all the actions contained in this script ?"),
		QMessageBox::Yes | QMessageBox::No,
		QMessageBox::Yes) != QMessageBox::Yes)
		return;

	scriptWasModified(true);
	mScriptModel->reset();

	actionSelectionChanged();
	actionCountChanged();
}

void MainWindow::on_actionSelect_all_actions_triggered()
{
	ui->scriptView->selectAll();
}

void MainWindow::on_actionSelect_none_triggered()
{
	ui->scriptView->clearSelection();
}

void MainWindow::on_actionInverse_selection_triggered()
{
    auto selection = mScriptProxyModel->mapSelectionFromSource(
                QItemSelection(mScriptModel->index(0, 0),
                               mScriptModel->index(mScriptModel->rowCount() - 1, 0)));

	ui->scriptView->selectionModel()->select(
            selection,
			QItemSelectionModel::Toggle | QItemSelectionModel::Rows);
}

void MainWindow::on_actionAbout_triggered()
{
	AboutDialog aboutDialog(this);

    aboutDialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);

	aboutDialog.exec();
}

void MainWindow::on_actionClear_triggered()
{
	QSettings settings;
    settings.setValue(QStringLiteral("recentFileList"), QStringList{});

	updateRecentFileActions();
}

void MainWindow::on_actionExport_executable_triggered()
{
#ifdef Q_OS_WIN
	QSettings settings;
	QString fileName = QFileDialog::getSaveFileName(this, tr("Choose the SFX script destination"), settings.value(QStringLiteral("sfxScript/destination")).toString(), tr("Executable file (*.exe)"));
	if(fileName.isEmpty())
		return;

	settings.setValue(QStringLiteral("sfxScript/destination"), fileName);

	SFXScriptDialog sfxScriptDialog(this);

    sfxScriptDialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);

	if(!sfxScriptDialog.exec())
		return;

	bool useActExec = (sfxScriptDialog.closeAfterExecution()
					   && sfxScriptDialog.disableTrayIcon()
					   && !sfxScriptDialog.showConsole()
					   && !sfxScriptDialog.showExecutionWindow());

	QString parameters(QStringLiteral("es"));//Execute the current script & disable splash screen
	if(sfxScriptDialog.disableTrayIcon())
		parameters += QStringLiteral("t");
	if(!sfxScriptDialog.showConsole())
		parameters += QStringLiteral("C");
	if(!sfxScriptDialog.showExecutionWindow())
		parameters += QStringLiteral("E");
	if(sfxScriptDialog.closeAfterExecution())
		parameters += QStringLiteral("x");

	const QString archivePath = QDir::temp().filePath(QStringLiteral("archive.7z"));
	const QString sfxPath = QDir(QApplication::applicationDirPath()).filePath(QStringLiteral("sfx/7zsd.sfx"));
	const QString scriptPath = QDir::temp().filePath(QStringLiteral("script.ascr"));
	QString sourceArchive;

    if(sfxScriptDialog.requiresActiona())
		sourceArchive = QDir(QApplication::applicationDirPath()).filePath(QStringLiteral("sfx/sfx.7z"));
	else
        sourceArchive = QDir(QApplication::applicationDirPath()).filePath(QStringLiteral("sfx/sfx64.7z"));

	QProgressDialog progressDialog(tr("Creating SFX script"), QString(), 0, 100, this);
	progressDialog.setWindowTitle(tr("Create SFX script"));
	progressDialog.setModal(true);
	progressDialog.setValue(0);
	progressDialog.setVisible(true);
	progressDialog.setLabelText(tr("Copying archive..."));
	QApplication::processEvents();

	QFile::remove(archivePath);
	if(!QFile::copy(sourceArchive, archivePath))
	{
		QMessageBox::warning(this, tr("Create SFX script"), tr("Unable to copy the source archive."));
		return;
	}

	progressDialog.setLabelText(tr("Writing script..."));
	QApplication::processEvents();

	//Write the script
	QFile file(scriptPath);
	if(!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, tr("Create SFX script"), tr("Unable to write the script to %1.").arg(scriptPath));
		return;
	}
    mScript->write(&file, Global::ACTIONA_VERSION, Global::SCRIPT_VERSION);
	file.close();

    progressDialog.setLabelText(tr("Writing config file..."));
	QApplication::processEvents();

	//Write the config file
	QString configFile;
	QTextStream configFileStream(&configFile);
	configFileStream << QStringLiteral(";!@Install@!UTF-8!") << QStringLiteral("\r\n");
	configFileStream << QStringLiteral("ExecuteFile=\"runner.exe\"") << QStringLiteral("\r\n");
	if(useActExec)
		configFileStream << QStringLiteral("ExecuteParameters=\"%1 open actexec \\\"script.ascr\\\"\"").arg(mUsedLocale) << QStringLiteral("\r\n");
	else
		configFileStream << QStringLiteral("ExecuteParameters=\"%1 open actiona \\\"-%2 script.ascr\\\"\"").arg(mUsedLocale).arg(parameters) << QStringLiteral("\r\n");

	configFileStream << QStringLiteral("GUIMode=\"2\"") << QStringLiteral("\r\n");
	configFileStream << QStringLiteral(";!@InstallEnd@!");

	Tools::SevenZipArchiveWrite archive(archivePath);

	progressDialog.setLabelText(tr("Adding files..."));
	QApplication::processEvents();

	if(!archive.addFile(QDir(QApplication::applicationDirPath()).filePath(scriptPath)))
	{
		QFile::remove(archivePath);
		QFile::remove(scriptPath);
		QMessageBox::warning(this, tr("Create SFX script"), tr("Failed to add the script file to the SFX archive."));
		return;
	}

	QFile outFile(fileName);
	if(!outFile.open(QIODevice::WriteOnly))
	{
		QFile::remove(archivePath);
		QFile::remove(scriptPath);
		QMessageBox::warning(this, tr("Create SFX script"), tr("Unable to open %1 for writing.").arg(fileName));
		return;
	}

	//Copy the sfx stub
	QFile sfxStubFile(sfxPath);
	if(!sfxStubFile.open(QIODevice::ReadOnly))
	{
		outFile.close();
		QFile::remove(archivePath);
		QFile::remove(scriptPath);
		QFile::remove(fileName);
		QMessageBox::warning(this, tr("Create SFX script"), tr("Unable to open %1 for reading.").arg(sfxPath));
		return;
	}
	outFile.write(sfxStubFile.readAll());
	sfxStubFile.close();

	//Copy the config file
    outFile.write(configFile.toLatin1());

	progressDialog.setLabelText(tr("Creating SFX archive..."));
	QApplication::processEvents();

	//Copy the archive
	QFile archiveFile(archivePath);
	if(!archiveFile.open(QIODevice::ReadOnly))
	{
		outFile.close();
		QFile::remove(archivePath);
		QFile::remove(scriptPath);
		QFile::remove(fileName);
		QMessageBox::warning(this, tr("Create SFX script"), tr("Unable to open %1 for reading.").arg(archivePath));
		return;
	}
	outFile.write(archiveFile.readAll());
	archiveFile.close();

	outFile.close();
	progressDialog.close();

	QFile::remove(archivePath);
	QFile::remove(scriptPath);

	QMessageBox::information(this, tr("Create SFX script"), tr("SFX script successfully created."));
#endif
}

void MainWindow::on_actionSettings_triggered()
{
	SettingsDialog settingsDialog(mSystemTrayIcon, this);

    settingsDialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);

	if(settingsDialog.exec() == QDialog::Accepted)
	{
		QSettings settings;

		updateProxySettings();

		int recentFileCount = settings.value(QStringLiteral("gui/maxRecentFiles"), 5).toInt();
		if(recentFileCount != mMaxRecentFiles)
		{
			mMaxRecentFiles = recentFileCount;

			updateRecentFileActions();
		}
		
		QString startStopExecutionHotkey = settings.value(QStringLiteral("actions/stopExecutionHotkey"), QKeySequence(QStringLiteral("Ctrl+Alt+Q"))).toString();
		if(!startStopExecutionHotkey.isEmpty())
            mStartStopExecutionHotkey->setShortcut(QKeySequence(startStopExecutionHotkey), true);
		QString pauseExecutionHotkey = settings.value(QStringLiteral("actions/pauseExecutionHotkey"), QKeySequence(QStringLiteral("Ctrl+Alt+W"))).toString();
		if(!pauseExecutionHotkey.isEmpty())
            mPauseExecutionHotkey->setShortcut(QKeySequence(pauseExecutionHotkey), true);

        mScriptModel->setHeatmapColors
        ({
            settings.value(QStringLiteral("heatmap/minColor")).value<QColor>(),
            settings.value(QStringLiteral("heatmap/maxColor")).value<QColor>()
        });

        auto theme = static_cast<ThemeSelection::Theme>(settings.value(QStringLiteral("gui/theme"), QVariant(static_cast<int>(ThemeSelection::Theme::Default))).toInt());
        ThemeSelection::selectTheme(theme);
	}
}

void MainWindow::on_actionParameters_triggered()
{
	openParametersDialog();
}

void MainWindow::on_actionMove_up_triggered()
{
	mScriptModel->moveActions(ScriptModel::UP, selectedRows());

	scriptWasModified(true);
}

void MainWindow::on_actionMove_down_triggered()
{
	mScriptModel->moveActions(ScriptModel::DOWN, selectedRows());

	scriptWasModified(true);
}

void MainWindow::on_actionExecute_triggered()
{
	if(mScript->actionCount() == 0 || !mScript->hasEnabledActions())
		return;

	execute(false);
}

void MainWindow::on_actionExecute_selection_triggered()
{
	const QList<int> &selection = selectedRows();
	if(selection.count() == 0)
		return;

	//Unselect all
	for(int actionIndex = 0; actionIndex < mScript->actionCount(); ++actionIndex)
	{
		ActionTools::ActionInstance *actionInstance = mScript->actionAt(actionIndex);
		if(!actionInstance)
			continue;

		actionInstance->setSelected(false);
	}

	//Set the current selection
	for(int row: selection)
	{
		ActionTools::ActionInstance *actionInstance = mScript->actionAt(row);
		if(!actionInstance)
			continue;

		actionInstance->setSelected(true);
	}

	execute(true);
}

void MainWindow::on_actionCut_triggered()
{
	on_actionCopy_triggered();

	mScriptModel->removeActions(selectedRows());
}

void MainWindow::on_actionCopy_triggered()
{
	mScriptModel->copyActions(selectedRows());
}

void MainWindow::on_actionPaste_triggered()
{
	QList<int> selection = selectedRows();
    std::sort(selection.begin(), selection.end());
	int destination = mScript->actionCount();

	if(selection.count() > 0)
		destination = selection.at(0);

	mScriptModel->pasteActions(destination);
}

void MainWindow::on_actionUndo_triggered()
{
	mUndoGroup->undo();

	scriptEdited();
}

void MainWindow::on_actionRedo_triggered()
{
	mUndoGroup->redo();

	scriptEdited();
}

void MainWindow::on_actionSet_action_color_triggered()
{
	const QList<int> &selection = selectedRows();
	if(selection.count() == 0)
		return;

	ActionTools::ActionInstance *firstActionInstance = mScript->actionAt(selection.at(0));
	if(!firstActionInstance)
		return;

	QColorDialog colorDialog(firstActionInstance->color(), this);
    colorDialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
	colorDialog.setOptions(QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);
	colorDialog.setCurrentColor(firstActionInstance->color());

	if(colorDialog.exec() == QDialog::Accepted)
		mScriptModel->setActionsColor(selection, colorDialog.currentColor());
}

void MainWindow::on_actionClear_selection_color_triggered()
{
	mScriptModel->setActionsColor(selectedRows(), QColor());
}

void MainWindow::on_actionEnable_all_actions_triggered()
{
	mScriptModel->setActionsEnabled(true);
}

void MainWindow::on_actionDisable_all_actions_triggered()
{
	mScriptModel->setActionsEnabled(false);
}

void MainWindow::on_actionEnable_selection_triggered()
{
	mScriptModel->setActionsEnabled(selectedRows(), true);
}

void MainWindow::on_actionDisable_selection_triggered()
{
	mScriptModel->setActionsEnabled(selectedRows(), false);
}

void MainWindow::on_actionNew_action_triggered()
{
	if(mActionFactory->actionDefinitionCount() == 0)
		return;

    NewActionDialog dialog(mActionFactory,
                           mNewActionModel,
                           this);
    dialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
	if(dialog.exec() == QDialog::Accepted)
		wantToAddAction(dialog.selectedAction());
}

void MainWindow::on_actionEdit_action_triggered()
{
	const QList<int> &selection = selectedRows();
	if(selection.count() != 1)
		return;

	editAction(mScript->actionAt(selection.at(0)));
}

void MainWindow::on_actionJump_to_line_triggered()
{
	if(mScript->actionCount() == 0)
		return;

	QInputDialog inputDialog(this);
    inputDialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
	inputDialog.setWindowTitle(tr("Jump to line"));
	inputDialog.setLabelText(tr("Line:"));
	inputDialog.setInputMode(QInputDialog::IntInput);
	inputDialog.setIntRange(1, mScript->actionCount());

	if(inputDialog.exec() == QDialog::Accepted)
	{
		int line = inputDialog.intValue() - 1;
		if(line >= 0 && line < mScript->actionCount())
		{
			ui->scriptView->setFocus();
            //ui->scriptView->selectRow(line);
		}
	}
}

void MainWindow::on_actionCheck_for_updates_triggered()
{
#ifdef ACT_UPDATER
	checkForUpdate(false);
#endif
}

void MainWindow::on_actionCreate_shortcut_triggered()
{
	if(mCurrentFile.isEmpty())
	{
		if(!saveAs())
			return;
	}

    QString defaultDestination = QDir(QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(), QStandardPaths::LocateDirectory))
                                 .filePath(QFileInfo(mCurrentFile).fileName());
	QString filePath = QFileDialog::getSaveFileName(this, tr("Choose the shortcut destination"), defaultDestination);
	if(filePath.isEmpty())
		return;

#ifdef Q_OS_WIN
	filePath += QStringLiteral(".lnk");
#endif

	QFile file(mCurrentFile);
	if(!file.link(filePath))
		QMessageBox::warning(this, tr("Shortcut creation failed"), tr("Unable to create the shortcut."));
}

void MainWindow::on_actionImport_script_content_triggered()
{
	ScriptContentDialog scriptContentDialog(ScriptContentDialog::Write, mScript, this);
    scriptContentDialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
	if(scriptContentDialog.exec() == QDialog::Accepted)
	{
		QByteArray newContent(scriptContentDialog.text().trimmed().toUtf8());
		QBuffer buffer(&newContent);

		buffer.open(QIODevice::ReadOnly);

        checkReadResult(readScript(&buffer));
	}
}

void MainWindow::on_actionExport_script_content_triggered()
{
	QBuffer buffer;
	buffer.open(QIODevice::WriteOnly);

    writeScript(&buffer);

    ScriptContentDialog scriptContentDialog(ScriptContentDialog::Read, mScript, this);
    scriptContentDialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    scriptContentDialog.setText(QString::fromUtf8(buffer.buffer()));
    scriptContentDialog.exec();
}

void MainWindow::on_actionScriptSettings_triggered()
{
	ScriptSettingsDialog scriptSettingsDialog(this);
    scriptSettingsDialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
	scriptSettingsDialog.setPauseBefore(mScript->pauseBefore());
	scriptSettingsDialog.setPauseAfter(mScript->pauseAfter());
	if(scriptSettingsDialog.exec() == QDialog::Accepted)
	{
		if(mScript->pauseBefore() != scriptSettingsDialog.pauseBefore() ||
		   mScript->pauseAfter() != scriptSettingsDialog.pauseAfter())
		{
			mScript->setPauseBefore(scriptSettingsDialog.pauseBefore());
			mScript->setPauseAfter(scriptSettingsDialog.pauseAfter());
			scriptEdited();
		}
	}
}

void MainWindow::on_actionResources_triggered()
{
    openResourceDialog();
}

void MainWindow::on_scriptView_customContextMenuRequested(const QPoint &pos)
{
	ui->menuEdit->exec(ui->scriptView->mapToGlobal(pos));
}

void MainWindow::on_actionHelp_triggered()
{
	QDesktopServices::openUrl(QUrl(QStringLiteral("http://wiki.actiona.tools/")));
}

void MainWindow::on_actionTake_screenshot_triggered()
{
    ActionTools::ScreenshotWizard screenshotWizard(mScript, true, this);
    screenshotWizard.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    screenshotWizard.exec();
}

void MainWindow::on_actionsfilterLineEdit_textChanged(const QString &text)
{
    mNewActionProxyModel->setFilterString(text);
    ui->newActionTreeView->expandAll();
}

void MainWindow::on_scriptFilterLineEdit_textChanged(const QString &text)
{
    mScriptProxyModel->setFilterString(text);
    ui->scriptView->resizeColumnToContents(0);
    ui->scriptView->resizeColumnToContents(1);
}

void MainWindow::on_scriptFilterCriteriaFlagsComboBox_flagsChanged(unsigned int flags)
{
    mScriptProxyModel->setFilteringFlags(static_cast<ActionFilteringFlags>(flags));
    ui->scriptView->resizeColumnToContents(0);
    ui->scriptView->resizeColumnToContents(1);
}

void MainWindow::systemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	if(reason == QSystemTrayIcon::DoubleClick)
	{
		switch(windowState())
		{
			case Qt::WindowNoState:
			case Qt::WindowMaximized:
			case Qt::WindowFullScreen:
			case Qt::WindowActive:
				showMinimized();
				break;
			case Qt::WindowMinimized:
				showNormal();
				ActionTools::CrossPlatform::setForegroundWindow(this);
				break;
		}
	}
}

void MainWindow::scriptEdited()
{
#ifdef ACT_PROFILE
    Tools::HighResolutionTimer timer(QStringLiteral("scriptEdited"));
#endif
	scriptWasModified(true);
	ui->scriptView->resizeColumnToContents(0);
	ui->scriptView->resizeColumnToContents(1);
	actionCountChanged();
	actionEnabled();
	actionSelectionChanged();

	updateUndoRedoStatus();
}

void MainWindow::actionSelectionChanged()
{
	int columnCount = mScriptModel->columnCount();
	if(columnCount == 0 )
		actionSelectionChanged(0);
    else
		actionSelectionChanged(ui->scriptView->selectionModel()->selectedIndexes().count() / columnCount);
}

void MainWindow::scriptWasModified(bool modified)
{
	mScriptModified = modified;
	setWindowModified(modified);
}

void MainWindow::readSettings()
{
	QSettings settings;

	restoreGeometry(settings.value(QStringLiteral("geometry")).toByteArray());
	restoreState(settings.value(QStringLiteral("windowState")).toByteArray());
	mMaxRecentFiles = settings.value(QStringLiteral("gui/maxRecentFiles"), 5).toInt();
	if(mSystemTrayIcon)
		mSystemTrayIcon->setVisible(settings.value(QStringLiteral("gui/showTaskbarIcon"), true).toBool());

	QList<QVariant> customColors = settings.value(QStringLiteral("customColors")).toList();
	for(int colorIndex = 0; colorIndex < customColors.count(); ++colorIndex)
		QColorDialog::setCustomColor(colorIndex, customColors.at(colorIndex).value<QRgb>());

	ui->clearBeforeExecutionCheckBox->setChecked(settings.value(QStringLiteral("gui/clearConsoleBeforeExecution"), true).toBool());

    mScriptModel->setHeatmapColors
    ({
        settings.value(QStringLiteral("heatmap/minColor"), QColor{Qt::yellow}).value<QColor>(),
        settings.value(QStringLiteral("heatmap/maxColor"), QColor{Qt::red}).value<QColor>()
    });
}

void MainWindow::writeSettings()
{
	QSettings settings;

	settings.setValue(QStringLiteral("geometry"), saveGeometry());
	settings.setValue(QStringLiteral("windowState"), saveState());
	settings.setValue(QStringLiteral("gui/maxRecentFiles"), mMaxRecentFiles);
	if(mSystemTrayIcon)
		settings.setValue(QStringLiteral("gui/showTaskbarIcon"), mSystemTrayIcon->isVisible());

	QList<QVariant> customColors;
	for(int colorIndex = 0; colorIndex < QColorDialog::customCount(); ++colorIndex)
		customColors << QColorDialog::customColor(colorIndex);
	settings.setValue(QStringLiteral("customColors"), customColors);

	settings.setValue(QStringLiteral("gui/clearConsoleBeforeExecution"), ui->clearBeforeExecutionCheckBox->isChecked());

    settings.setValue(QStringLiteral("heatmap/minColor"), mScriptModel->heatmapColors().first);
    settings.setValue(QStringLiteral("heatmap/maxColor"), mScriptModel->heatmapColors().second);
}

void MainWindow::updateRecentFileActions()
{
	QSettings settings;
	QStringList files = settings.value(QStringLiteral("recentFileList")).toStringList();
	files.removeAll(QString());

	int numRecentFiles = qMin(files.size(), mMaxRecentFiles);

	for(int i=0; i<numRecentFiles; ++i)
	{
		QString text = tr("&%1 %2").arg(i + 1).arg(QFileInfo(files.at(i)).fileName());
		mRecentFileActs.at(i)->setText(text);
		mRecentFileActs.at(i)->setData(files.at(i));
		mRecentFileActs.at(i)->setVisible(true);
	}

    for(int j = numRecentFiles; j < mMaxRecentFiles && j < mRecentFileActs.size(); ++j)
		mRecentFileActs.at(j)->setVisible(false);

	ui->menuRecent_scripts->setEnabled(numRecentFiles > 0);
	ui->actionClear->setEnabled(numRecentFiles > 0);
}

void MainWindow::updateProxySettings()
{
	QSettings settings;
	QNetworkProxy proxy;

	int proxyMode = settings.value(QStringLiteral("network/proxyMode"), ActionTools::Settings::PROXY_SYSTEM).toInt();
	switch(proxyMode)
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
		{
			int type = settings.value(QStringLiteral("network/proxyType"), ActionTools::Settings::PROXY_TYPE_HTTP).toInt();
			QNetworkProxy proxy;

			if(type == ActionTools::Settings::PROXY_TYPE_HTTP)
				proxy.setType(QNetworkProxy::HttpProxy);
			else
				proxy.setType(QNetworkProxy::Socks5Proxy);

			proxy.setHostName(settings.value(QStringLiteral("network/proxyHost"), QStringLiteral("0.0.0.0")).toString());
			proxy.setPort(settings.value(QStringLiteral("network/proxyPort"), 0).value<quint16>());
			proxy.setUser(settings.value(QStringLiteral("network/proxyUser"), QString()).toString());
			proxy.setPassword(settings.value(QStringLiteral("network/proxyPassword"), QString()).toString());
		}
		break;
	}

	QNetworkProxy::setApplicationProxy(proxy);
}

bool MainWindow::checkReadResult(ActionTools::Script::ReadResult result)
{
	switch(result)
	{
	case ActionTools::Script::ReadSuccess:
		{
			if(mScript->scriptVersion() < Global::SCRIPT_VERSION)
			{
                QMessageBox::warning(this, tr("Load script"), tr("This script was created with an older version of Actiona.\nIt will be updated when you save it.\nYour version: %1\nScript version: %2")
									 .arg(Global::SCRIPT_VERSION.toString()).arg(mScript->scriptVersion().toString()));
			}

			if(mScript->missingActions().count() > 0)
			{
				QString missingActions(tr("Script loaded, some actions are missing:<ul>"));

				for(const QString &missingAction: mScript->missingActions())
					missingActions += QStringLiteral("<li>") + missingAction + QStringLiteral("</li>");

				missingActions += QStringLiteral("</ul>");

				QMessageBox::warning(this, tr("Load script"), missingActions);
			}
		}
		return true;
	case ActionTools::Script::ReadInternal:
		QMessageBox::warning(this, tr("Load script"), tr("Unable to load the script due to an internal error."));
		return false;
	case ActionTools::Script::ReadInvalidScriptVersion:
        QMessageBox::warning(this, tr("Load script"), tr("Unable to load the script because it was created with a more recent version of Actiona.\nPlease update your version of Actiona to load this script.\nYour version: %1\nScript version: %2")
							 .arg(Global::SCRIPT_VERSION.toString()).arg(mScript->scriptVersion().toString()));
		return false;
	default:
		return false;
	}
}

ActionTools::Script::ReadResult MainWindow::readScript(QIODevice *device)
{
    auto progressDialog = createStandardProgressDialog();
    progressDialog->setLabelText(tr("Loading script..."));
    progressDialog->setWindowTitle(tr("Loading script"));
    progressDialog->open();

    QApplication::processEvents();

    std::function<void(int, int, QString)> progressCallback = [&progressDialog](int progress, int total, const QString &description)
    {
        progressDialog->setLabelText(description);
        progressDialog->setRange(0, total);
        progressDialog->setValue(progress);
    };
    std::function<void()> resetCallback = [this]()
	{
		mScriptModel->reset();
	};
    std::function<void(QList<ActionTools::ActionInstance *>)> addActionCallback = [this](QList<ActionTools::ActionInstance *> instances)
	{
		mScriptModel->appendActions(instances);
	};

    ActionTools::Script::ReadResult result = mScript->read(device, Global::SCRIPT_VERSION, &progressCallback, &resetCallback, &addActionCallback);
    if(result == ActionTools::Script::ReadSuccess)
        scriptEdited();

    progressDialog->close();

    return result;
}

bool MainWindow::writeScript(QIODevice *device)
{
    auto progressDialog = createStandardProgressDialog();
    progressDialog->setLabelText(tr("Saving script..."));
    progressDialog->setWindowTitle(tr("Saving script"));
    progressDialog->open();

    std::function<void(int, int, QString)> progressCallback = [&progressDialog](int progress, int total, const QString &description)
    {
        progressDialog->setLabelText(description);
        progressDialog->setRange(0, total);
        progressDialog->setValue(progress);
    };

    bool result = mScript->write(device, Global::ACTIONA_VERSION, Global::SCRIPT_VERSION, &progressCallback);

    progressDialog->close();

    return result;
}

std::unique_ptr<QProgressDialog> MainWindow::createStandardProgressDialog()
{
    auto result = std::unique_ptr<QProgressDialog>(new QProgressDialog(this));

    result->setWindowModality(Qt::ApplicationModal);
    result->setCancelButton(nullptr);
    result->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    result->setAutoClose(false);
    result->setMinimumDuration(0);

    return result;
}

ActionDialog *MainWindow::getOrCreateActionDialog(const ActionTools::ActionDefinition *actionDefinition)
{
    int actionIndex = actionDefinition->index();

    if(mActionDialogs.contains(actionIndex))
        return mActionDialogs.value(actionIndex);
    else
    {
        auto newActionDialog = new ActionDialog(mCompletionModel, mScript, actionDefinition, mUsedLocale, this);

        newActionDialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);

        mActionDialogs.insert(actionIndex, newActionDialog);

        return newActionDialog;
    }
}

#ifdef ACT_UPDATER
void MainWindow::checkForUpdate(bool silent)
{
	mUpdaterProgressDialog->setRange(0, 0);
	mUpdaterProgressDialog->setLabelText(tr("Checking if an update is available..."));
	mUpdaterProgressDialog->setMinimumDuration(0);
	mUpdaterProgressDialog->open(this, SLOT(updateCanceled()));
	mUpdaterProgressDialog->setWindowTitle(tr("Checking for updates"));
	mSilentUpdate = silent;
    QString localeName = QLocale::system().name();
	QStringList localeParts = localeName.split(QLatin1Char('_'));
    QString languageName = localeName;

    if(localeParts.size() >= 2)
        languageName = localeParts[0];

    mUpdater->checkForUpdates(QStringLiteral("actiona3"),
                              Global::ACTIONA_VERSION,
                              Global::applicationBits(),
                              Tools::Updater::Binary,
                              Tools::Updater::Installer,
                              Global::currentOSType(),
                              Global::currentOSBits(),
                              languageName);
}
#endif

void MainWindow::logItemClicked(int itemRow, bool doubleClick)
{
	auto model = qobject_cast<QStandardItemModel *>(ui->consoleWidget->model());
	if(!model)
		return;

	QStandardItem *item = model->item(itemRow, 0);
	if(!item)
		return;

	switch(item->data(ActionTools::ConsoleWidget::SourceRole).value<ActionTools::ConsoleWidget::Source>())
	{
	case ActionTools::ConsoleWidget::Parameters:
		{
			if(doubleClick)
			{
				int parameter = item->data(ActionTools::ConsoleWidget::ParameterRole).toInt();
				int line = item->data(ActionTools::ConsoleWidget::LineRole).toInt();
				int column = item->data(ActionTools::ConsoleWidget::ColumnRole).toInt();
				openParametersDialog(parameter, line, column);
			}
		}
		break;
    case ActionTools::ConsoleWidget::Resources:
        {
            if(doubleClick)
            {
                const QString &resource = item->data(ActionTools::ConsoleWidget::ResourceRole).toString();
                openResourceDialog(resource);
            }
        }
        break;
	case ActionTools::ConsoleWidget::Action:
	case ActionTools::ConsoleWidget::User:
		{
			qint64 actionRuntimeId = item->data(ActionTools::ConsoleWidget::ActionRole).toLongLong();
			int action = mScript->actionIndexFromRuntimeId(actionRuntimeId);
			if(action == -1)
				break;

			if(doubleClick)
			{
				QString field = item->data(ActionTools::ConsoleWidget::FieldRole).toString();
				QString subField = item->data(ActionTools::ConsoleWidget::SubFieldRole).toString();
				int line = item->data(ActionTools::ConsoleWidget::LineRole).toInt();
				int column = item->data(ActionTools::ConsoleWidget::ColumnRole).toInt();
				editAction(mScript->actionAt(action), field, subField, line, column);
			}
			else
			{
                ui->scriptView->setCurrentIndex(mScriptProxyModel->mapFromSource(mScriptModel->index(action, 0)));
				ui->scriptView->setFocus();
			}
		}
		break;
	case ActionTools::ConsoleWidget::Exception:
		{
			qint64 actionRuntimeId = item->data(ActionTools::ConsoleWidget::ActionRole).toLongLong();
			int action = mScript->actionIndexFromRuntimeId(actionRuntimeId);
			if(action == -1)
				break;

			if(doubleClick)
			{
				int exception = item->data(ActionTools::ConsoleWidget::ExceptionRole).toInt();
				editAction(mScript->actionAt(action), exception);
			}
			else
			{
                ui->scriptView->setCurrentIndex(mScriptProxyModel->mapFromSource(mScriptModel->index(action, 0)));
				ui->scriptView->setFocus();
			}
		}
		break;
	default:
		break;
	}
}

void MainWindow::updateUndoRedoStatus()
{
	ui->actionUndo->setEnabled(mUndoGroup->canUndo());
	ui->actionRedo->setEnabled(mUndoGroup->canRedo());
}

void MainWindow::execute(bool onlySelection)
{
	if(ui->clearBeforeExecutionCheckBox->isChecked())
		ui->consoleWidget->clear();

	QSettings settings;

    bool addStartEndSeparators = settings.value(QStringLiteral("gui/addConsoleStartEndSeparators"), QVariant(true)).toBool();
    if(addStartEndSeparators)
		ui->consoleWidget->addStartSeparator();

    int maxEntries = settings.value(QStringLiteral("gui/consoleMaxEntries"), QVariant(0)).toInt();
    ui->consoleWidget->setMaxEntries(maxEntries);

	bool showExecutionWindow = settings.value(QStringLiteral("actions/showExecutionWindow"), QVariant(true)).toBool();
	int executionWindowPosition = settings.value(QStringLiteral("actions/executionWindowPosition"), QVariant(0)).toInt();
	int executionWindowScreen = settings.value(QStringLiteral("actions/executionWindowScreen"), QVariant(0)).toInt();
	bool showConsoleWindow = settings.value(QStringLiteral("actions/showConsoleWindow"), QVariant(true)).toBool();
	int consoleWindowPosition = settings.value(QStringLiteral("actions/consoleWindowPosition"), QVariant(1)).toInt();
	int consoleWindowScreen = settings.value(QStringLiteral("actions/consoleWindowScreen"), QVariant(0)).toInt();

	if(mCommandLineParser.isSet(QStringLiteral("noexecutionwindow")))
		showExecutionWindow = false;
	if(mCommandLineParser.isSet(QStringLiteral("noconsolewindow")))
		showConsoleWindow = false;

	{
#ifdef ACT_PROFILE
        Tools::HighResolutionTimer timer(QStringLiteral("Executer setup"));
#endif
		mExecuter.setup(mScript,
						 mActionFactory,
						 showExecutionWindow,
						 executionWindowPosition,
						 executionWindowScreen,
						 showConsoleWindow,
						 consoleWindowPosition,
                         consoleWindowScreen,
                         Global::ACTIONA_VERSION,
						 Global::SCRIPT_VERSION,
						 false,
                         ui->consoleWidget->model(),
                         maxEntries);
	}

    mWasNewActionDockShown = !ui->actionsDockWidget->isHidden();
    mWasConsoleDockShown = !ui->consoleDockWidget->isHidden();

    mScriptStopped = false;
    if(mExecuter.startExecution(onlySelection, mCurrentFile))
    {
        // mScriptStopped is set to true if a code exception has happened.
        // In that case we don't want to hide everything.
        if(!mScriptStopped)
        {
            mPreviousWindowPosition = pos();
            hide();
            ui->actionsDockWidget->hide();
            ui->consoleDockWidget->hide();

            if(mSystemTrayIcon)
                mSystemTrayIcon->setToolTip(tr("Actiona - executing"));

            ui->actionExecute->setEnabled(false);
            ui->actionExecute_selection->setEnabled(false);
            mStopExecutionAction->setEnabled(true);
        }
	}
	else
	{
		ui->consoleWidget->updateClearButton();

		if(settings.value(QStringLiteral("gui/addConsoleStartEndSeparators"), QVariant(true)).toBool())
			ui->consoleWidget->addEndSeparator();
	}
}

void MainWindow::fillNewActionModel()
{
    mNewActionModel->clear();

    // Add categories
    {
        QFont boldFont;
        boldFont.setWeight(QFont::Bold);

        for(int categoryIndex = 0; categoryIndex < ActionTools::CategoryCount; ++categoryIndex)
        {
			QString categoryName = QApplication::translate("ActionDefinition::CategoryName", ActionTools::ActionDefinition::CategoryName[categoryIndex].toLatin1().constData());
            auto categoryItem = new QStandardItem(categoryName);

            categoryItem->setFont(boldFont);

            if(mActionFactory->actionDefinitionCount(static_cast<ActionTools::ActionCategory>(categoryIndex)) == 0)
                categoryItem->setFlags(Qt::NoItemFlags);
            else
                categoryItem->setFlags(Qt::ItemIsEnabled);

            mNewActionModel->appendRow(categoryItem);
        }
    }

    // Add actions
    {
        QFont italicFont;
        italicFont.setItalic(true);

        for(int i = 0; i < mActionFactory->actionDefinitionCount(); ++i)
        {
            ActionTools::ActionDefinition *actionDefinition = mActionFactory->actionDefinition(i);
            QStandardItem *categoryItem = mNewActionModel->item(actionDefinition->category(), 0);
            QString tooltip = actionDefinition->description();
            QStandardItem *actionItem = new QStandardItem(actionDefinition->cachedIcon(), actionDefinition->name());

            if(!actionDefinition->worksUnderThisOS())
            {
                actionItem->setFont(italicFont);

				tooltip.append(QStringLiteral("\n"));
                tooltip.append(tr("Note: does not work under this operating system"));
            }

            actionItem->setToolTip(tooltip);
            actionItem->setData(actionDefinition->id(), NewActionModel::ActionIdRole);

            categoryItem->appendRow(actionItem);
        }
    }

    ui->newActionTreeView->expandAll();
}

void MainWindow::editAction(const QModelIndex &index)
{
	if(index.column() != ScriptModel::ColumnActionName)
		return;

	editAction(mScript->actionAt(index.row()));
}

void MainWindow::wantToAddAction(const QString &actionId)
{
	wantToAddAction(mScript->actionCount(), actionId);
}

void MainWindow::wantToAddAction(int row, const QString &actionId)
{
	QTimer::singleShot(0, this, SLOT(addAction()));

	mAddActionRow = row;
	mAddAction = actionId;
}

void MainWindow::scriptFileDropped(const QString &scriptFilename)
{
	QFileInfo fileInfo(scriptFilename);

	if(fileInfo.isFile() && fileInfo.isReadable() && maybeSave())
		loadFile(scriptFilename);
}

void MainWindow::scriptContentDropped(const QString &scriptContent)
{
	if(maybeSave())
	{
		QByteArray newContent(scriptContent.toUtf8());
		QBuffer buffer(&newContent);

		buffer.open(QIODevice::ReadOnly);

        checkReadResult(readScript(&buffer));
	}
}

void MainWindow::addAction()
{
	ActionTools::ActionDefinition *definition = mActionFactory->actionDefinition(mAddAction);
	if(!definition)
		return;

	ActionTools::ActionInstance *actionInstance = definition->newActionInstance();
	if(!actionInstance)
		return;

	if(editAction(actionInstance))
	{
		if(mAddActionRow == mScript->actionCount())
			ui->scriptView->scrollToBottom();

		mScriptModel->insertAction(mAddActionRow, ActionTools::ActionInstanceBuffer(mAddAction, *actionInstance));
	}

	delete actionInstance;//Always delete it, since the model creates a copy of it...
}

void MainWindow::openRecentFile()
{
	auto action = qobject_cast<QAction *>(sender());

	if(action && maybeSave())
	{
		if(!loadFile(action->data().toString()))
		{
			QSettings settings;
			QStringList files = settings.value(QStringLiteral("recentFileList")).toStringList();

			files.removeAll(action->data().toString());
			settings.setValue(QStringLiteral("recentFileList"), files);

			updateRecentFileActions();
		}
	}
}

void MainWindow::newActionDoubleClicked(const QModelIndex &index)
{
    auto modelIndex = mNewActionProxyModel->mapToSource(index);

    QString actionId = modelIndex.data(NewActionModel::ActionIdRole).toString();

    if(!actionId.isEmpty())
        wantToAddAction(actionId);
}

void MainWindow::actionEnabled()
{
	enabledActionsCountChanged(mScript->hasEnabledActions());
}

void MainWindow::packLoadError(const QString &error)
{
	mPackLoadErrors << error;
}

void MainWindow::stopExecution()
{
	mExecuter.stopExecution();
}

void MainWindow::startOrStopExecution()
{
	if(!ui->actionExecute->isEnabled()) // Executing
		stopExecution();
	else if(ui->actionExecute->isEnabled())
		execute(false);
}

void MainWindow::pauseOrResumeExecution()
{
	mExecuter.pauseExecution();
}

void MainWindow::scriptExecutionStopped()
{
    mScriptStopped = true;
    ui->heatmapModeComboBox->setEnabled(true);

	QSettings settings;

	if(settings.value(QStringLiteral("gui/addConsoleStartEndSeparators"), QVariant(true)).toBool())
		ui->consoleWidget->addEndSeparator();

	ui->consoleWidget->updateClearButton();

	if(mCommandLineParser.isSet(QStringLiteral("exitatend")))
		QApplication::quit();
	else
	{
		show();

		ui->actionsDockWidget->setVisible(mWasNewActionDockShown);
		ui->consoleDockWidget->setVisible(mWasConsoleDockShown);

		QTimer::singleShot(50, this, SLOT(postExecution()));

		if(mSystemTrayIcon)
            mSystemTrayIcon->setToolTip(tr("Actiona - ready"));

		ui->actionExecute->setEnabled(true);
		mStopExecutionAction->setEnabled(false);

		actionSelectionChanged();
	}
}

void MainWindow::postExecution()
{
    if(mScriptStopped)
        return;

    move(mPreviousWindowPosition);
}

void MainWindow::logItemDoubleClicked(int itemRow)
{
	logItemClicked(itemRow, true);
}

void MainWindow::logItemClicked(int itemRow)
{
	logItemClicked(itemRow, false);
}

void MainWindow::otherInstanceMessage(const QString &message)
{
	if(!mCurrentFile.isEmpty() && !ui->actionExecute->isEnabled()) // Executing
		return;

	if(!message.isEmpty())
	{
		QFileInfo fileInfo(message);

		if(fileInfo.isFile() && fileInfo.isReadable())
		{
			if(maybeSave())
				loadFile(message);

			emit needToShow();
		}
    }
}

#ifdef ACT_UPDATER
void MainWindow::updateError(const QString &message)
{
	mUpdaterProgressDialog->hide();

	if(mSilentUpdate)
		return;

	QMessageBox::warning(this, tr("Update"), tr("An error occured while checking for a new version:\n%1").arg(message));
}

void MainWindow::updateNoResult()
{
	mUpdaterProgressDialog->hide();

	if(mSilentUpdate)
		return;

	QMessageBox::information(this, tr("Update"), tr("No new version is available."));
}

void MainWindow::updateSuccess(const QVersionNumber &version,
			 const QDate &releaseDate,
			 const QString &type,
			 const QString &changelog,
			 const QString &filename,
			 int size,
			 const QString &hash)
{
	mUpdaterProgressDialog->hide();

    if(version <= Global::ACTIONA_VERSION)
	{
		if(!mSilentUpdate)
			QMessageBox::information(this, tr("Update"), tr("No new version is available."));
		return;
	}

	ChangelogDialog changelogDialog(this);
    changelogDialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
	changelogDialog.setVersion(version);
	changelogDialog.setReleaseDate(releaseDate);

	if(type == QLatin1String("major"))
		changelogDialog.setType(tr("Major"));
	else if(type == QLatin1String("release"))
		changelogDialog.setType(tr("Release"));
	else
		changelogDialog.setType(tr("Bugfix"));

	changelogDialog.setChangelog(changelog);
	changelogDialog.exec();

    if(changelogDialog.changelogAction() == ChangelogDialog::NoAction)
		return;

    QString updateFilename;
    if(changelogDialog.changelogAction() == ChangelogDialog::DownloadOnly)
	{
        updateFilename = QFileDialog::getSaveFileName(	this,
                                    tr("Select where to save the installation file"),
                                    QDir(QStandardPaths::locate(QStandardPaths::DownloadLocation, QString(), QStandardPaths::LocateDirectory)).filePath(QFileInfo(filename).fileName()));

		mInstallAfterUpdateDownload = false;
	}
	else if(changelogDialog.changelogAction() == ChangelogDialog::DownloadAndInstall)
	{
		updateFilename = QDir::temp().filePath(QFileInfo(filename).fileName());
		mInstallAfterUpdateDownload = true;
	}

	if(updateFilename.isEmpty())
		return;

	mUpdateFile.setFileName(updateFilename);
	if(!mUpdateFile.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, tr("Download update"), tr("Unable to save the update file."));

		return;
	}

	mUpdateFileSize = size;
	mUpdateFileHash = hash;
	mHashCalculator.reset();

	mUpdateDownloadNetworkReply = mNetworkAccessManager->get(QNetworkRequest(filename));

    connect(mUpdateDownloadNetworkReply, &QNetworkReply::downloadProgress, this, &MainWindow::updateDownloadProgress);
    connect(mUpdateDownloadNetworkReply, &QNetworkReply::finished, this, &MainWindow::updateDownloadFinished);
    connect(mUpdateDownloadNetworkReply, &QNetworkReply::readyRead, this, &MainWindow::updateDownloadDataAvailable);

	mUpdaterProgressDialog->setLabelText(tr("Downloading file..."));
	mUpdaterProgressDialog->setWindowTitle(tr("Update download"));
	mUpdaterProgressDialog->open(this, SLOT(updateDownloadCanceled()));
}

void MainWindow::updateCanceled()
{
	mUpdater->cancel();
}

void MainWindow::updateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	mUpdaterProgressDialog->setValue(static_cast<int>(bytesReceived));
	mUpdaterProgressDialog->setMaximum(static_cast<int>(bytesTotal));
}

void MainWindow::updateDownloadFinished()
{
	mUpdaterProgressDialog->hide();
	mUpdateFile.close();

	if(mUpdateDownloadNetworkReply->error() != QNetworkReply::NoError)
	{
		QString errorMessage;
		switch(mUpdateDownloadNetworkReply->error())
		{
		case QNetworkReply::ConnectionRefusedError:
			errorMessage = tr("Connection to the server was refused.");
			break;
		case QNetworkReply::HostNotFoundError:
			errorMessage = tr("Unable to establish a connection to the server.");
			break;
		case QNetworkReply::TimeoutError:
			errorMessage = tr("Connection timeout.");
			break;
		case QNetworkReply::OperationCanceledError:
			break;
		case QNetworkReply::ContentNotFoundError:
			errorMessage = tr("Serverside error.");
			break;
		default:
			errorMessage = tr("Connection error.");
			break;
		}

		mUpdateDownloadNetworkReply->deleteLater();
		mUpdateDownloadNetworkReply = nullptr;

		if(!errorMessage.isEmpty())
			QMessageBox::warning(this, tr("Update download"), tr("An error occured while downloading the file.\nError message: %1").arg(errorMessage));
		return;
	}

	if(QFileInfo(mUpdateFile).size() != mUpdateFileSize || QLatin1String(mHashCalculator.result().toHex()) != mUpdateFileHash)
		QMessageBox::warning(this, tr("Update download"), tr("The downloaded file is corrupted. Try again later."));
	else
		QTimer::singleShot(1, this, SLOT(postDownloadOperation()));

	mUpdateDownloadNetworkReply->deleteLater();
	mUpdateDownloadNetworkReply = nullptr;
}

void MainWindow::updateDownloadDataAvailable()
{
	const QByteArray &data = mUpdateDownloadNetworkReply->readAll();
	mUpdateFile.write(data);
	mHashCalculator.addData(data);
}

void MainWindow::updateDownloadCanceled()
{
	if(!mUpdateDownloadNetworkReply)
		return;

	mUpdateDownloadNetworkReply->disconnect();
	mUpdateDownloadNetworkReply->abort();
	mUpdateDownloadNetworkReply->deleteLater();
	mUpdateDownloadNetworkReply = nullptr;

	mUpdateFile.close();
	mUpdateFile.remove();
}

void MainWindow::postDownloadOperation()
{
	if(mInstallAfterUpdateDownload)
	{
		mUpdateFile.setPermissions(mUpdateFile.permissions() | QFile::ExeOwner);
        if(QProcess::startDetached(mUpdateFile.fileName(), {}))
			QApplication::quit();
		else
			QMessageBox::warning(this, tr("Update"), tr("Unable to execute the downloaded file."));
	}
	else
        QDesktopServices::openUrl(QUrl(QStringLiteral("file:///") + QFileInfo(mUpdateFile.fileName()).dir().path(), QUrl::TolerantMode));
}
#endif

ActionDialog *MainWindow::actionDialog(ActionTools::ActionInstance *actionInstance)
{
    ActionDialog *actionDialogPointer = getOrCreateActionDialog(actionInstance->definition());
    QSize sizeDifference = size() / 2 - actionDialogPointer->size() / 2;
    actionDialogPointer->move(pos().x() + sizeDifference.width(), pos().y() + sizeDifference.height());

    return actionDialogPointer;
}


bool MainWindow::editAction(ActionTools::ActionInstance *actionInstance, const QString &field, const QString &subField, int line, int column)
{
	if(!actionInstance)
		return false;

	ActionTools::ActionInstance oldAction = *actionInstance;
    ActionDialog *dialog = actionDialog(actionInstance);
	if(!dialog)
	{
		qWarning("Unable to create an ActionDialog");
		return false;
	}

	int result = dialog->exec(actionInstance, field, subField, line, column);
	if(result == QDialog::Accepted)
	{
		if(oldAction != *actionInstance)
			scriptEdited();
	}

	return (result == QDialog::Accepted);
}

bool MainWindow::editAction(ActionTools::ActionInstance *actionInstance, int exception)
{
	if(!actionInstance)
		return false;

	ActionTools::ActionInstance oldAction = *actionInstance;
    ActionDialog *dialog = actionDialog(actionInstance);
	if(!dialog)
	{
		qWarning("Unable to create an ActionDialog");
		return false;
	}

	int result = dialog->exec(actionInstance, exception);
	if(result == QDialog::Accepted)
	{
		if(oldAction != *actionInstance)
			scriptEdited();
	}

	return (result == QDialog::Accepted);
}

void MainWindow::openParametersDialog(int parameter, int line, int column)
{
    ScriptParametersDialog scriptParametersDialog(mScript, this);
    scriptParametersDialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
	QList<ActionTools::ScriptParameter> parameters = mScript->parameters();
	scriptParametersDialog.setCurrentParameter(parameter);
	scriptParametersDialog.setCurrentLine(line);
	scriptParametersDialog.setCurrentColumn(column);
	if(scriptParametersDialog.exec() == QDialog::Accepted && mScript->parameters() != parameters)
        scriptWasModified(true);
}

void MainWindow::openResourceDialog(const QString &resource)
{
    ResourceDialog resourceDialog(mScript, this);
    resourceDialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    resourceDialog.setCurrentResource(resource);
	QMap<QString, ActionTools::Resource> resources = mScript->resources();
    if(resourceDialog.exec() == QDialog::Accepted)
    {
        if(resources != mScript->resources())
            scriptEdited();
    }
}

QList<int> MainWindow::selectedRows() const
{
	QModelIndexList selectedIndexes = ui->scriptView->selectionModel()->selectedIndexes();

    for(QModelIndex &modelIndex: selectedIndexes)
        modelIndex = mScriptProxyModel->mapToSource(modelIndex);

	if(selectedIndexes.count() == 0)
		return QList<int>();

	QList<int> selectedRows;

	for(const QModelIndex &index: qAsConst(selectedIndexes))
	{
		if(index.column() == ScriptModel::ColumnLabel)
			selectedRows << index.row();
	}

	return selectedRows;
}

bool MainWindow::loadFile(const QString &fileName, bool verbose)
{
#ifdef ACT_PROFILE
    Tools::HighResolutionTimer timer(QStringLiteral("load file %1").arg(fileName));
#endif
	QFile loadFile(fileName);
	QFileInfo loadFileInfo(loadFile);
	if(!loadFileInfo.isReadable() || !loadFile.open(QIODevice::ReadOnly))
	{
		if(verbose)
			QMessageBox::warning(this, tr("Load script"), tr("Unable to load the script because the file is not readable or you don't have enough rights."));
		return false;
	}

	if(fileName.endsWith(QStringLiteral(".acod")))
	{
		if(verbose)
            QMessageBox::warning(this, tr("Load script"), tr("Actiona currently has no builtin editor for Code files (.acod). Please use an external code editor."));
		return false;
	}

    ActionTools::Script::ReadResult result = readScript(&loadFile);
	if(result == ActionTools::Script::ReadSuccess)
	{
		QSettings settings;

		settings.setValue(QStringLiteral("gui/lastScript"), fileName);

		statusBar()->showMessage(tr("File loaded"), 2000);

		setCurrentFile(fileName);
	}

	return checkReadResult(result);
}

bool MainWindow::saveFile(const QString &fileName, bool copy)
{
#ifdef ACT_PROFILE
    Tools::HighResolutionTimer timer(QStringLiteral("save file %1").arg(fileName));
#endif
	QFile saveFile(fileName);
	if(!saveFile.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, tr("Save script"), tr("Unable to save the script because the file is not writable or you don't have enough rights."));
		return false;
	}

    bool result = writeScript(&saveFile);
	if(result)
	{
		if(!copy)
			setCurrentFile(fileName);
		statusBar()->showMessage(tr("File saved"), 2000);

		QSettings settings;

		settings.setValue(QStringLiteral("gui/lastScript"), fileName);
	}

	return result;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
	mCurrentFile = fileName;
	setWindowTitle(QString());
	scriptWasModified(false);

	QString shownName = mCurrentFile;
	if(shownName.isEmpty())
		shownName = tr("untitled.ascr");
	setWindowFilePath(shownName);

	QSettings settings;
	QStringList files = settings.value(QStringLiteral("recentFileList")).toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);
	while(files.size() > mMaxRecentFiles)
		files.removeLast();

	settings.setValue(QStringLiteral("recentFileList"), files);

	updateRecentFileActions();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(maybeSave())
	{
		writeSettings();

        QApplication::quit();
	}
	else
		event->ignore();
}

bool MainWindow::maybeSave()
{
	if(mScriptModified)
	{
		QMessageBox::StandardButton result = QMessageBox::warning(this, tr("Save changes"), tr("The script has been modified.\nDo you want to save your changes ?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
		if(result == QMessageBox::Save)
			return save();
		else if(result == QMessageBox::Cancel)
			return false;
	}

	return true;
}

bool MainWindow::save()
{
	if(mCurrentFile.isEmpty())
		return saveAs();
    else if(mScriptModified)
		return saveFile(mCurrentFile);
    else
        return false;
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save script"), QString(), tr("Actiona script (*.ascr)"));
	if(fileName.isEmpty())
		return false;

	QFileInfo fileInfo(fileName);
	if(fileInfo.suffix().isEmpty())
		fileName += QStringLiteral(".ascr");

	return saveFile(fileName);
}

void MainWindow::deleteSelection()
{
	mScriptModel->removeActions(selectedRows());

	scriptWasModified(true);
}

void MainWindow::actionSelectionChanged(int selectionCount)
{
	bool hasSelection = (selectionCount > 0);

	bool hasSelectionEnabledActions = false;
    const auto rows = selectedRows();
    for(int row: rows)
	{
		ActionTools::ActionInstance *actionInstance = mScript->actionAt(row);
		if(!actionInstance)
			continue;

		if(actionInstance->isEnabled() && actionInstance->definition()->worksUnderThisOS())
		{
			hasSelectionEnabledActions = true;
			break;
		}
	}

	ui->actionDelete_action->setEnabled(hasSelection);
	ui->actionCut->setEnabled(hasSelection);
	ui->actionCopy->setEnabled(hasSelection);
	ui->actionEdit_action->setEnabled(selectionCount == 1);
	ui->actionExecute_selection->setEnabled(hasSelection && hasSelectionEnabledActions);
	ui->actionMove_up->setEnabled(hasSelection);
	ui->actionMove_down->setEnabled(hasSelection);
	ui->actionSet_action_color->setEnabled(hasSelection);
	ui->actionClear_selection_color->setEnabled(hasSelection);
	ui->deleteDropTarget->setEnabled(hasSelection);
	ui->actionEnable_selection->setEnabled(hasSelection);
	ui->actionDisable_selection->setEnabled(hasSelection);
}

void MainWindow::actionCountChanged()
{
	bool hasActions = (mScript->actionCount() > 0);

	ui->actionDelete_all_actions->setEnabled(hasActions);
	ui->actionSelect_all_actions->setEnabled(hasActions);
	ui->actionExecute->setEnabled(hasActions);
	ui->actionSelect_all_actions->setEnabled(hasActions);
	ui->actionSelect_none->setEnabled(hasActions);
	ui->actionEnable_all_actions->setEnabled(hasActions);
	ui->actionDisable_all_actions->setEnabled(hasActions);
	ui->actionInverse_selection->setEnabled(hasActions);
	ui->actionJump_to_line->setEnabled(hasActions);
    ui->heatmapModeComboBox->setEnabled(hasActions && mScript->hasBeenExecuted());
}

void MainWindow::enabledActionsCountChanged(bool hasEnabledActions)
{
	ui->actionExecute->setEnabled(hasEnabledActions);
	ui->actionExecute_selection->setEnabled(hasEnabledActions);
}
