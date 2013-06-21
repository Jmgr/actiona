/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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

#ifndef EXECUTIONWINDOW_H
#define EXECUTIONWINDOW_H

#include <QWidget>

namespace Ui
{
	class ExecutionWindow;
}

namespace LibExecuter
{
	class ExecutionWindow : public QWidget
	{
		Q_OBJECT
	
	public:
		ExecutionWindow(QWidget *parent = 0);
		~ExecutionWindow();

		void setCurrentActionName(const QString &actionName);
		void setCurrentActionColor(const QColor &actionColor);
		void setProgressEnabled(bool enabled);
		void setProgressValue(int value);
		void setProgressMinimum(int minimum);
		void setProgressMaximum(int maximum);
		void setPauseStatus(bool paused);
	
	signals:
		void canceled();
		void paused();
		void debug();
		
	public slots:
		void onEvaluationResumed();
		void onEvaluationPaused();
		void enableDebug();
		void disableDebug();

	private slots:
		void on_pausePushButton_clicked();
		void on_debugPushButton_clicked();
	
	private:
		Ui::ExecutionWindow *ui;
	};
}

#endif // EXECUTIONWINDOW_H
