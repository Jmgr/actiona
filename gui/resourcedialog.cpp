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

#include "resourcedialog.h"
#include "ui_resourcedialog.h"
#include "filetypeguesser.h"
#include "numberformat.h"
#include "resourcenamedelegate.h"
#include "resourcetypedelegate.h"
#include "resourcesizeitem.h"
#include "script.h"
#include "actioninstance.h"

#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

#include <algorithm>

ResourceDialog::ResourceDialog(ActionTools::Script *script, QWidget *parent)
  : QDialog(parent),
    ui(new Ui::ResourceDialog),
    mScript(script)
{
    ui->setupUi(this);

    connect(ui->resourcesTableWidget, &ResourceTableWidget::filesDropped, this, &ResourceDialog::insertFiles);
    connect(ui->resourcesTableWidget, &ResourceTableWidget::removeSelection, this, &ResourceDialog::removeSelection);
    connect(ui->resourcesTableWidget, &ResourceTableWidget::itemSelectionChanged, this, &ResourceDialog::selectionChanged);

    ui->resourcesTableWidget->setItemDelegateForColumn(0, new ResourceNameDelegate(ui->resourcesTableWidget, this));
    ui->resourcesTableWidget->setItemDelegateForColumn(1, new ResourceTypeDelegate(this));
    ui->removeSelectedPushButton->setEnabled(false);
    ui->resourcesTableWidget->setSortingEnabled(false);

	QMap<QString, ActionTools::Resource> resources = script->resources();
	QMap<QString, ActionTools::Resource>::const_iterator resourceIt = resources.constBegin();
    while(resourceIt != resources.constEnd())
    {
        addResource(resourceIt.key(), resourceIt.value().data(), resourceIt.value().type());

        ++resourceIt;
    }

    ui->resourcesTableWidget->setSortingEnabled(true);
    ui->resourcesTableWidget->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);

    ui->clearPushButton->setEnabled(ui->resourcesTableWidget->rowCount() > 0);
    updateTotalSize();
}

ResourceDialog::~ResourceDialog()
{
    delete ui;
}

void ResourceDialog::setCurrentResource(const QString &resource)
{
    QList<QTableWidgetItem*> items = ui->resourcesTableWidget->findItems(resource, Qt::MatchFixedString | Qt::MatchCaseSensitive);
    if(!items.isEmpty())
    {
        QTableWidgetItem *selectedItem = items.first();

        ui->resourcesTableWidget->selectRow(selectedItem->row());
    }
}

void ResourceDialog::accept()
{
    mScript->clearResources();

    for(int row = 0; row < ui->resourcesTableWidget->rowCount(); ++row)
    {
        QTableWidgetItem *dataItem = ui->resourcesTableWidget->item(row, 0);
        QTableWidgetItem *typeItem = ui->resourcesTableWidget->item(row, 1);
        if(!dataItem || !typeItem)
            continue;

        QString name = dataItem->text();
        QByteArray data = dataItem->data(Qt::UserRole).toByteArray();
        ActionTools::Resource::Type type = static_cast<ActionTools::Resource::Type>(typeItem->data(Qt::UserRole).toInt());

        mScript->addResource(name, data, type);
    }

    QDialog::accept();
}

void ResourceDialog::insertFiles(const QStringList &filenames)
{
    bool wasSortingEnabled = ui->resourcesTableWidget->isSortingEnabled();
    if(wasSortingEnabled)
        ui->resourcesTableWidget->setSortingEnabled(false);

    for(const QString &filename: filenames)
    {
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this, tr("Insert files"), tr("Unable to insert file %1").arg(filename));

            continue;
        }

        QFileInfo fileInfo(file);
        QByteArray fileData = file.readAll();
        QString resourceName = ui->resourcesTableWidget->checkResourceName(ActionTools::ActionInstance::convertToVariableName(fileInfo.fileName()));
        ActionTools::Resource::Type resourceType = FiletypeGuesser::guessFiletype(fileData, fileInfo.fileName());

        addResource(resourceName, fileData, resourceType);
    }

    ui->resourcesTableWidget->setSortingEnabled(wasSortingEnabled);
    ui->resourcesTableWidget->resizeColumnToContents(0);
    ui->resourcesTableWidget->resizeColumnToContents(1);
    ui->clearPushButton->setEnabled(true);
    updateTotalSize();
}

