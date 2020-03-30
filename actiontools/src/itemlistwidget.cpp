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

#include "actiontools/itemlistwidget.hpp"
#include "ui_itemlistwidget.h"
#include "actiontools/itemlistmodel.hpp"

namespace ActionTools
{
	ItemListWidget::ItemListWidget(QWidget *parent)
	  : QWidget(parent),
		ui(new Ui::ItemListWidget),
        mModel(new ItemListModel(this))
	{
		ui->setupUi(this);

		QAbstractItemModel *oldModel = ui->itemsListView->model();
		ui->itemsListView->setModel(mModel);
		delete oldModel;

        connect(ui->itemsListView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ItemListWidget::selectionChanged);

		selectionChanged(QItemSelection(), QItemSelection());
	}

	ItemListWidget::~ItemListWidget()
	{
        delete mModel;
		delete ui;
	}

	void ItemListWidget::setItems(const QStringList &items)
	{
		mModel->setRowCount(0);

		appendItems(items);
	}

	void ItemListWidget::appendItems(const QStringList &items)
	{
		if(items.isEmpty())
			return;

		int beginRow = mModel->rowCount();

		mModel->insertRows(beginRow, items.size());

		for(int row = beginRow, index = 0; index < items.size(); ++index, ++row)
			mModel->setData(mModel->index(row, 0), items[index]);
	}

	QStringList ItemListWidget::items() const
	{
		QStringList back;

		for(int row = 0; row < mModel->rowCount(); ++row)
			back.append(mModel->data(mModel->index(row, 0)).toString());

		return back;
	}

	void ItemListWidget::on_addPushButton_clicked()
	{
		auto newItem = new QStandardItem();
		mModel->appendRow(newItem);

		ui->itemsListView->edit(newItem->index());
		ui->itemsListView->setCurrentIndex(newItem->index());
	}

	void ItemListWidget::on_removePushButton_clicked()
	{
		const QModelIndexList &selection = ui->itemsListView->selectionModel()->selectedRows();
		if(selection.isEmpty())
			return;

		int currentRow = selection.first().row();

		mModel->removeRow(currentRow);

		if(mModel->rowCount() == 0)
			return;

		if(!mModel->hasIndex(currentRow, 0))
			--currentRow;

		ui->itemsListView->setCurrentIndex(mModel->index(currentRow, 0));
	}

	void ItemListWidget::on_moveUpPushButton_clicked()
	{
		move(true);
	}

	void ItemListWidget::on_moveDownPushButton_clicked()
	{
		move(false);
	}

	void ItemListWidget::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
	{
		Q_UNUSED(deselected)

		bool hasSelection = (!selected.isEmpty());

		ui->removePushButton->setEnabled(hasSelection);

		bool canMoveUp = false;
		bool canMoveDown = false;

		if(hasSelection)
		{
			int selectedRow = selected.first().top();

			canMoveUp = (selectedRow > 0);
			canMoveDown= (selectedRow < mModel->rowCount() - 1);
		}
		else
		{
			canMoveUp = false;
			canMoveDown = false;
		}

		ui->moveUpPushButton->setEnabled(canMoveUp);
		ui->moveDownPushButton->setEnabled(canMoveDown);
	}

	void ItemListWidget::on_itemsListView_removeCurrentItem()
	{
		on_removePushButton_clicked();
	}

	void ItemListWidget::on_itemsListView_moveCurrentItem(bool up)
	{
		move(up);
	}

	void ItemListWidget::move(bool up)
	{
		const QModelIndexList &selection = ui->itemsListView->selectionModel()->selectedRows();
		if(selection.isEmpty())
			return;

		int selectedRow = selection.first().row();

		if(up && selectedRow == 0)
			return;
		if(!up && selectedRow == mModel->rowCount() - 1)
			return;

		int direction = (up ? -1 : 1);

		QStandardItem *movedItem = mModel->takeRow(selectedRow).first();
		mModel->insertRow(selectedRow + direction, movedItem);

		ui->itemsListView->setCurrentIndex(movedItem->index());
	}
}
