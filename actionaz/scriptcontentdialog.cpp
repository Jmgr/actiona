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

#include "scriptcontentdialog.h"
#include "ui_scriptcontentdialog.h"

#include <QClipboard>
#include <QApplication>

ScriptContentDialog::ScriptContentDialog(Type type, QWidget *parent)
	: QDialog(parent),
	ui(new Ui::ScriptContentDialog),
	mType(type)
{
    ui->setupUi(this);

	switch(mType)
	{
	case Read:
		setWindowTitle(tr("Export script content"));
		ui->clipboardButton->setText(tr("Copy to clipboard"));
		ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok);
		ui->scriptContent->setReadOnly(true);
		break;
	case Write:
		setWindowTitle(tr("Import script content"));
		ui->clipboardButton->setText(tr("Paste from clipboard"));
		break;
	}
}

ScriptContentDialog::~ScriptContentDialog()
{
    delete ui;
}

void ScriptContentDialog::on_clipboardButton_clicked()
{
	QClipboard *clipboard = QApplication::clipboard();

	switch(mType)
	{
	case Read:
		clipboard->setText(ui->scriptContent->toPlainText());
		break;
	case Write:
		ui->scriptContent->setPlainText(clipboard->text());
		break;
	}
}

void ScriptContentDialog::setText(const QString &text)
{
	ui->scriptContent->setPlainText(text);
}

QString ScriptContentDialog::text() const
{
	return ui->scriptContent->toPlainText();
}
