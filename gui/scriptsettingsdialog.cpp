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

#include "scriptsettingsdialog.h"
#include "ui_scriptsettingsdialog.h"
#include "script.h"

#include <limits>

ScriptSettingsDialog::ScriptSettingsDialog(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::ScriptSettingsDialog)
{
    ui->setupUi(this);

	ui->pauseBeforeSpinBox->setRange(0, std::numeric_limits<int>::max());
	ui->pauseAfterSpinBox->setRange(0, std::numeric_limits<int>::max());
}

ScriptSettingsDialog::~ScriptSettingsDialog()
{
    delete ui;
}

void ScriptSettingsDialog::setPauseBefore(int pauseBefore)
{
	ui->pauseBeforeSpinBox->setValue(pauseBefore);
}

void ScriptSettingsDialog::setPauseAfter(int pauseAfter)
{
	ui->pauseAfterSpinBox->setValue(pauseAfter);
}

int ScriptSettingsDialog::pauseBefore() const
{
	return ui->pauseBeforeSpinBox->value();
}

int ScriptSettingsDialog::pauseAfter() const
{
	return ui->pauseAfterSpinBox->value();
}
