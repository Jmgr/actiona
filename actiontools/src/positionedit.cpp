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

#include "actiontools/positionedit.hpp"
#include "ui_positionedit.h"

#include <QRegularExpressionValidator>

namespace ActionTools
{
	PositionEdit::PositionEdit(QWidget *parent)
		: QWidget(parent),
        ui(new Ui::PositionEdit),
        mValidator(new QRegularExpressionValidator(QRegularExpression(QStringLiteral("^\\d+(\\.\\d{1,2})?:\\d+(\\.\\d{1,2})?$")), this))
	{
		ui->setupUi(this);

        ui->position->setValidator(mValidator);
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

    void PositionEdit::setParameterContainer(const ParameterContainer *parameterContainer)
    {
        codeLineEdit()->setParameterContainer(parameterContainer);
    }

    QSet<QString> PositionEdit::findVariables() const
    {
        return codeLineEdit()->findVariables();
    }

    void PositionEdit::setPosition(QPointF position)
	{
		ui->position->setText(QStringLiteral("%1:%2").arg(position.x()).arg(position.y()));
	}

    void PositionEdit::on_choose_positionChosen(QPointF position)
	{
		setPosition(position);
		
        emit positionChosen(position);
	}

	void PositionEdit::on_position_codeChanged(bool code)
	{
		if(code)
		{
			QString oldText = ui->position->text();
            ui->position->setValidator(nullptr);
			ui->position->setText(oldText);
		}
        else
            ui->position->setValidator(mValidator);
	}
}
