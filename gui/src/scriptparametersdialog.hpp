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

#pragma once

#include "actiontools/scriptparameter.hpp"
#include "actiontools/parametercontainer.hpp"

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
    ScriptParametersDialog(ActionTools::Script *script, QWidget *parent = nullptr);
	~ScriptParametersDialog() override;

	void setCurrentParameter(int parameter)						{ mCurrentParameter = parameter; }
	void setCurrentLine(int line)								{ mCurrentLine = line; }
	void setCurrentColumn(int column)							{ mCurrentColumn = column; }

	int exec() override;

    QMenu *createVariablesMenu(QWidget *parent) const override;

private slots:
	void postInit();
	void removeParameter();
	void accept() override;
	void parameterTypeChanged(int type);
	void on_addParameter_clicked();

private:
	void addParameter(const QString &name, const QString &value, bool code, ActionTools::ScriptParameter::ParameterType parameterType);
	void setupValueParameter(int row, ActionTools::ScriptParameter::ParameterType type, const QString &value = QString(), bool code = false);
	bool eventFilter(QObject *obj, QEvent *event) override;

    Ui::ScriptParametersDialog *ui;
	ActionTools::Script *mScript;
	int mCurrentParameter;
	int mCurrentLine;
	int mCurrentColumn;
	QList<ActionTools::ScriptParameter::ParameterType> mParameterTypes;

	Q_DISABLE_COPY(ScriptParametersDialog)
};

