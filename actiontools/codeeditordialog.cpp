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

#include "codeeditordialog.h"
#include "ui_codeeditordialog.h"
#include "recorder.h"

#include <QtScript>
#include <QSettings>
#include <QMessageBox>

//#include <windows.h>//TMP

namespace ActionTools
{
	CodeEditorDialog::CodeEditorDialog(QAbstractItemModel *completionModel, QWidget *parent)
		: QDialog(parent),
		ui(new Ui::CodeEditorDialog)
//		mRecorder(new Recorder(this))
	{
		ui->setupUi(this);
		
		ui->editor->setCompletionModel(completionModel);

		connect(ui->editor, SIGNAL(textChanged()), this, SLOT(textChanged()));

		QSettings settings;

		QAction *swapCodeAction = new QAction(this);
		swapCodeAction->setShortcut(QKeySequence(settings.value("actions/switchTextCode", QKeySequence("Ctrl+Shift+C")).toString()));
		swapCodeAction->setShortcutContext(Qt::WindowShortcut);
		addAction(swapCodeAction);

		connect(swapCodeAction, SIGNAL(triggered()), this, SLOT(swapCode()));
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

	void CodeEditorDialog::changeEvent(QEvent *e)
	{
		QDialog::changeEvent(e);
		switch(e->type())
		{
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
		}
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

	void CodeEditorDialog::on_hook_toggled(bool checked)
	{
		Q_UNUSED(checked);
	}
/*
	void kbdSimule(TCHAR *psz)
	{
	INPUT npt;
	npt.ki.dwExtraInfo = npt.ki.time = 0;
	npt.type = INPUT_KEYBOARD;
	npt.ki.wVk = 0;
	while(*psz) {
	#ifdef UNICODE
	npt.ki.wScan = (WORD) *psz;
	#else
	npt.ki.wScan = (WORD) (BYTE) *psz;
	#endif
	npt.ki.dwFlags = KEYEVENTF_UNICODE;
	SendInput(1, &npt, sizeof(INPUT));
	npt.ki.dwFlags = KEYEVENTF_UNICODE| KEYEVENTF_KEYUP;
	SendInput(1, &npt, sizeof(INPUT));
	psz++;
	}
   }
*/
	void CodeEditorDialog::on_test_clicked()
	{
		ui->editor->setFocus();

//		kbdSimule(L"ש&$פי");
		//QString t("א");
	//	wchar_t buffer[] = L"ש&$י";
		//t.toWCharArray(buffer);

//TMP
/*
		INPUT inputfn;
		inputfn.type = INPUT_KEYBOARD;
		inputfn.ki.wVk = 0;
		inputfn.ki.wScan = 6;
		inputfn.ki.time = 0;
		inputfn.ki.dwExtraInfo = GetMessageExtraInfo();
		inputfn.ki.dwFlags = KEYEVENTF_UNICODE;
		if(!SendInput(1, &inputfn, sizeof(INPUT)))
			qDebug() << "Fail : " << GetLastError();
			*/
/*
		inputfn.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
		if(!SendInput(1, &inputfn, sizeof(INPUT)))
			qDebug() << "Fail : " << GetLastError();
			*/
	}

	void CodeEditorDialog::swapCode()
	{
		setCode(!ui->editor->isCode());
	}

	void CodeEditorDialog::textChanged()
	{
		/*
		const QString &text = ui->editor->toPlainText();

		//ui->errorList->clear();
		QScriptSyntaxCheckResult result = QScriptEngine::checkSyntax(text);
		if(result.state() != QScriptSyntaxCheckResult::Valid)
		{
			int line = result.errorLineNumber();
			if(line != -1)
			{
				moveCursorToLine(line);
				QTextCursor cur = ui->editor->textCursor();
				cur.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
				QTextCharFormat cf;
				cf.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
				cf.setUnderlineColor(Qt::red);
				cur.setCharFormat(cf);
				//TODO
				//ui->errorList->addItem(QString("%1 : %2").arg(line).arg(result.errorMessage()));
			}
		}
		else
		{
			QScriptEngine e;
			e.evaluate(text);
			if(e.hasUncaughtException())
			{
				//ui->errorList->addItem(QString("%1 : %2").arg(e.uncaughtExceptionLineNumber()).arg(e.uncaughtException().toString()));
			}
		}
		*/
	}

	void CodeEditorDialog::showSyntaxCheckError()
	{
		QString message;

		if(ui->editor->lastSyntaxError().isEmpty())
			message = tr("Syntax error detected.");
		else
			message = tr("Syntax error detected : %1").arg(ui->editor->lastSyntaxError());

		QMessageBox::warning(this, tr("Syntax error check"), message);
	}

	void CodeEditorDialog::moveCursorToLine(int line)
	{
		QTextCursor cur = ui->editor->textCursor();
		cur.movePosition(QTextCursor::Start);
		cur.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, line);
	}
}
