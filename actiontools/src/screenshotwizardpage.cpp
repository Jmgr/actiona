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

#include "actiontools/screenshotwizardpage.hpp"
#include "ui_screenshotwizardpage.h"
#include "actiontools/screenshooter.hpp"
#include "actiontools/screenshotwizard.hpp"
#include "actiontools/windowhandle.hpp"
#include "backend/windowing.hpp"

#include <QApplication>
#include <QKeyEvent>
#include <QDebug>

namespace ActionTools
{
    ScreenshotWizardPage::ScreenshotWizardPage(QWidget *parent)
      : QWizardPage(parent),
        ui(new Ui::ScreenshotWizardPage)
        
    {
        ui->setupUi(this);

        connect(ui->captureWindowPushButton, &ActionTools::ChooseWindowPushButton::windowChosen, this, &ScreenshotWizardPage::onWindowSearchEnded);

        ui->screenComboBox->addItem(tr("All screens"));

        auto screens = QGuiApplication::screens();
        for(int screenIndex = 0; screenIndex < screens.size(); ++screenIndex)
            ui->screenComboBox->addItem(tr("Screen %1").arg(screenIndex + 1));
    }

    ScreenshotWizardPage::~ScreenshotWizardPage()
    {
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
        mAreaChooser = Backend::Instance::windowing().createAreaChooser(this);
        connect(mAreaChooser, &Backend::AreaChooser::done, this, &ScreenshotWizardPage::onRectangleSelected);
        connect(mAreaChooser, &Backend::AreaChooser::canceled, this, []{ qDebug() << "test"; });// TODO
        connect(mAreaChooser, &Backend::AreaChooser::errorOccurred, this, [](const Backend::BackendError &e){ qDebug() << e.what(); });// TODO
        mAreaChooser->choose();

        //emit completeChanged();
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
        mAreaChooser->deleteLater();

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
