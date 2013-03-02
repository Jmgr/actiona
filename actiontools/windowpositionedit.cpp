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

//TODO
#include "windowpositionedit.h"
#include "ui_windowpositionedit.h"

#include <QColorDialog>
#include <QDesktopWidget>

namespace ActionTools
{
	WindowPositionEdit::WindowPositionEdit(QWidget *parent)
		: QWidget(parent),
		ui(new Ui::WindowPositionEdit),
		mScreenPositionDialog(new ScreenPositionDialog(this))
	{
		ui->setupUi(this);

		setChooseByPositionButtonVisible(false);
	}

	WindowPositionEdit::~WindowPositionEdit()
	{
		delete ui;
	}

	void WindowPositionEdit::setChooseByPositionButtonVisible(bool visible)
	{
		ui->chooseByPosition->setVisible(visible);
	}

	void WindowPositionEdit::setText(const QString &text)
	{
		ui->windowPositionLineEdit->setText(text);
	}

	QString WindowPositionEdit::text() const
	{
		return ui->windowPositionLineEdit->text();
	}

	bool WindowPositionEdit::isCode() const
	{
		return ui->windowPositionLineEdit->isCode();
	}

	void WindowPositionEdit::setCode(bool code)
	{
		return ui->windowPositionLineEdit->setCode(code);
	}

	void WindowPositionEdit::setFromSubParameter(const SubParameter &subParameter)
	{
		ui->windowPositionLineEdit->setFromSubParameter(subParameter);

		updateDialog(subParameter.value().toString());
	}

	void WindowPositionEdit::openEditor(int line, int column)
	{
		ui->windowPositionLineEdit->openEditor(line, column);
	}

	void WindowPositionEdit::setCompletionModel(QAbstractItemModel *completionModel)
	{
		codeLineEdit()->setCompletionModel(completionModel);
	}

	void WindowPositionEdit::on_chooseByPosition_positionChosen(QPoint position)
	{
		//setPosition(position);

		emit positionChosen(position);
	}

	void WindowPositionEdit::on_choose_clicked()
	{
		//mColorDialog->setCurrentColor(currentColor());
		//mColorDialog->open(this, SLOT(onColorSelected()));
		int result = mScreenPositionDialog->exec();
		if( result == QDialog::Accepted)
		{
			int screen = mScreenPositionDialog->windowPosition->screen();
			int position = mScreenPositionDialog->windowPosition->position();
			QString cardinalPosition = QString();
			if((position%3) == 0)
				cardinalPosition += "N";
			if((position%3) == 2)
				cardinalPosition += "S";
			if(position < 3)
				cardinalPosition += "W";
			if(position > 5)
				cardinalPosition += "E";
			if(position == 4)
				cardinalPosition = "C";

			ui->windowPositionLineEdit->setText(QString("Screen%1:%2").arg(screen+1).arg(cardinalPosition));
		}
	}

	void WindowPositionEdit::setPosition(QPoint position)
	{
		QPixmap pixel = QPixmap::grabWindow(QApplication::desktop()->winId(), position.x(), position.y(), 1, 1);
		//QColor pixelColor = pixel.toImage().pixel(0, 0);
		//mColorDialog->setCurrentColor(pixelColor);
		//onColorSelected();
		//on_screenPositionLineEdit_textChanged(QString());
	}

	void WindowPositionEdit::updateDialog(const QString &text)
	{
		QStringList superPosition = text.split(":");

		int screen = 1;
		if(superPosition.at(0).contains("Screen[\\d]+:"))
			//TODO: récuperer le numéro de screen
			screen = 1;
		else
			screen = 1;

		int position = 4;
		if(superPosition.at(1) == "NW")
			position = 0;
		if(superPosition.at(1) == "W")
			position = 1;
		if(superPosition.at(1) == "SW")
			position = 2;
		if(superPosition.at(1) == "N")
			position = 3;
		if(superPosition.at(1) == "S")
			position = 5;
		if(superPosition.at(1) == "NE")
			position = 6;
		if(superPosition.at(1) == "E")
			position = 7;
		if(superPosition.at(1) == "SE")
			position = 8;

		mScreenPositionDialog->windowPosition->setPosition(screen, position);
	}


	void WindowPositionEdit::on_windowPositionLineEdit_textChanged(const QString &text)
	{
		if(ui->windowPositionLineEdit->isCode())
			return;
		else
			updateDialog(text);
	}

	void WindowPositionEdit::on_windowPositionLineEdit_codeChanged(bool code)
	{
		QString oldText = ui->windowPositionLineEdit->text();

		if(code)
			ui->windowPositionLineEdit->setInputMask(QString());
		else
			ui->windowPositionLineEdit->setInputMask("Scree\\n9:>Aa");

		ui->windowPositionLineEdit->setText(oldText);
	}

	CodeLineEdit *WindowPositionEdit::codeLineEdit() const
	{
		return ui->windowPositionLineEdit;
	}
}
