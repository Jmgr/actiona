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

#include "codeeditordialog.h"
#include "ui_codeeditordialog.h"
#include "actioninstance.h"

#include <QtScript>
#include <QSettings>
#include <QMessageBox>
#include <QMenu>

#include <algorithm>

namespace ActionTools
{
    CodeEditorDialog::CodeEditorDialog(QAbstractItemModel *completionModel, QMenu *variablesMenu, QMenu *resourcesMenu, QWidget *parent)
		: QDialog(parent),
        ui(new Ui::CodeEditorDialog),
        mVariablesMenu(variablesMenu),
        mResourcesMenu(resourcesMenu)
	{
		ui->setupUi(this);
		
		ui->editor->setCompletionModel(completionModel);

		QSettings settings;

		auto swapCodeAction = new QAction(this);
		swapCodeAction->setShortcut(QKeySequence(settings.value(QStringLiteral("actions/switchTextCode"), QKeySequence(QStringLiteral("Ctrl+Shift+C"))).toString()));
		swapCodeAction->setShortcutContext(Qt::WindowShortcut);
		addAction(swapCodeAction);

        connect(swapCodeAction, &QAction::triggered, this, &CodeEditorDialog::swapCode);
        connect(ui->editor, &ActionTools::CodeEdit::acceptDialog, this, &CodeEditorDialog::accept);
        if(mResourcesMenu)
            connect(mResourcesMenu, &QMenu::triggered, this, static_cast<void (CodeEditorDialog::*)(QAction *action)>(&CodeEditorDialog::insertVariable));
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
        ui->codePushButton->setEnabled(allowTextCodeChange);
        ui->textPushButton->setEnabled(allowTextCodeChange);
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
		if(settings.value(QStringLiteral("actions/checkCodeSyntaxAutomatically"), QVariant(true)).toBool() && !ui->editor->checkSyntax())
			showSyntaxCheckError();
		else
			QDialog::accept();
	}

	void CodeEditorDialog::on_codePushButton_toggled(bool checked)
	{
		ui->editor->setCode(checked);
        ui->checkSyntax->setEnabled(checked);
    }

    void CodeEditorDialog::on_insertPushButton_clicked()
    {
        QSet<QString> variables = ActionTools::ActionInstance::findVariables(text(), isCode());

        for(QAction *action: mVariablesMenu->actions())
            variables.insert(action->text());

        QStringList variableList = variables.toList();
        std::sort(variableList.begin(), variableList.end());

        QMenu *variablesMenu = nullptr;

        if(variableList.isEmpty())
        {
            variablesMenu = new QMenu(tr("No variables to insert"));
            variablesMenu->setEnabled(false);
        }
        else
        {
            variablesMenu = new QMenu(tr("Insert variable"));
            connect(variablesMenu, &QMenu::triggered, this, static_cast<void (CodeEditorDialog::*)(QAction *action)>(&CodeEditorDialog::insertVariable));
            for(const QString &variable: variableList)
                variablesMenu->addAction(variable);
        }

		variablesMenu->setIcon(QIcon(QStringLiteral(":/images/variable.png")));

        auto menu = new QMenu;

        menu->addMenu(variablesMenu);
        menu->addMenu(mResourcesMenu);

        menu->exec(QCursor::pos());

        delete menu;
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

    void CodeEditorDialog::insertVariable(QAction *action)
    {
        insertVariable(action->text());
    }

    void CodeEditorDialog::insertVariable(const QString &variable)
    {
        if(isCode())
            ui->editor->insertPlainText(variable);
        else
			ui->editor->insertPlainText(QStringLiteral("$") + variable);
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
