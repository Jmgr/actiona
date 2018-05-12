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

#include "savescreenshotwizardpage.h"
#include "ui_savescreenshotwizardpage.h"
#include "screenshotwizard.h"
#include "script.h"
#include "resourcenamedialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QBuffer>
#include <QClipboard>
#include <QApplication>

namespace ActionTools
{
    SaveScreenshotWizardPage::SaveScreenshotWizardPage(ActionTools::Script *script, bool allowSaveToClipboard, QWidget *parent)
      : QWizardPage(parent),
        ui(new Ui::SaveScreenshotWizardPage),
        mScript(script)
    {
        ui->setupUi(this);

        if(!allowSaveToClipboard)
            ui->saveToClipboardCommandLinkButton->setVisible(false);
    }

    SaveScreenshotWizardPage::~SaveScreenshotWizardPage()
    {
        delete ui;
    }

    bool SaveScreenshotWizardPage::isComplete() const
    {
        return false;
    }

    void SaveScreenshotWizardPage::on_saveToFileCommandLinkButton_clicked()
    {
        QString filename = QFileDialog::getSaveFileName(this, tr("Select the file where to save the screenshot"),
                                                        QString(), tr("Images (*.png *.jpg *.jpeg *.bmp *.ppm *.xbm *.xpm);;All files (*.*)"));
        if(filename.isEmpty())
            return;

        QFileInfo fileInfo(filename);
        if(fileInfo.completeSuffix() == QString())
			filename += QStringLiteral(".png");

        if(!capturePixmap().save(filename))
        {
            QMessageBox::warning(this, tr("Failed to save the screenshot"), tr("Failed to save the screenshot."));

            return;
        }

        screenshotWizard()->mSaveTarget = ScreenshotWizard::FileSaveTarget;
        screenshotWizard()->mResourceOrFilename = filename;

        wizard()->accept();
    }

    void SaveScreenshotWizardPage::on_saveToResourceCommandLinkButton_clicked()
    {
        ResourceNameDialog resourceNameDialog(mScript, this);

        resourceNameDialog.setWindowFlags(resourceNameDialog.windowFlags() | Qt::WindowContextHelpButtonHint);

        if(resourceNameDialog.exec() != QDialog::Accepted)
            return;

        QBuffer imageData;
        imageData.open(QIODevice::WriteOnly);
        capturePixmap().save(&imageData, "PNG");

        mScript->addResource(resourceNameDialog.resourceName(), imageData.buffer(), ActionTools::Resource::ImageType);

        screenshotWizard()->mSaveTarget = ScreenshotWizard::ResourceSaveTarget;
        screenshotWizard()->mResourceOrFilename = resourceNameDialog.resourceName();

        wizard()->accept();
    }

    void SaveScreenshotWizardPage::on_saveToClipboardCommandLinkButton_clicked()
    {
        QClipboard *clipboard = QApplication::clipboard();

        clipboard->setImage(capturePixmap().toImage());

        screenshotWizard()->mSaveTarget = ScreenshotWizard::ClipboardSaveTarget;

        wizard()->accept();
    }

    QPixmap SaveScreenshotWizardPage::capturePixmap() const
    {
        return qobject_cast<ScreenshotWizard *>(wizard())->mCapturePixmap;
    }

    ScreenshotWizard *SaveScreenshotWizardPage::screenshotWizard()
    {
        return qobject_cast<ScreenshotWizard *>(wizard());
    }
}
