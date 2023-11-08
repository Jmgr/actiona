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

#include "actiontools/resource.hpp"

namespace Ui
{
    class ResourceDialog;
}

namespace ActionTools
{
    class Script;
}

class QTableWidgetItem;

class ResourceDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ResourceDialog(ActionTools::Script *script, QWidget *parent = nullptr);
    ~ResourceDialog() override;

    void setCurrentResource(const QString &resource);

public slots:
    void accept() override;

private slots:
    void insertFiles(const QStringList &filenames);
    void removeSelection();
    void selectionChanged();
    void on_addFilesPushButton_clicked();
    void on_removeSelectedPushButton_clicked();
    void on_clearPushButton_clicked();

private:
    void updateTotalSize();
    void addResource(const QString &name, const QByteArray &data, ActionTools::Resource::Type type);

private:
    Ui::ResourceDialog *ui;
    ActionTools::Script *mScript;
};

