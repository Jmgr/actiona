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

#include "sizeedit.h"
#include "ui_sizeedit.h"

namespace ActionTools
{
	SizeEdit::SizeEdit(QWidget *parent)
		: QWidget(parent),
		ui(new Ui::SizeEdit)
	{
		ui->setupUi(this);

		ui->position->setInputMask("#000900:#009000");
	}

	SizeEdit::~SizeEdit()
	{
		delete ui;
	}

	void SizeEdit::setText(const QString &text)
	{
		ui->position->setText(text);
	}

	QString SizeEdit::text() const
	{
		return ui->position->text();
	}

	CodeLineEdit *SizeEdit::codeLineEdit() const
	{
		return ui->position;
	}

	bool SizeEdit::isCode() const
	{
		return ui->position->isCode();
	}

	void SizeEdit::setCode(bool code)
	{
		ui->position->setCode(code);
	}

	void SizeEdit::setFromSubParameter(const SubParameter &subParameter)
	{
		ui->position->setFromSubParameter(subParameter);
	}

	void SizeEdit::openEditor(int line, int column)
	{
		ui->position->openEditor(line, column);
	}
	
	void SizeEdit::setCompletionModel(QAbstractItemModel *completionModel)
	{
		codeLineEdit()->setCompletionModel(completionModel);
	}

	void SizeEdit::setPosition(QPoint position)
	{
		ui->position->setText(QString("%1:%2").arg(position.x()).arg(position.y()));
	}

	void SizeEdit::on_choose_positionChosen(QPoint position)
	{
		setPosition(position);
		
		emit positionChosen(position);
	}

	void SizeEdit::on_position_codeChanged(bool code)
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
