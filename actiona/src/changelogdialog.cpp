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

#include "changelogdialog.hpp"
#include "ui_changelogdialog.h"

#include <QVersionNumber>

ChangelogDialog::ChangelogDialog(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::ChangelogDialog)
	
{
	ui->setupUi(this);
}

ChangelogDialog::~ChangelogDialog()
{
	delete ui;
}

void ChangelogDialog::setVersion(const QVersionNumber &version)
{
	ui->versionNumber->setText(version.toString());
}

void ChangelogDialog::setReleaseDate(const QDate &releaseDate)
{
    QLocale locale;
    ui->releaseDate->setText(locale.toString(releaseDate, QLocale::LongFormat));
}

void ChangelogDialog::setType(const QString &type)
{
	ui->type->setText(type);
}

void ChangelogDialog::setChangelog(const QString &changelog)
{
	ui->changelog->setText(changelog);
}

void ChangelogDialog::on_downloadAndInstallButton_clicked()
{
	mChangelogAction = DownloadAndInstall;

	accept();
}

void ChangelogDialog::on_downloadButton_clicked()
{
	mChangelogAction = DownloadOnly;

	accept();
}
