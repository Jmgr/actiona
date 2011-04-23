/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#include "codeeditordialog.h"
#include "script.h"
#include "codelineedit.h"
#include "codespinbox.h"
#include "windowedit.h"
#include "fileedit.h"
#include "linecombobox.h"

#include <QMessageBox>
#include <QScriptEngine>
#include <QTimer>
#include <QComboBox>

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

	ui->parameterTable->setColumnWidth(1, 250);

	int parameterCount = script->parameterCount();
	for(int parameterIndex = 0; parameterIndex < parameterCount; ++parameterIndex)
	{
		const ActionTools::ScriptParameter &parameter = script->parameter(parameterIndex);

		addParameter(parameter.name(), parameter.value(), parameter.isCode(), parameter.type());
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
		if(ui->parameterTable->cellWidget(row, 3) == removeParameterPushButton)
		{
			mParameterTypes.removeAt(row);
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

		bool isCode;
		QString value;

		switch(mParameterTypes.at(row))
		{
		case ActionTools::ScriptParameter::Text:
			{
				ActionTools::CodeLineEdit *valueWidget = qobject_cast<ActionTools::CodeLineEdit *>(widget);

				isCode = valueWidget->isCode();
				value = valueWidget->text();
			}
			break;
		case ActionTools::ScriptParameter::Number:
			{
				ActionTools::CodeSpinBox *valueWidget = qobject_cast<ActionTools::CodeSpinBox *>(widget);

				isCode = valueWidget->isCode();
				value = valueWidget->text();
			}
			break;
		case ActionTools::ScriptParameter::Window:
			{
				ActionTools::WindowEdit *valueWidget = qobject_cast<ActionTools::WindowEdit *>(widget);

				isCode = valueWidget->isCode();
				value = valueWidget->text();
			}
			break;
		case ActionTools::ScriptParameter::File:
			{
				ActionTools::FileEdit *valueWidget = qobject_cast<ActionTools::FileEdit *>(widget);

				isCode = valueWidget->isCode();
				value = valueWidget->text();
			}
			break;
		case ActionTools::ScriptParameter::Line:
			{
				ActionTools::LineComboBox *valueWidget = qobject_cast<ActionTools::LineComboBox *>(widget);

				isCode = valueWidget->isCode();
				value = valueWidget->codeLineEdit()->text();
			}
			break;
		}

		if(isCode)
		{
			QScriptSyntaxCheckResult result = scriptEngine.checkSyntax(value);
			if(result.state() != QScriptSyntaxCheckResult::Valid)
			{
				QMessageBox::warning(this, tr("Script parameter error"), tr("The script parameter named \"%1\" contains an error: \"%2\", please correct it.")
					.arg(nameLineEdit->text())
					.arg(result.errorMessage()));
				widget->setFocus();

				return;
			}
		}

		mScript->addParameter(ActionTools::ScriptParameter(nameLineEdit->text(), value, isCode, mParameterTypes.at(row)));
	}

	QDialog::accept();
}

void ScriptParametersDialog::parameterTypeChanged(int type)
{
	for(int row = 0; row < ui->parameterTable->rowCount(); ++row)
	{
		if(ui->parameterTable->cellWidget(row, 2) == sender())
		{
			setupValueParameter(row, static_cast<ActionTools::ScriptParameter::ParameterType>(type));

			break;
		}
	}
}

void ScriptParametersDialog::on_addParameter_clicked()
{
	addParameter(QString(), QString(), false, ActionTools::ScriptParameter::Text);
}

void ScriptParametersDialog::addParameter(const QString &name, const QString &value, bool code, ActionTools::ScriptParameter::ParameterType parameterType)
{
	int rowCount = ui->parameterTable->rowCount();

	ui->parameterTable->insertRow(rowCount);

	QLineEdit *nameLineEdit = new QLineEdit(this);
	nameLineEdit->setText(name);

	ui->parameterTable->setCellWidget(rowCount, 0, nameLineEdit);

	mParameterTypes.append(parameterType);
	setupValueParameter(rowCount, parameterType, value, code);

	QComboBox *typeComboBox = new QComboBox(this);
	typeComboBox->addItems(QStringList() << tr("Text") << tr("Number") << tr("Window title") << tr("File") << tr("Line"));
	typeComboBox->setCurrentIndex(parameterType);
	typeComboBox->installEventFilter(this);
	connect(typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(parameterTypeChanged(int)));
	ui->parameterTable->setCellWidget(rowCount, 2, typeComboBox);

	QPushButton *removeParameterPushButton = new QPushButton(tr("Remove"), this);
	connect(removeParameterPushButton, SIGNAL(clicked()), this, SLOT(removeParameter()));

	ui->parameterTable->setCellWidget(rowCount, 3, removeParameterPushButton);
}

void ScriptParametersDialog::setupValueParameter(int row, ActionTools::ScriptParameter::ParameterType type, const QString &value, bool code)
{
	switch(type)
	{
	case ActionTools::ScriptParameter::Text:
		{
			ActionTools::CodeLineEdit *valueWidget = new ActionTools::CodeLineEdit(this);
			valueWidget->setCode(code);
			valueWidget->setText(value);

			ui->parameterTable->setCellWidget(row, 1, valueWidget);
		}
		break;
	case ActionTools::ScriptParameter::Number:
		{
			ActionTools::CodeSpinBox *valueWidget = new ActionTools::CodeSpinBox(this);
			valueWidget->setRange(INT_MIN, INT_MAX);
			valueWidget->setCode(code);
			valueWidget->codeLineEdit()->setText(value);

			ui->parameterTable->setCellWidget(row, 1, valueWidget);
		}
		break;
	case ActionTools::ScriptParameter::Window:
		{
			ActionTools::WindowEdit *valueWidget = new ActionTools::WindowEdit(this);
			valueWidget->setCode(code);
			valueWidget->codeLineEdit()->setText(value);

			ui->parameterTable->setCellWidget(row, 1, valueWidget);
		}
		break;
	case ActionTools::ScriptParameter::File:
		{
			ActionTools::FileEdit *valueWidget = new ActionTools::FileEdit(this);
			valueWidget->setCode(code);
			valueWidget->codeLineEdit()->setText(value);

			ui->parameterTable->setCellWidget(row, 1, valueWidget);
		}
		break;
	case ActionTools::ScriptParameter::Line:
		{
			ActionTools::LineComboBox *valueWidget = new ActionTools::LineComboBox(mScript->labels(), mScript->actionCount(), this);
			valueWidget->setCode(code);
			valueWidget->codeLineEdit()->setText(value);

			ui->parameterTable->setCellWidget(row, 1, valueWidget);
		}
		break;
	}

	ui->parameterTable->resizeRowToContents(row);

	mParameterTypes[row] = type;
}

bool ScriptParametersDialog::eventFilter(QObject *obj, QEvent *event)
{
	if(event->type() == QEvent::Wheel)
	{
		QApplication::sendEvent(this, event);

		return true;
	}

	return QObject::eventFilter(obj, event);
}
