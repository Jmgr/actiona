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

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "global.h"

#include <QTimer>
#include <QDesktopServices>
#include <QUrl>

AboutDialog::AboutDialog(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::AboutDialog)
{
	ui->setupUi(this);

#ifdef Q_WS_WIN
	QString os = tr("Windows");
#endif
#ifdef Q_WS_X11
	QString os = tr("GNU/Linux");
#endif
#ifdef Q_WS_MAC
	QString os = tr("Mac");
#endif

	QString message = tr("<img src=':/icons/logo.png'><h2>Actionaz %1</h2>").arg(Global::ACTIONAZ_VERSION.toString());
	message += " 2008-2011 Jonathan \"Jmgr\" Mercier-Ganady <a href='mailto:jmgr@jmgr.info'>jmgr@jmgr.info</a><br/><br/>";
	message += tr("<i>Emulates clics, key presses and other actions</i><br/><br/>Using Qt %1 (runtime %2) under %3 (%4 bit)</center><br/>")
			   .arg(QT_VERSION_STR)
			   .arg(qVersion())
			   .arg(os)
			   .arg(QSysInfo::WordSize);
	ui->titleLabel->setText(message);

	mComboBoxes.append(ui->act3Prog);
	mComboBoxes.append(ui->act3Ideas);
	mComboBoxes.append(ui->act3Donators);
	mComboBoxes.append(ui->act3Art);
	mComboBoxes.append(ui->act3Pack);
	mComboBoxes.append(ui->act3Documentation);
	mComboBoxes.append(ui->act2Prog);
	mComboBoxes.append(ui->act2Ideas);
	mComboBoxes.append(ui->act2Donators);
	mComboBoxes.append(ui->act2Art);

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
	timer->start(1500);
}

AboutDialog::~AboutDialog()
{
	delete ui;
}

void AboutDialog::animate()
{
	foreach(QComboBox *comboBox, mComboBoxes)
	{
		int count = comboBox->count();
		int current = comboBox->currentIndex();
		if(current < count - 1)
			comboBox->setCurrentIndex(current + 1);
		else
			comboBox->setCurrentIndex(0);
	}
}
