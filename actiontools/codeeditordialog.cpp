/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#include "codeeditordialog.h"
#include "ui_codeeditordialog.h"

#include <QtScript>
#include <QSettings>
#include <QMessageBox>

namespace ActionTools
{
	CodeEditorDialog::CodeEditorDialog(QAbstractItemModel *completionModel, QWidget *parent)
		: QDialog(parent),
		ui(new Ui::CodeEditorDialog)
	{
		ui->setupUi(this);
		
		ui->editor->setCompletionModel(completionModel);

		QSettings settings;

		QAction *swapCodeAction = new QAction(this);
		swapCodeAction->setShortcut(QKeySequence(settings.value("actions/switchTextCode", QKeySequence("Ctrl+Shift+C")).toString()));
		swapCodeAction->setShortcutContext(Qt::WindowShortcut);
		addAction(swapCodeAction);

		connect(swapCodeAction, SIGNAL(triggered()), this, SLOT(swapCode()));
		connect(ui->editor, SIGNAL(acceptDialog()), this, SLOT(accept()));
	}

	CodeEditorDialog::~CodeEditorDialog()
	{
		delete ui;
	}

	void CodeEditorDialog::setText(const QString &text)
	{
		ui->editor->setPlainText(text);
	}

	void CodeEditorDialog::setCode(bool code)
	{
		ui->codePushButton->setChecked(code);
		ui->textPushButton->setChecked(!code);
		ui->editor->setCode(code);
		ui->checkSyntax->setEnabled(code);
	}

	void CodeEditorDialog::setAllowTextCodeChange(bool allowTextCodeChange)
	{
		ui->textCodeGroupBox->setEnabled(allowTextCodeChange);
	}

	void CodeEditorDialog::setCurrentLine(int line)
	{
		ui->editor->setCurrentLine(line);
	}

	void CodeEditorDialog::setCurrentColumn(int column)
	{
		ui->editor->setCurrentColumn(column);
	}

	QString CodeEditorDialog::text() const
	{
		return ui->editor->toPlainText();
	}

	bool CodeEditorDialog::isCode() const
	{
		return ui->codePushButton->isChecked();
	}
	
	void CodeEditorDialog::accept()
	{
		QSettings settings;
		if(settings.value("actions/checkCodeSyntaxAutomatically", QVariant(true)).toBool() && !ui->editor->checkSyntax())
			showSyntaxCheckError();
		else
			QDialog::accept();
	}

	void CodeEditorDialog::on_codePushButton_toggled(bool checked)
	{
		ui->editor->setCode(checked);
		ui->checkSyntax->setEnabled(checked);
	}

	void CodeEditorDialog::on_checkSyntax_clicked()
	{
		if(ui->editor->checkSyntax())
			QMessageBox::information(this, tr("Syntax error check"), tr("No syntax errors where found."));
		else
			showSyntaxCheckError();
	}

	void CodeEditorDialog::swapCode()
	{
		setCode(!ui->editor->isCode());
	}

	void CodeEditorDialog::showSyntaxCheckError()
	{
		QString message;

		if(ui->editor->lastSyntaxError().isEmpty())
			message = tr("Syntax error detected.");
		else
			message = tr("Syntax error detected: %1").arg(ui->editor->lastSyntaxError());

		QMessageBox::warning(this, tr("Syntax error check"), message);
	}

	void CodeEditorDialog::moveCursorToLine(int line)
	{
		QTextCursor cur = ui->editor->textCursor();
		cur.movePosition(QTextCursor::Start);
		cur.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, line);
	}
}
