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

#pragma once

#include <QDialog>

namespace Ui
{
	class NewActionDialog;
}

namespace ActionTools
{
	class ActionFactory;
}

class NewActionTreeView;
class QTreeWidgetItem;
class NewActionModel;
class NewActionProxyModel;

class NewActionDialog : public QDialog
{
	Q_OBJECT

public:
    NewActionDialog(ActionTools::ActionFactory *actionFactory,
                    NewActionModel *newActionModel,
                    QWidget *parent = nullptr);
	~NewActionDialog() override;

	QString selectedAction() const							{ return mSelectedAction; }
	int exec() override;

private slots:
    void on_newActionTreeView_doubleClicked(const QModelIndex &index);
    void on_filterLineEdit_textChanged(const QString &text);
    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
	void accept() override;

	Ui::NewActionDialog *ui;
	QString mSelectedAction;
	ActionTools::ActionFactory *mActionFactory;
    NewActionModel *mNewActionModel;
    NewActionProxyModel *mNewActionProxyModel;
};

