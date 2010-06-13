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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

/*
  Note for X11 Clipboard action :
  Lastly, the X11 clipboard is event driven, i.e. the clipboard will not function properly
  if the event loop is not running. Similarly, it is recommended that the contents of
  the clipboard are stored or retrieved in direct response to user-input events,
  e.g. mouse button or key presses and releases.
  ******************************************************************************************************************
  ******You should not store or retrieve the clipboard contents in response to timer or non-user-input events.******
  ******************************************************************************************************************
  Note for X11 :
  Remember to allow choosing between the copy/paste buffer and the selection buffer
  Note for Mac :
  Allow setting the search buffer
  */

namespace Ui
{
	class MainWindow;
}

namespace ActionTools
{
	class ActionFactory;
	class Script;
	class Action;
}

class QToolBox;
class QSystemTrayIcon;
class ScriptModel;
class QSplashScreen;
class QTreeWidget;
class QTreeWidgetItem;
class NewActionTreeWidget;
class QNetworkReply;
class QFile;
class QProgressDialog;
class Executer;
class QUndoGroup;
class QStandardItemModel;
class QxtCommandOptions;

#include <QNetworkReply>
#include <QSystemTrayIcon>

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QxtCommandOptions *commandOptions, QSplashScreen *splashScreen, const QString &startScript);
	~MainWindow();

protected:
	void changeEvent(QEvent *e);

private slots:
	void postInit();
	void opacityOpenUpdate();
	void opacityCloseUpdate();
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
	void on_scriptView_customContextMenuRequested(const QPoint &pos);
	void systemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void scriptEdited();
	void actionSelectionChanged();
	void editAction(const QModelIndex &index);
	void wantToAddAction(const QString &actionId);
	void wantToAddAction(int row, const QString &actionId);
	void addAction();
	void openRecentFile();
	void newActionDoubleClicked(QTreeWidgetItem *item, int column);
	void actionEnabled();
	void packLoadError(const QString &error);
	void stopExecution();

	void scriptExecutionStopped();
	void postExecution();
	void logItemDoubleClicked(int itemRow);

private:
	void updateUndoRedoStatus();
	void execute(bool onlySelection);
	void fillNewActionTreeWidget(NewActionTreeWidget *widget);
	bool editAction(ActionTools::Action *action, const QString &field = QString(), const QString &subField = QString(), int line = -1, int column = -1);
	void openParametersDialog(int parameter = -1, int line = -1, int column = -1);
	QList<int> selectedRows() const;
	bool loadFile(const QString &fileName, bool verbose = true);
	bool saveFile(const QString &fileName, bool copy = false);
	void setCurrentFile(const QString &fileName);
	void closeEvent(QCloseEvent *event);
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

	Ui::MainWindow *ui;
	float mOpacity;
	QTimer *mOpacityTimer;
	bool mScriptModified;
	QString mCurrentFile;
	int mMaxRecentFiles;
	QList<QAction *> mRecentFileActs;
	ActionTools::ActionFactory *mActionFactory;
	ActionTools::Script *mScript;
	ScriptModel *mScriptModel;
	QSystemTrayIcon *mSystemTrayIcon;
	QSplashScreen *mSplashScreen;
	QNetworkReply *mReply;
	QFile *mFile;
	QProgressDialog *mProg;
	QStringList mPackLoadErrors;
	Executer *mExecuter;
	bool mWasNewActionDockShown;
	bool mWasConsoleDockShown;
	QUndoGroup *mUndoGroup;
	QPoint mPreviousWindowPosition;
	QStandardItemModel *mCompletionModel;
	QString mStartScript;
	QxtCommandOptions *mCommandOptions;
	int mAddActionRow;
	QString mAddAction;
	QAction *mStopExecutionAction;

	Q_DISABLE_COPY(MainWindow)
};

#endif // MAINWINDOW_H
