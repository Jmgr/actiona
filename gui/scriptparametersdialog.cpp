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

#include "scriptparametersdialog.h"
#include "ui_scriptparametersdialog.h"

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
#include <QMenu>

#include <limits>
#include <algorithm>

ScriptParametersDialog::ScriptParametersDialog(ActionTools::Script *script, QWidget *parent)
	: QDialog(parent),
    ParameterContainer(script),
    ui(new Ui::ScriptParametersDialog),
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

QMenu *ScriptParametersDialog::createVariablesMenu(QWidget *parent) const
{
    int rowCount = ui->parameterTable->rowCount();

    QSet<QString> variableSet;
    for(int row = 0; row < rowCount; ++row)
    {
        QWidget *nameWidget = ui->parameterTable->cellWidget(row, 0);
        auto nameLineEdit = qobject_cast<QLineEdit *>(nameWidget);
        QWidget *valueWidget = ui->parameterTable->cellWidget(row, 1);
        if(!nameLineEdit || !valueWidget)
            continue;

        if(!nameLineEdit->text().isEmpty())
            variableSet.insert(nameLineEdit->text());

        if(auto codeEditor = dynamic_cast<ActionTools::AbstractCodeEditor *>(valueWidget))
            variableSet.unite(codeEditor->findVariables());
    }

    QStringList variableList = variableSet.toList();
    std::sort(variableList.begin(), variableList.end());

    if(variableList.isEmpty())
        return nullptr;

    auto back = new QMenu(parent);

    for(const QString &variable: variableList)
        back->addAction(variable);

    return back;
}

void ScriptParametersDialog::postInit()
{
	if(mCurrentParameter != -1)
	{
		QWidget *widget = ui->parameterTable->cellWidget(mCurrentParameter, 1);
		widget->setFocus();

		if(mCurrentLine != -1)
		{
			auto valueLineEdit = qobject_cast<ActionTools::CodeLineEdit *>(widget);
			if(valueLineEdit->text().contains(QLatin1Char('\n')))//Multiline : open the editor
			{
				if(auto codeEditor = dynamic_cast<ActionTools::AbstractCodeEditor *>(widget))
					codeEditor->openEditor(mCurrentLine, mCurrentColumn);
			}
		}
	}
}

void ScriptParametersDialog::removeParameter()
{
	auto removeParameterPushButton = qobject_cast<QPushButton *>(sender());
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

		auto nameLineEdit = qobject_cast<QLineEdit *>(widget);

		widget = ui->parameterTable->cellWidget(row, 1);
		if(!widget)
			continue;

		if(nameLineEdit->text().isEmpty())
			continue;

		QRegExp nameRegExp(QStringLiteral("[a-z_][a-z0-9_]*"), Qt::CaseInsensitive);
		if(!nameRegExp.exactMatch(nameLineEdit->text()))
		{
			QMessageBox::warning(this, tr("Script parameter error"), tr("Incorrect parameter name \"%1\".")
				.arg(nameLineEdit->text()));
			nameLineEdit->setFocus();

			return;
		}

		bool isCode = false;
		QString value;

		switch(mParameterTypes.at(row))
		{
		case ActionTools::ScriptParameter::Text:
			{
				auto valueWidget = qobject_cast<ActionTools::CodeLineEdit *>(widget);

				isCode = valueWidget->isCode();
				value = valueWidget->text();
			}
			break;
		case ActionTools::ScriptParameter::Number:
			{
				auto valueWidget = qobject_cast<ActionTools::CodeSpinBox *>(widget);

				isCode = valueWidget->isCode();
				value = valueWidget->text();
			}
			break;
		case ActionTools::ScriptParameter::Window:
			{
				auto valueWidget = qobject_cast<ActionTools::WindowEdit *>(widget);

				isCode = valueWidget->isCode();
				value = valueWidget->text();
			}
			break;
		case ActionTools::ScriptParameter::File:
			{
				auto valueWidget = qobject_cast<ActionTools::FileEdit *>(widget);

				isCode = valueWidget->isCode();
				value = valueWidget->text();
			}
			break;
		case ActionTools::ScriptParameter::Line:
			{
				auto valueWidget = qobject_cast<ActionTools::LineComboBox *>(widget);

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
				QMessageBox::warning(this, tr("Script parameter error"), tr(R"(The script parameter named "%1" contains an error: "%2", please correct it.)")
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

	auto nameLineEdit = new QLineEdit(this);
	nameLineEdit->setText(name);

	ui->parameterTable->setCellWidget(rowCount, 0, nameLineEdit);

	mParameterTypes.append(parameterType);
	setupValueParameter(rowCount, parameterType, value, code);

	auto typeComboBox = new QComboBox(this);
    typeComboBox->addItems({tr("Text"), tr("Number"), tr("Window title"), tr("File"), tr("Line")});
	typeComboBox->setCurrentIndex(parameterType);
	typeComboBox->installEventFilter(this);
    connect(typeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ScriptParametersDialog::parameterTypeChanged);
	ui->parameterTable->setCellWidget(rowCount, 2, typeComboBox);

	QPushButton *removeParameterPushButton = new QPushButton(tr("Remove"), this);
    connect(removeParameterPushButton, &QPushButton::clicked, this, &ScriptParametersDialog::removeParameter);

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
            valueWidget->setParameterContainer(this);

			ui->parameterTable->setCellWidget(row, 1, valueWidget);
		}
		break;
	case ActionTools::ScriptParameter::Number:
		{
			auto valueWidget = new ActionTools::CodeSpinBox(this);
			valueWidget->setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
			valueWidget->setCode(code);
			valueWidget->codeLineEdit()->setText(value);
            valueWidget->setParameterContainer(this);

			ui->parameterTable->setCellWidget(row, 1, valueWidget);
		}
		break;
	case ActionTools::ScriptParameter::Window:
		{
			auto valueWidget = new ActionTools::WindowEdit(this);
			valueWidget->setCode(code);
			valueWidget->codeLineEdit()->setText(value);
            valueWidget->setParameterContainer(this);
            valueWidget->setWindowTitles(ActionTools::WindowHandle::windowTitles());

			ui->parameterTable->setCellWidget(row, 1, valueWidget);
		}
		break;
	case ActionTools::ScriptParameter::File:
		{
			auto valueWidget = new ActionTools::FileEdit(this);
			valueWidget->setCode(code);
			valueWidget->codeLineEdit()->setText(value);
            valueWidget->setParameterContainer(this);

			ui->parameterTable->setCellWidget(row, 1, valueWidget);
		}
		break;
	case ActionTools::ScriptParameter::Line:
		{
            ActionTools::LineComboBox *valueWidget = new ActionTools::LineComboBox(*mScript, this);
            valueWidget->setValue(code, value);
            valueWidget->setParameterContainer(this);

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
