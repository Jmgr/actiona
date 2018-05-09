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

#include "scriptcontentdialog.h"
#include "ui_scriptcontentdialog.h"
#include "script.h"
#include "global.h"

#include <QClipboard>
#include <QApplication>
#include <QMessageBox>

ScriptContentDialog::ScriptContentDialog(Type type, ActionTools::Script *script, QWidget *parent)
	: QDialog(parent),
	ui(new Ui::ScriptContentDialog),
	mType(type),
	mScript(script)
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

void ScriptContentDialog::accept()
{
	if(mType == Read)
	{
		QDialog::accept();
		
		return;
	}
	
	QString content = ui->scriptContent->toPlainText().trimmed();
    if(!content.isEmpty() && !mScript->validateContent(content, Global::SCRIPT_VERSION))
	{
		if(!mScript->statusMessage().isEmpty())
		{
			QMessageBox messageBox(tr("Script error"), tr("Unable to validate the script content.%1Line: %2<br>Column: %3")
								   .arg(mScript->statusMessage())
								   .arg(mScript->line())
								   .arg(mScript->column()), QMessageBox::Warning, QMessageBox::Ok, 0, 0, this);
            messageBox.setWindowFlags(messageBox.windowFlags() | Qt::WindowContextHelpButtonHint);
			messageBox.setTextFormat(Qt::RichText);
			messageBox.exec();
			
			ui->scriptContent->moveCursor(QTextCursor::Start);
			for(int i=1; i < mScript->line(); ++i)
				ui->scriptContent->moveCursor(QTextCursor::Down);
			
			for(int i=1; i < mScript->column(); ++i)
				ui->scriptContent->moveCursor(QTextCursor::Right);
		}
	
		return;
	}
	
	QDialog::accept();
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
