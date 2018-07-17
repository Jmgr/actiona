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

#include "newactiondialog.h"
#include "ui_newactiondialog.h"
#include "actionfactory.h"
#include "actiondefinition.h"
#include "newactionmodel.h"
#include "newactionproxymodel.h"

#include <QPushButton>

NewActionDialog::NewActionDialog(ActionTools::ActionFactory *actionFactory,
                                 NewActionModel *newActionModel,
                                 QWidget *parent)
	: QDialog(parent),
	ui(new Ui::NewActionDialog),
    mActionFactory(actionFactory),
    mNewActionModel(newActionModel),
    mNewActionProxyModel(new NewActionProxyModel(this))
{
	ui->setupUi(this);

    mNewActionProxyModel->setDynamicSortFilter(false);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    {
        QItemSelectionModel *model = ui->newActionTreeView->selectionModel();
        mNewActionProxyModel->setSourceModel(mNewActionModel);
        ui->newActionTreeView->setModel(mNewActionProxyModel);
        delete model;
    }

    QItemSelectionModel *selectionModel = ui->newActionTreeView->selectionModel();

    connect(selectionModel, &QItemSelectionModel::currentChanged, this, &NewActionDialog::onCurrentChanged);

    ui->newActionTreeView->expandAll();
}

NewActionDialog::~NewActionDialog()
{
	delete ui;
}

int NewActionDialog::exec()
{
    QStandardItem *selectedItem = nullptr;

    for(int rowIndex = 0; rowIndex < mNewActionModel->rowCount(); ++rowIndex)
    {
        QStandardItem *currentItem = mNewActionModel->item(rowIndex);
        if(!currentItem)
            continue;

        if(currentItem->hasChildren())
        {
            selectedItem = currentItem->child(0);
            break;
        }
    }

    if(selectedItem)
        ui->newActionTreeView->setCurrentIndex(mNewActionProxyModel->mapFromSource(selectedItem->index()));

	return QDialog::exec();
}

void NewActionDialog::on_newActionTreeView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    accept();
}

void NewActionDialog::on_filterLineEdit_textChanged(const QString &text)
{
    mNewActionProxyModel->setFilterString(text);
    ui->newActionTreeView->expandAll();
}

void NewActionDialog::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    auto index = mNewActionProxyModel->mapToSource(current);
    const QString &actionId = index.data(NewActionModel::ActionIdRole).toString();
    ActionTools::ActionDefinition *actionDefinition = mActionFactory->actionDefinition(actionId);
    if(!actionDefinition)
        return;

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

    ui->actionDescription->setText(actionDefinition->description());
    ui->versionLabel->setText(actionDefinition->version().toString());

    QString status;
    switch(actionDefinition->status())
    {
    case ActionTools::Alpha:
        status = tr("Alpha");
        break;
    case ActionTools::Beta:
        status = tr("Beta");
        break;
    case ActionTools::Testing:
        status = tr("Testing");
        break;
    case ActionTools::Stable:
        status = tr("Stable");
        break;
    }
    ui->statusLabel->setText(status);

    QString official;
    if(actionDefinition->flags() & ActionTools::Official)
        official = tr("Yes");
    else
        official = tr("No");
    ui->officialLabel->setText(official);
}

void NewActionDialog::accept()
{
    auto selectedIndexes = ui->newActionTreeView->selectionModel()->selectedRows(0);
    if(selectedIndexes.isEmpty())
        return;

    auto firstIndex = mNewActionProxyModel->mapToSource(selectedIndexes.first());

    QString selectedAction = firstIndex.data(NewActionModel::ActionIdRole).toString();

    if(selectedAction.isEmpty())
        return;

    mSelectedAction = selectedAction;

	QDialog::accept();
}
