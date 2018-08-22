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

#include "script.h"
#include "executer.h"
#include "heatmapmode.h"

#include <QMainWindow>
#ifndef ACT_NO_UPDATER
#include <QCryptographicHash>
#include <QFile>
#endif

#include <memory>

namespace Ui
{
	class MainWindow;
}

namespace ActionTools
{
	class ActionFactory;
	class ActionInstance;
    class ActionDefinition;
}

namespace Tools
{
	class Updater;
}

class QToolBox;
class QSystemTrayIcon;
class ScriptModel;
class ProgressSplashScreen;
class QTreeWidget;
class QTreeWidgetItem;
class NewActionModel;
class QNetworkReply;
class QFile;
class QProgressDialog;
class QUndoGroup;
class QStandardItemModel;
class QCommandLineParser;
class QNetworkAccessManager;
class ActionDialog;
class QModelIndex;
class NewActionProxyModel;
class ScriptProxyModel;
class QHotkey;

#ifdef Q_OS_WIN
class QWinTaskbarButton;
class QWinTaskbarProgress;
#endif

#include <QNetworkReply>
#include <QSystemTrayIcon>

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QCommandLineParser &commandLineParser, ProgressSplashScreen *splashScreen, const QString &startScript, const QString &usedLocale);
	~MainWindow() override;

signals:
	void needToShow();

public slots:
    void otherInstanceMessage(const QString &message);

private slots:
	void postInit();
	void on_actionSave_triggered();
	void on_actionSave_as_triggered();
	void on_actionSave_copy_as_triggered();
	void on_actionOpen_triggered();
	void on_actionNew_triggered();
	void on_actionQuit_triggered();
	void on_deleteDropTarget_clicked();
	void on_actionDelete_action_triggered();
	void on_actionDelete_all_actions_triggered();
	void on_actionSelect_all_actions_triggered();
	void on_actionSelect_none_triggered();
	void on_actionInverse_selection_triggered();
	void on_actionAbout_triggered();
	void on_actionClear_triggered();
	void on_actionExport_executable_triggered();
	void on_actionSettings_triggered();
	void on_actionParameters_triggered();
	void on_actionMove_up_triggered();
	void on_actionMove_down_triggered();
	void on_actionExecute_triggered();
	void on_actionExecute_selection_triggered();
	void on_actionCut_triggered();
	void on_actionCopy_triggered();
	void on_actionPaste_triggered();
	void on_actionUndo_triggered();
	void on_actionRedo_triggered();
	void on_actionSet_action_color_triggered();
	void on_actionClear_selection_color_triggered();
	void on_actionEnable_all_actions_triggered();
	void on_actionDisable_all_actions_triggered();
	void on_actionEnable_selection_triggered();
	void on_actionDisable_selection_triggered();
	void on_actionNew_action_triggered();
	void on_actionEdit_action_triggered();
	void on_actionJump_to_line_triggered();
	void on_actionCheck_for_updates_triggered();
	void on_actionCreate_shortcut_triggered();
	void on_actionImport_script_content_triggered();
	void on_actionExport_script_content_triggered();
	void on_actionScriptSettings_triggered();
    void on_actionResources_triggered();
	void on_scriptView_customContextMenuRequested(const QPoint &pos);
	void on_actionHelp_triggered();
    void on_actionTake_screenshot_triggered();
    void on_actionsfilterLineEdit_textChanged(const QString &text);
    void on_scriptFilterLineEdit_textChanged(const QString &text);
    void on_scriptFilterCriteriaFlagsComboBox_flagsChanged(unsigned int flags);
	void systemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void scriptEdited();
	void actionSelectionChanged();
	void editAction(const QModelIndex &index);
	void wantToAddAction(const QString &actionId);
	void wantToAddAction(int row, const QString &actionId);
	void scriptFileDropped(const QString &scriptFilename);
	void scriptContentDropped(const QString &scriptContent);
	void addAction();
	void openRecentFile();
    void newActionDoubleClicked(const QModelIndex &index);
	void actionEnabled();
	void packLoadError(const QString &error);
	void stopExecution();
	void startOrStopExecution();
	void pauseOrResumeExecution();
	void scriptExecutionStopped();
	void postExecution();
	void logItemDoubleClicked(int itemRow);
	void logItemClicked(int itemRow);
