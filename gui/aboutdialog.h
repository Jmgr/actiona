/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui
{
	class AboutDialog;
}

class QComboBox;

class AboutDialog : public QDialog
{
	Q_OBJECT
public:
	explicit AboutDialog(QWidget *parent = 0);
	~AboutDialog();

private slots:
	void animate();

private:
	Ui::AboutDialog *ui;
	QList<QComboBox *> mComboBoxes;

	Q_DISABLE_COPY(AboutDialog)
};

#endif // ABOUTDIALOG_H
