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

#include "sfxscriptdialog.hpp"
#include "ui_sfxscriptdialog.h"

#include <QSettings>

SFXScriptDialog::SFXScriptDialog(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::SFXScriptDialog)
{
	ui->setupUi(this);

	QSettings settings;
	ui->disableTrayIcon->setChecked(settings.value(QStringLiteral("sfxScript/disableTrayIcon"), true).toBool());
	ui->showConsole->setChecked(settings.value(QStringLiteral("sfxScript/showConsole"), false).toBool());
	ui->showExecutionWindow->setChecked(settings.value(QStringLiteral("sfxScript/showExecutionWindow"), false).toBool());
	ui->closeAfterExecution->setChecked(settings.value(QStringLiteral("sfxScript/closeAfterExecution"), true).toBool());
    ui->requiresActiona->setChecked(settings.value(QStringLiteral("sfxScript/requiresActiona"), false).toBool());
}

SFXScriptDialog::~SFXScriptDialog()
{
	delete ui;
}

bool SFXScriptDialog::disableTrayIcon() const
{
	return ui->disableTrayIcon->isChecked();
}

bool SFXScriptDialog::showConsole() const
{
	return ui->showConsole->isChecked();
}

bool SFXScriptDialog::showExecutionWindow() const
{
	return ui->showExecutionWindow->isChecked();
}

bool SFXScriptDialog::closeAfterExecution() const
{
	return ui->closeAfterExecution->isChecked();
}

bool SFXScriptDialog::requiresActiona() const
{
    return ui->requiresActiona->isChecked();
}

void SFXScriptDialog::accept()
{
	QSettings settings;
	settings.setValue(QStringLiteral("sfxScript/disableTrayIcon"), ui->disableTrayIcon->isChecked());
	settings.setValue(QStringLiteral("sfxScript/showConsole"), ui->showConsole->isChecked());
	settings.setValue(QStringLiteral("sfxScript/showExecutionWindow"), ui->showExecutionWindow->isChecked());
	settings.setValue(QStringLiteral("sfxScript/closeAfterExecution"), ui->closeAfterExecution->isChecked());
	settings.setValue(QStringLiteral("sfxScript/requiresActiona"), ui->requiresActiona->isChecked());

	QDialog::accept();
}
