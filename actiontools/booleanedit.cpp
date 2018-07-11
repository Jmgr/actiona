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

#include "subparameter.h"
#include "booleanedit.h"
#include "codelineedit.h"
#include "ui_booleanedit.h"

namespace ActionTools
{
	BooleanEdit::BooleanEdit(QWidget *parent)
		: QWidget(parent),
		ui(new Ui::BooleanEdit)
	{
		ui->setupUi(this);

        ui->comboBox->addItems({QStringLiteral("\"true\""), QStringLiteral("\"false\"")});
		ui->comboBox->setVisible(false);
		ui->comboBox->setCode(true);
		ui->comboBox->codeLineEdit()->setAllowTextCodeChange(false);
		ui->comboBox->setCurrentIndex(0);
	}

	BooleanEdit::~BooleanEdit()
	{
		delete ui;
	}

	void BooleanEdit::setText(bool isCode, const QString &text)
	{
		setCode(isCode);

		if(isCode)
			ui->comboBox->lineEdit()->setText(text);
		else
		{
			if(text == QLatin1String("true") || text == QLatin1String("yes") || text == QLatin1String("1"))
				ui->checkBox->setChecked(true);
			else
				ui->checkBox->setChecked(false);
		}
	}

	QString BooleanEdit::text() const
	{
		if(isCode())
			return ui->comboBox->lineEdit()->text();
		else
			return (ui->checkBox->isChecked() ? QStringLiteral("true") : QStringLiteral("false"));
	}

	bool BooleanEdit::isCode() const
	{
		return ui->switchTextModePushButton->isChecked();
	}

	void BooleanEdit::setCode(bool code)
	{
		return ui->switchTextModePushButton->setChecked(code);
	}

	void BooleanEdit::setFromSubParameter(const SubParameter &subParameter)
	{
        setText(subParameter.isCode(), subParameter.value());

		on_switchTextModePushButton_clicked();
	}

	void BooleanEdit::openEditor(int line, int column)
	{
		ui->comboBox->openEditor(line, column);
	}

	void BooleanEdit::setCompletionModel(QAbstractItemModel *completionModel)
	{
        ui->comboBox->setCompletionModel(completionModel);
    }

    void BooleanEdit::setParameterContainer(const ParameterContainer *parameterContainer)
    {
        ui->comboBox->setParameterContainer(parameterContainer);
    }

    QSet<QString> BooleanEdit::findVariables() const
    {
        return ui->comboBox->findVariables();
    }

	void BooleanEdit::on_switchTextModePushButton_clicked()
	{
		bool pressed = ui->switchTextModePushButton->isChecked();

		ui->comboBox->setVisible(pressed);
		ui->checkBox->setVisible(!pressed);
	}
}
