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

#include "windowedit.h"
#include "ui_windowedit.h"

namespace ActionTools
{
	WindowEdit::WindowEdit(QWidget *parent)
		: QWidget(parent),
		ui(new Ui::WindowEdit)
	{
		ui->setupUi(this);
	}

	WindowEdit::~WindowEdit()
	{
		delete ui;
	}

	void WindowEdit::setText(const QString &text)
	{
		ui->window->setText(text);
	}

	QString WindowEdit::text() const
	{
		return ui->window->text();
	}

	CodeLineEdit *WindowEdit::codeLineEdit() const
	{
		return ui->window;
	}

	bool WindowEdit::isCode() const
	{
		return ui->window->isCode();
	}

	void WindowEdit::setCode(bool code)
	{
		ui->window->setCode(code);
	}

	void WindowEdit::setFromSubParameter(const SubParameter &subParameter)
	{
		ui->window->setFromSubParameter(subParameter);
	}

	void WindowEdit::openEditor(int line, int column)
	{
		ui->window->openEditor(line, column);
	}
	
	void WindowEdit::setCompletionModel(QAbstractItemModel *completionModel)
	{
		codeLineEdit()->setCompletionModel(completionModel);
	}

	void WindowEdit::on_choose_searchEnded(const WindowHandle &handle)
	{
		ui->window->setText(handle.title());
	}
}
