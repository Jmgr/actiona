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

#include "coloredit.h"
#include "ui_coloredit.h"

#include <QColorDialog>
#include <QDesktopWidget>
#include <QRegExpValidator>
#include <QApplication>
#include <QScreen>

namespace ActionTools
{
	ColorEdit::ColorEdit(QWidget *parent)
		: QWidget(parent),
		ui(new Ui::ColorEdit),
        mColorDialog(new QColorDialog(this)),
		mValidator(new QRegExpValidator(QRegExp(QStringLiteral("^\\d\\d{0,2}:\\d\\d{0,2}:\\d\\d{0,2}$"), Qt::CaseSensitive, QRegExp::RegExp2), this))
	{
		ui->setupUi(this);

        ui->colorLineEdit->setValidator(mValidator);
	}

	ColorEdit::~ColorEdit()
	{
		delete ui;
	}

	void ColorEdit::setText(const QString &text)
	{
		ui->colorLineEdit->setText(text);
	}

	QString ColorEdit::text() const
	{
		return ui->colorLineEdit->text();
	}

	CodeLineEdit *ColorEdit::codeLineEdit() const
	{
		return ui->colorLineEdit;
	}

	bool ColorEdit::isCode() const
	{
		return ui->colorLineEdit->isCode();
	}

	void ColorEdit::setCode(bool code)
	{
		return ui->colorLineEdit->setCode(code);
	}

	void ColorEdit::setFromSubParameter(const SubParameter &subParameter)
	{
		ui->colorLineEdit->setFromSubParameter(subParameter);
	}

	void ColorEdit::openEditor(int line, int column)
	{
		ui->colorLineEdit->openEditor(line, column);
	}
	
	void ColorEdit::setCompletionModel(QAbstractItemModel *completionModel)
	{
        codeLineEdit()->setCompletionModel(completionModel);
    }

    void ColorEdit::setParameterContainer(const ParameterContainer *parameterContainer)
    {
        codeLineEdit()->setParameterContainer(parameterContainer);
    }

    QSet<QString> ColorEdit::findVariables() const
    {
        return codeLineEdit()->findVariables();
    }
	
	void ColorEdit::setChooseByPositionButtonVisible(bool visible)
	{
		ui->chooseByPosition->setVisible(visible);
	}
	
    void ColorEdit::setPosition(QPointF position)
	{
        QPixmap pixel = QGuiApplication::primaryScreen()->grabWindow(0, position.x(), position.y(), 1, 1);
		QColor pixelColor = pixel.toImage().pixel(0, 0);
		mColorDialog->setCurrentColor(pixelColor);
		onColorSelected();
		on_colorLineEdit_textChanged(QString());
	}

    void ColorEdit::on_chooseByPosition_positionChosen(QPointF position)
	{
		setPosition(position);
		
		emit positionChosen(position);
	}

	void ColorEdit::on_choose_clicked()
	{
		mColorDialog->setCurrentColor(currentColor());
		mColorDialog->open(this, SLOT(onColorSelected()));
	}

	void ColorEdit::on_colorLineEdit_textChanged(const QString &text)
	{
		Q_UNUSED(text)

		if(ui->colorLineEdit->isCode())
			return;

		QPalette palette = ui->colorLineEdit->palette();
		QColor color = currentColor();

		palette.setColor(QPalette::Base, color);

		QColor inverseColor(255 - color.red(),
			255 - color.green(),
			255 - color.blue());

		palette.setColor(QPalette::Text, inverseColor);
		ui->colorLineEdit->setPalette(palette);
	}

	void ColorEdit::on_colorLineEdit_codeChanged(bool code)
	{
		if(code)
		{
			QString oldText = ui->colorLineEdit->text();
            ui->colorLineEdit->setValidator(nullptr);
			ui->colorLineEdit->setText(oldText);
			ui->colorLineEdit->setPalette(palette());
		}
		else
		{
			QColor oldColor = currentColor();
            ui->colorLineEdit->setValidator(mValidator);
			mColorDialog->setCurrentColor(oldColor);
			onColorSelected();
		}
	}

	void ColorEdit::onColorSelected()
	{
		ui->colorLineEdit->setText(QStringLiteral("%1:%2:%3")
			.arg(mColorDialog->currentColor().red())
			.arg(mColorDialog->currentColor().green())
			.arg(mColorDialog->currentColor().blue()));
	}

	QColor ColorEdit::currentColor() const
	{
		QStringList values = ui->colorLineEdit->text().split(QLatin1Char(':'));

		if(values.size() != 3)
			return QColor(0, 0, 0);

		bool redOk, greenOk, blueOk;
		int r = values.at(0).toInt(&redOk);
		int g = values.at(1).toInt(&greenOk);
		int b = values.at(2).toInt(&blueOk);

		if(!redOk || !greenOk || !blueOk)
			return QColor(0, 0, 0);

		if(r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
			return QColor(0, 0, 0);

		return {r, g , b};
	}
}
