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

#pragma once

#include <QDialog>
#include <QDate>

class QVersionNumber;

namespace Ui
{
	class ChangelogDialog;
}

class ChangelogDialog : public QDialog
{
	Q_OBJECT

public:
    enum ChangelogAction
	{
        NoAction,
		DownloadAndInstall,
		DownloadOnly
	};

	explicit ChangelogDialog(QWidget *parent = nullptr);
	~ChangelogDialog() override;

	void setVersion(const QVersionNumber &version);
	void setReleaseDate(const QDate &releaseDate);
	void setType(const QString &type);
	void setChangelog(const QString &changelog);

	ChangelogAction changelogAction() const					{ return mChangelogAction; }

private slots:
	void on_downloadAndInstallButton_clicked();
	void on_downloadButton_clicked();

private:
    Ui::ChangelogDialog *ui;
    ChangelogAction mChangelogAction{NoAction};
};

