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

#include "positionedit.h"
#include "ui_positionedit.h"

namespace ActionTools
{
	PositionEdit::PositionEdit(QWidget *parent)
		: QWidget(parent),
		ui(new Ui::PositionEdit)
	{
		ui->setupUi(this);

		ui->position->setInputMask("#000900:#009000");
	}

	PositionEdit::~PositionEdit()
	{
		delete ui;
	}

	void PositionEdit::setText(const QString &text)
	{
		ui->position->setText(text);
	}

	QString PositionEdit::text() const
	{
		return ui->position->text();
	}

	CodeLineEdit *PositionEdit::codeLineEdit() const
	{
		return ui->position;
	}

	bool PositionEdit::isCode() const
	{
		return ui->position->isCode();
	}

	void PositionEdit::setCode(bool code)
	{
		ui->position->setCode(code);
	}

	void PositionEdit::setFromSubParameter(const SubParameter &subParameter)
	{
		ui->position->setFromSubParameter(subParameter);
	}

	void PositionEdit::openEditor(int line, int column)
	{
		ui->position->openEditor(line, column);
	}
	
	void PositionEdit::setCompletionModel(QAbstractItemModel *completionModel)
	{
		codeLineEdit()->setCompletionModel(completionModel);
	}

	void PositionEdit::setPosition(QPoint position)
	{
		ui->position->setText(QString("%1:%2").arg(position.x()).arg(position.y()));
	}

	void PositionEdit::on_choose_positionChosen(QPoint position)
	{
		setPosition(position);
		
		emit positionChosen(position);
	}

	void PositionEdit::on_position_codeChanged(bool code)
	{
		if(code)
		{
			QString oldText = ui->position->text();
			ui->position->setInputMask(QString());
			ui->position->setText(oldText);
		}
		else
			ui->position->setInputMask("#000900:#009000");
	}
}
