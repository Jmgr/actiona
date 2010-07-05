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

#ifndef EXECUTIONWINDOW_H
#define EXECUTIONWINDOW_H

#include <QWidget>
#include <QKeySequence>

namespace Ui
{
	class ExecutionWindow;
}

class ExecutionWindow : public QWidget
{
	Q_OBJECT

public:
	ExecutionWindow(QWidget *parent = 0);
	~ExecutionWindow();

	void setStopExecutionShortcut(const QKeySequence &keySequence);
	void setCurrentActionName(const QString &actionName);
	void setCurrentActionColor(const QColor &actionColor);

signals:
	void canceled();

private:
	void changeEvent(QEvent *e);

	Ui::ExecutionWindow *ui;
};

#endif // EXECUTIONWINDOW_H
