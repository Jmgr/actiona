/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef SCRIPTPARAMETERSDIALOG_H
#define SCRIPTPARAMETERSDIALOG_H

#include "scriptparameter.h"
#include "parametercontainer.h"

#include <QDialog>

namespace Ui
{
	class ScriptParametersDialog;
}

namespace ActionTools
{
	class CodeLineEdit;
	class Script;
	class CodeEditorDialog;
}

class QAbstractItemModel;

class ScriptParametersDialog : public QDialog, public ActionTools::ParameterContainer
{
	Q_OBJECT

public:
    ScriptParametersDialog(ActionTools::Script *script, QWidget *parent = 0);
	~ScriptParametersDialog();

	void setCurrentParameter(int parameter)						{ mCurrentParameter = parameter; }
	void setCurrentLine(int line)								{ mCurrentLine = line; }
	void setCurrentColumn(int column)							{ mCurrentColumn = column; }

	int exec();

    virtual QMenu *createVariablesMenu(QWidget *parent) const;

private slots:
	void postInit();
	void removeParameter();
	void accept();
	void parameterTypeChanged(int type);
	void on_addParameter_clicked();

private:
	void addParameter(const QString &name, const QString &value, bool code, ActionTools::ScriptParameter::ParameterType parameterType);
	void setupValueParameter(int row, ActionTools::ScriptParameter::ParameterType type, const QString &value = QString(), bool code = false);
	bool eventFilter(QObject *obj, QEvent *event);

    Ui::ScriptParametersDialog *ui;
	ActionTools::Script *mScript;
	int mCurrentParameter;
	int mCurrentLine;
	int mCurrentColumn;
	QList<ActionTools::ScriptParameter::ParameterType> mParameterTypes;

	Q_DISABLE_COPY(ScriptParametersDialog)
};

#endif // SCRIPTPARAMETERSDIALOG_H
