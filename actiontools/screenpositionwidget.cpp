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

#include "screenpositionwidget.h"

#include <QButtonGroup>
#include <QRadioButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QApplication>
#include <QDesktopWidget>

namespace ActionTools
{
	QString ScreenPositionWidget::iconNames[3][3] =
	{
		{QStringLiteral("lu"),	QStringLiteral("u"),	QStringLiteral("ru")},
		{QStringLiteral("l"),	QStringLiteral("c"),	QStringLiteral("r")},
		{QStringLiteral("ld"),	QStringLiteral("d"),	QStringLiteral("rd")}
	};

	ScreenPositionWidget::ScreenPositionWidget(QWidget *parent)
		: QWidget(parent),
		mButtonGroup(new QButtonGroup(this))
	{
		auto mainLayout = new QHBoxLayout();

		int screenCount = QApplication::desktop()->numScreens();
		for(int screen=0;screen<screenCount;++screen)
		{
			QGroupBox *screenPositionGroupBox = new QGroupBox(tr("Screen %1").arg(screen+1));
			auto gridLayout = new QGridLayout();
			gridLayout->setMargin(0);
			gridLayout->setSpacing(0);

			int i = 0;
			for(int column=0;column<3;++column)
			{
				for(int row=0;row<3;++row,++i)
				{
					auto radioButton = new QRadioButton(this);
					radioButton->setIconSize(QSize(40, 40));
					radioButton->setIcon(QIcon(QStringLiteral(":/images/monitor_%1.png").arg(iconNames[row][column])));

					mButtonGroup->addButton(radioButton, screen * 9 + i);
					mRadioButtons.append(radioButton);

					gridLayout->addWidget(radioButton, row, column, Qt::AlignCenter);
				}
			}

			screenPositionGroupBox->setLayout(gridLayout);

			mainLayout->addWidget(screenPositionGroupBox);
		}

		setLayout(mainLayout);
	}


	void ScreenPositionWidget::setPosition(int screen, int position)
	{
		if(screen < 0 || screen >= QApplication::desktop()->numScreens())
			screen = 0;
		if(position < 0 || position >= 9)
			position = 0;

		QRadioButton *button = mRadioButtons.at(screen * 9 + position);
		if(button)
			button->setChecked(true);
	}

	int ScreenPositionWidget::screen() const
	{
		return (mButtonGroup->checkedId() / 9);
	}

	int ScreenPositionWidget::position() const
	{
		return (mButtonGroup->checkedId() % 9);
	}
}
