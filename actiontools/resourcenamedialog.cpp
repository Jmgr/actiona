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

#include "resourcenamedialog.h"
#include "ui_resourcenamedialog.h"
#include "actioninstance.h"
#include "script.h"

#include <QRegExpValidator>
#include <QPushButton>

namespace ActionTools
{
    ResourceNameDialog::ResourceNameDialog(ActionTools::Script *script, QWidget *parent)
      : QDialog(parent),
        ui(new Ui::ResourceNameDialog),
        mScript(script)
    {
        ui->setupUi(this);

        connect(ui->resourceNameLineEdit, &QLineEdit::textChanged, this, &ResourceNameDialog::onTextChanged);

        ui->resourceNameLineEdit->setValidator(new QRegExpValidator(ActionTools::ActionInstance::NameRegExp, ui->resourceNameLineEdit));
        ui->resourceNameLineEdit->setFocus();

        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }

    ResourceNameDialog::~ResourceNameDialog()
    {
        delete ui;
    }

    QString ResourceNameDialog::resourceName() const
    {
        return ui->resourceNameLineEdit->text();
    }

    void ResourceNameDialog::accept()
    {
        if(!acceptable())
            return;

        QDialog::accept();
    }

    void ResourceNameDialog::onTextChanged(const QString &text)
    {
        Q_UNUSED(text)

        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(acceptable());
    }

    bool ResourceNameDialog::acceptable() const
    {
        QString resultResourceName = resourceName();

        return (!resultResourceName.isEmpty() && !mScript->hasResource(resourceName()));
    }
}
