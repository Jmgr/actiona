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

#ifndef ACTIONDIALOG_H
#define ACTIONDIALOG_H

#include <QDialog>

namespace Ui
{
	class ActionDialog;
}

namespace ActionTools
{
	class Action;
	class GroupDefinition;
	class ParameterDefinition;
	class Script;
}

class QAbstractItemModel;

class ActionDialog : public QDialog
{
	Q_OBJECT

public:
	ActionDialog(QAbstractItemModel *completionModel, ActionTools::Script *script, ActionTools::Action *action, QWidget *parent = 0);
	~ActionDialog();

	void setCurrentField(const QString &field, const QString &subField)				{ mCurrentField = field; mCurrentSubField = subField; }
	void setCurrentLine(int currentLine)											{ mCurrentLine = currentLine; }
	void setCurrentColumn(int currentColumn)										{ mCurrentColumn = currentColumn; }

public slots:
	void accept();
	int exec();

private slots:
	void postInit();

private:
	void addGroup(ActionTools::GroupDefinition *group);
	QLayout *addParameter(ActionTools::ParameterDefinition *parameter);
	void changeEvent(QEvent *event);

	Ui::ActionDialog *ui;
	ActionTools::Action *mAction;
	ActionTools::Script *mScript;
	QList<ActionTools::ParameterDefinition *> mParameters;
	QString mCurrentField;
	QString mCurrentSubField;
	int mCurrentLine;
	int mCurrentColumn;
	QAbstractItemModel *mCompletionModel;

	Q_DISABLE_COPY(ActionDialog)
};

#endif // ACTIONDIALOG_H
