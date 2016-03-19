/*
	Actiona
	Copyright (C) 2005-2016 Jonathan Mercier-Ganady

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

#ifndef NEWACTIONDIALOG_H
#define NEWACTIONDIALOG_H

#include <QDialog>

namespace Ui
{
	class NewActionDialog;
}

namespace ActionTools
{
	class ActionFactory;
}

class NewActionTreeWidget;
class QTreeWidgetItem;

class NewActionDialog : public QDialog
{
	Q_OBJECT

public:
	NewActionDialog(ActionTools::ActionFactory *actionFactory, QWidget *parent = 0);
	~NewActionDialog();

	NewActionTreeWidget *newActionTreeWidget() const;
	QString selectedAction() const							{ return mSelectedAction; }
	int exec();

private slots:
	void on_newActionTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
	void on_newActionTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
	void accept();

	Ui::NewActionDialog *ui;
	QString mSelectedAction;
	ActionTools::ActionFactory *mActionFactory;
};

#endif // NEWACTIONDIALOG_H
