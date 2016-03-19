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

#include "newactiondialog.h"
#include "ui_newactiondialog.h"
#include "actionfactory.h"
#include "actiondefinition.h"

NewActionDialog::NewActionDialog(ActionTools::ActionFactory *actionFactory, QWidget *parent)
	: QDialog(parent),
	ui(new Ui::NewActionDialog),
	mActionFactory(actionFactory)
{
	ui->setupUi(this);
}

NewActionDialog::~NewActionDialog()
{
	delete ui;
}

NewActionTreeWidget *NewActionDialog::newActionTreeWidget() const
{
	return ui->newActionTreeWidget;
}

int NewActionDialog::exec()
{
	QTreeWidgetItem *chosenItem = 0;
	for(int topLevelItemIndex = 0; topLevelItemIndex < ui->newActionTreeWidget->topLevelItemCount(); ++topLevelItemIndex)
	{
		QTreeWidgetItem *topLevelItem = ui->newActionTreeWidget->topLevelItem(topLevelItemIndex);
		if(!topLevelItem)
			continue;

		if(topLevelItem->childCount() > 0)
		{
			chosenItem = topLevelItem->child(0);
			break;
		}
	}

	if(chosenItem)
		ui->newActionTreeWidget->setCurrentItem(chosenItem);

	return QDialog::exec();
}

void NewActionDialog::on_newActionTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	Q_UNUSED(item)
	Q_UNUSED(column)

	accept();
}

void NewActionDialog::on_newActionTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	Q_UNUSED(previous)

	const QString &actionId = current->data(0, NewActionTreeWidget::ActionIdRole).toString();
	ActionTools::ActionDefinition *actionDefinition = mActionFactory->actionDefinition(actionId);
	if(!actionDefinition)
		return;

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
	QTreeWidgetItem *currentItem = ui->newActionTreeWidget->currentItem();
	if(!currentItem)
		return;

	mSelectedAction = currentItem->data(0, NewActionTreeWidget::ActionIdRole).toString();

	QDialog::accept();
}
