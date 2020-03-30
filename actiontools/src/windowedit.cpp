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

#include "actiontools/windowedit.hpp"
#include "ui_windowedit.h"

#include "actiontools/codelineedit.hpp"

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
		ui->window->codeLineEdit()->setText(text);
	}

	QString WindowEdit::text() const
	{
		return ui->window->codeLineEdit()->text();
	}

	CodeLineEdit *WindowEdit::codeLineEdit() const
	{
		return ui->window->codeLineEdit();
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
		ui->window->codeLineEdit()->setFromSubParameter(subParameter);
	}

	void WindowEdit::openEditor(int line, int column)
	{
		ui->window->openEditor(line, column);
	}
	
	void WindowEdit::setCompletionModel(QAbstractItemModel *completionModel)
	{
        codeLineEdit()->setCompletionModel(completionModel);
    }

    void WindowEdit::setParameterContainer(const ParameterContainer *parameterContainer)
    {
        codeLineEdit()->setParameterContainer(parameterContainer);
    }

    QSet<QString> WindowEdit::findVariables() const
    {
        return codeLineEdit()->findVariables();
    }

	void WindowEdit::setWindowTitles(const QStringList &windowTitles)
	{
		ui->window->clear();
		ui->window->addItems(windowTitles);
	}

    void WindowEdit::on_choose_searchEnded(const ActionTools::WindowHandle &handle)
	{
		ui->window->codeLineEdit()->setText(handle.title());
	}
}
