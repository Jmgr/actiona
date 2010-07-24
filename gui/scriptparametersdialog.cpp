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

#include "scriptparametersdialog.h"
#include "ui_scriptparametersdialog.h"

#include "codelineedit.h"
#include "codeeditordialog.h"
#include "script.h"

#include <QMessageBox>
#include <QScriptEngine>
#include <QTimer>

ScriptParametersDialog::ScriptParametersDialog(QAbstractItemModel *completitionModel, ActionTools::Script *script, QWidget *parent)
	: QDialog(parent),
	ui(new Ui::ScriptParametersDialog),
	mCodeEditorDialog(new ActionTools::CodeEditorDialog(completitionModel, this)),
	mScript(script),
	mCurrentParameter(-1),
	mCurrentLine(-1),
	mCurrentColumn(-1)
{
	ui->setupUi(this);

	int parameterCount = script->parameterCount();
	for(int parameterIndex = 0; parameterIndex < parameterCount; ++parameterIndex)
	{
		const ActionTools::ScriptParameter &parameter = script->parameter(parameterIndex);

		addParameter(parameter.name(), parameter.value(), parameter.isCode());
	}

	on_addParameter_clicked();//Add an empty parameter at the end
}

ScriptParametersDialog::~ScriptParametersDialog()
{
	delete ui;
}

int ScriptParametersDialog::exec()
{
	QTimer::singleShot(1, this, SLOT(postInit()));

	return QDialog::exec();
}

void ScriptParametersDialog::addParameter(const QString &name, const QString &value, bool code)
{
	int rowCount = ui->parameterTable->rowCount();

	ui->parameterTable->insertRow(rowCount);

	QLineEdit *nameLineEdit = new QLineEdit(this);
	nameLineEdit->setText(name);

	ui->parameterTable->setCellWidget(rowCount, 0, nameLineEdit);

	ActionTools::CodeLineEdit *valueLineEdit = new ActionTools::CodeLineEdit(this);
	valueLineEdit->setCode(code);
	valueLineEdit->setText(value);

	ui->parameterTable->setCellWidget(rowCount, 1, valueLineEdit);

	QPushButton *removeParameterPushButton = new QPushButton(tr("Remove"), this);
	connect(removeParameterPushButton, SIGNAL(clicked()), this, SLOT(removeParameter()));

	ui->parameterTable->setCellWidget(rowCount, 2, removeParameterPushButton);
}

void ScriptParametersDialog::postInit()
{
	if(mCurrentParameter != -1)
	{
		QWidget *widget = ui->parameterTable->cellWidget(mCurrentParameter, 1);
		widget->setFocus();

		if(mCurrentLine != -1)
		{
			ActionTools::CodeLineEdit *valueLineEdit = qobject_cast<ActionTools::CodeLineEdit *>(widget);
			if(valueLineEdit->text().contains('\n'))//Multiline : open the editor
			{
				if(ActionTools::AbstractCodeEditor *codeEditor = dynamic_cast<ActionTools::AbstractCodeEditor *>(widget))
					codeEditor->openEditor(mCurrentLine, mCurrentColumn);
			}
		}
	}
}

void ScriptParametersDialog::removeParameter()
{
	QPushButton *removeParameterPushButton = qobject_cast<QPushButton *>(sender());
	if(!removeParameterPushButton)
		return;

	for(int row = 0; row < ui->parameterTable->rowCount(); ++row)
	{
		if(ui->parameterTable->cellWidget(row, 2) == removeParameterPushButton)
		{
			ui->parameterTable->removeRow(row);
			break;
		}
	}
}

void ScriptParametersDialog::accept()
{
	int rowCount = ui->parameterTable->rowCount();

	QScriptEngine scriptEngine;

	mScript->removeAllParameters();

	for(int row = 0; row < rowCount; ++row)
	{
		QWidget *widget = ui->parameterTable->cellWidget(row, 0);
		if(!widget)
			continue;

		QLineEdit *nameLineEdit = qobject_cast<QLineEdit *>(widget);

		widget = ui->parameterTable->cellWidget(row, 1);
		if(!widget)
			continue;

		ActionTools::CodeLineEdit *valueLineEdit = qobject_cast<ActionTools::CodeLineEdit *>(widget);

		if(nameLineEdit->text().isEmpty())
			continue;

		QRegExp nameRegExp("[a-z_][a-z0-9_]*", Qt::CaseInsensitive);
		if(!nameRegExp.exactMatch(nameLineEdit->text()))
		{
			QMessageBox::warning(this, tr("Script parameter error"), tr("Incorrect parameter name \"%1\".")
				.arg(nameLineEdit->text()));
			nameLineEdit->setFocus();

			return;
		}

		if(valueLineEdit->isCode())
		{
			QScriptSyntaxCheckResult result = scriptEngine.checkSyntax(valueLineEdit->text());
			if(result.state() != QScriptSyntaxCheckResult::Valid)
			{
				QMessageBox::warning(this, tr("Script parameter error"), tr("The script parameter named \"%1\" contains an error: \"%2\", please correct it.")
					.arg(nameLineEdit->text())
					.arg(result.errorMessage()));
				valueLineEdit->setFocus();

				return;
			}
		}

		mScript->addParameter(ActionTools::ScriptParameter(nameLineEdit->text(), valueLineEdit->text(), valueLineEdit->isCode()));
	}

	QDialog::accept();
}

void ScriptParametersDialog::on_addParameter_clicked()
{
	addParameter(QString(), QString(), false);
}