#ifndef ACT_NO_UPDATER
	void updateError(const QString &message);
	void updateNoResult();
	void updateSuccess(const Tools::Version &version,
				 const QDate &releaseDate,
				 const QString &type,
				 const QString &changelog,
				 const QString &filename,
				 int size,
				 const QString &hash);
	void updateCanceled();
	void updateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	void updateDownloadFinished();
	void updateDownloadDataAvailable();
	void updateDownloadCanceled();
	void postDownloadOperation();
#endif

protected:
    void showEvent(QShowEvent *event) override;

private:
	void logItemClicked(int itemRow, bool doubleClick);
	void updateUndoRedoStatus();
	void execute(bool onlySelection);
    void fillNewActionModel();
    ActionDialog *actionDialog(ActionTools::ActionInstance *actionInstance);
	bool editAction(ActionTools::ActionInstance *actionInstance, const QString &field = QString(), const QString &subField = QString(), int line = -1, int column = -1);
	bool editAction(ActionTools::ActionInstance *actionInstance, int exception);
	void openParametersDialog(int parameter = -1, int line = -1, int column = -1);
    void openResourceDialog(const QString &resource = QString());
	QList<int> selectedRows() const;
	bool loadFile(const QString &fileName, bool verbose = true);
	bool saveFile(const QString &fileName, bool copy = false);
	void setCurrentFile(const QString &fileName);
	void closeEvent(QCloseEvent *event) override;
	bool maybeSave();
	bool save();
	bool saveAs();
	void deleteSelection();
	void actionSelectionChanged(int selectionCount);
	void actionCountChanged();
	void enabledActionsCountChanged(bool hasEnabledActions);
	void scriptWasModified(bool modified);
	void readSettings();
	void writeSettings();
	void updateRecentFileActions();
	void updateProxySettings();
	bool checkReadResult(ActionTools::Script::ReadResult result);
	void setTaskbarProgress(int value, int max);
    void enableTaskbarProgress(bool enable);
    ActionTools::Script::ReadResult readScript(QIODevice *device);
    bool writeScript(QIODevice *device);
    std::unique_ptr<QProgressDialog> createStandardProgressDialog();
    ActionDialog *getOrCreateActionDialog(const ActionTools::ActionDefinition *actionDefinition);
#ifndef ACT_NO_UPDATER
	void checkForUpdate(bool silent);
#endif

	Ui::MainWindow *ui;
	bool mScriptModified;
	QString mCurrentFile;
	int mMaxRecentFiles;
	QList<QAction *> mRecentFileActs;
	ActionTools::ActionFactory *mActionFactory;
	ActionTools::Script *mScript;
	ScriptModel *mScriptModel;
	QSystemTrayIcon *mSystemTrayIcon;
	ProgressSplashScreen *mSplashScreen;
	QFile *mFile;
	QStringList mPackLoadErrors;
	LibExecuter::Executer mExecuter;
	bool mWasNewActionDockShown;
	bool mWasConsoleDockShown;
	QUndoGroup *mUndoGroup;
	QPoint mPreviousWindowPosition;
	QStandardItemModel *mCompletionModel;
	QString mStartScript;
	QCommandLineParser &mCommandLineParser;
	int mAddActionRow;
	QString mAddAction;
	QAction *mStopExecutionAction;
    QMap<int, ActionDialog *> mActionDialogs;
    QString mUsedLocale;
    NewActionProxyModel *mNewActionProxyModel;
    ScriptProxyModel *mScriptProxyModel;
    NewActionModel *mNewActionModel;
    HeatmapMode mHeatmapMode;
    QHotkey *mStartStopExecutionHotkey;
    QHotkey *mPauseExecutionHotkey;
#ifndef ACT_NO_UPDATER
	QNetworkAccessManager *mNetworkAccessManager;
	QNetworkReply *mUpdateDownloadNetworkReply;
	Tools::Updater *mUpdater;
	bool mSilentUpdate;
	QProgressDialog *mUpdaterProgressDialog;
	bool mInstallAfterUpdateDownload;
	QFile mUpdateFile;
	int mUpdateFileSize;
	QString mUpdateFileHash;
	QCryptographicHash mHashCalculator;
#endif
#ifdef Q_OS_WIN
    QWinTaskbarButton *mTaskbarButton;
    QWinTaskbarProgress* mTaskbarProgress;
#endif

	Q_DISABLE_COPY(MainWindow)
};