void ResourceDialog::removeSelection()
{
    if(QMessageBox::question(this, tr("Remove selection"), tr("Remove the selected resources?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) != QMessageBox::Yes)
        return;

    QList<QTableWidgetSelectionRange> selectionRanges = ui->resourcesTableWidget->selectedRanges();
    std::sort(selectionRanges.begin(), selectionRanges.end(), [](const QTableWidgetSelectionRange &first, const QTableWidgetSelectionRange &second)
    {
        return (first.topRow() > second.topRow());
    });

    for(const QTableWidgetSelectionRange &selectionRange: selectionRanges)
    {
        for(int row = selectionRange.bottomRow(); row >= selectionRange.topRow(); --row)
            ui->resourcesTableWidget->removeRow(row);
    }

    ui->clearPushButton->setEnabled(ui->resourcesTableWidget->rowCount() > 0);
    updateTotalSize();
}

void ResourceDialog::selectionChanged()
{
    ui->removeSelectedPushButton->setEnabled(!ui->resourcesTableWidget->selectedItems().isEmpty());
}

void ResourceDialog::on_addFilesPushButton_clicked()
{
    insertFiles(QFileDialog::getOpenFileNames(this, tr("Choose the files to insert")));
}

void ResourceDialog::on_removeSelectedPushButton_clicked()
{
    removeSelection();
}

void ResourceDialog::on_clearPushButton_clicked()
{
    if(QMessageBox::question(this, tr("Clear resources"), tr("Remove all resources?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) != QMessageBox::Yes)
        return;

    int rowCount = ui->resourcesTableWidget->rowCount();
    for(; rowCount >= 0; --rowCount)
        ui->resourcesTableWidget->removeRow(0);

    ui->clearPushButton->setEnabled(false);
    updateTotalSize();
}

void ResourceDialog::updateTotalSize()
{
    quint64 totalSize = 0;

    for(int row = 0; row < ui->resourcesTableWidget->rowCount(); ++row)
    {
        QTableWidgetItem *item = ui->resourcesTableWidget->item(row, 2);
        if(item)
            totalSize += item->data(Qt::UserRole).toInt();
    }

    ui->totalSizeLabel->setText(ActionTools::NumberFormat::sizeString(totalSize));
}

void ResourceDialog::addResource(const QString &name, const QByteArray &data, ActionTools::Resource::Type type)
{
    int row = ui->resourcesTableWidget->rowCount();
    ui->resourcesTableWidget->insertRow(row);

    auto item = new QTableWidgetItem(name);
    item->setData(Qt::UserRole, data);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
    ui->resourcesTableWidget->setItem(row, 0, item);

    item = new QTableWidgetItem;
    item->setData(Qt::UserRole, type);
    item->setData(Qt::DisplayRole, ActionTools::Resource::typeNames[type]);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
    switch(type)
    {
    case ActionTools::Resource::BinaryType:
    case ActionTools::Resource::TypeCount:
		item->setIcon(QIcon(QStringLiteral(":/images/binary.png")));
        break;
    case ActionTools::Resource::TextType:
		item->setIcon(QIcon(QStringLiteral(":/images/text.png")));
        break;
    case ActionTools::Resource::ImageType:
		item->setIcon(QIcon(QStringLiteral(":/images/image.png")));
        break;
    }
    ui->resourcesTableWidget->setItem(row, 1, item);

    item = new ResourceSizeItem;
    item->setData(Qt::UserRole, data.size());
    item->setData(Qt::DisplayRole, ActionTools::NumberFormat::sizeString(data.size()));
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    ui->resourcesTableWidget->setItem(row, 2, item);
}
