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

#include "screenshotwizardpage.h"
#include "ui_screenshotwizardpage.h"
#include "targetwindow.h"
#include "screenshooter.h"
#include "screenshotwizard.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>

namespace ActionTools
{
    ScreenshotWizardPage::ScreenshotWizardPage(QWidget *parent)
      : QWizardPage(parent),
        ui(new Ui::ScreenshotWizardPage)
        
    {
        ui->setupUi(this);

        connect(ui->captureWindowPushButton, &ActionTools::ChooseWindowPushButton::searchEnded, this, &ScreenshotWizardPage::onWindowSearchEnded);

        QDesktopWidget *desktopWidget = QApplication::desktop();

        ui->screenComboBox->addItem(tr("All screens"));

        for(int screenIndex = 0; screenIndex < desktopWidget->screenCount(); ++screenIndex)
            ui->screenComboBox->addItem(tr("Screen %1").arg(screenIndex + 1));
    }

    ScreenshotWizardPage::~ScreenshotWizardPage()
    {
        delete mTargetWindow;
        delete ui;
    }

    bool ScreenshotWizardPage::isComplete() const
    {
        return !capturePixmap().isNull();
    }

    void ScreenshotWizardPage::keyPressEvent(QKeyEvent *event)
    {
        if(event->key() == Qt::Key_Escape && mDisableEscape)
            return;

        QWizardPage::keyPressEvent(event);
    }

    void ScreenshotWizardPage::on_captureWholeScreenPushButton_clicked()
    {
        if(ui->screenComboBox->currentIndex() == 0)//All screens
            setCapturePixmap(ActionTools::ScreenShooter::captureAllScreens());
        else
        {
            int screenIndex = ui->screenComboBox->currentIndex() - 1;

            setCapturePixmap(ActionTools::ScreenShooter::captureScreen(screenIndex));
        }

        ui->captureImageLabel->setPixmap(capturePixmap());

        emit completeChanged();
    }

    void ScreenshotWizardPage::on_captureScreenPartPushButton_clicked()
    {
        mDisableEscape = true;
        if(mTargetWindow)
            delete mTargetWindow;
        mTargetWindow = new ActionTools::TargetWindow;
        connect(mTargetWindow, &ActionTools::TargetWindow::rectangleSelected, this, &ScreenshotWizardPage::onRectangleSelected);
        mTargetWindow->show();

        emit completeChanged();
    }

    void ScreenshotWizardPage::onWindowSearchEnded(const ActionTools::WindowHandle &handle)
    {
        setCapturePixmap(ActionTools::ScreenShooter::captureWindow(handle));

        ui->captureImageLabel->setPixmap(capturePixmap());

        emit completeChanged();
    }

    void ScreenshotWizardPage::onRectangleSelected(QRect rect)
    {
        mDisableEscape = false;
        mTargetWindow->deleteLater();
        mTargetWindow = nullptr;

        if(rect != QRect())
        {
            setCapturePixmap(ActionTools::ScreenShooter::captureRect(rect));

            ui->captureImageLabel->setPixmap(capturePixmap());

            emit completeChanged();
        }
    }

    void ScreenshotWizardPage::setCapturePixmap(const QPixmap &capturePixmap)
    {
        qobject_cast<ScreenshotWizard *>(wizard())->mCapturePixmap = capturePixmap;
    }

    QPixmap ScreenshotWizardPage::capturePixmap() const
    {
        return qobject_cast<ScreenshotWizard *>(wizard())->mCapturePixmap;
    }
}
