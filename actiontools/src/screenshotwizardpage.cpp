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
#include "actiontools/targetwindow.hpp"
#include "actiontools/screenshooter.hpp"
#include "actiontools/screenshotwizard.hpp"

#include <QApplication>
#include <QKeyEvent>

const int CAPTURE_DELAY = 200;

namespace ActionTools
{
    ScreenshotWizardPage::ScreenshotWizardPage(QWidget *parent)
      : QWizardPage(parent),
        ui(new Ui::ScreenshotWizardPage)

    {
        ui->setupUi(this);

        ui->captureWindowPushButton->setOpacityChangeOnWindows(false);

        connect(ui->captureWindowPushButton, &ActionTools::ChooseWindowPushButton::searchStarted, this, [this](){
            mShownWindows = ActionTools::AsyncScreenShooter::hideTopLevelWindows();
        });
        connect(ui->captureWindowPushButton, &ActionTools::ChooseWindowPushButton::searchEnded, this, &ScreenshotWizardPage::onWindowSearchEnded);

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
        {
            auto screenShooter = new ActionTools::AsyncScreenShooter(CAPTURE_DELAY, this);
            connect(screenShooter, &ActionTools::AsyncScreenShooter::finishedSingle, this, [this](const QPixmap &result){
                setCapturePixmap(result);
                ui->captureImageLabel->setPixmap(result);
                ui->captureImageLabel->updateGeometry();
                emit completeChanged();
                sender()->deleteLater();
            });
            screenShooter->captureAllScreens();
        }
        else
        {
            int screenIndex = ui->screenComboBox->currentIndex() - 1;

            auto screenShooter = new ActionTools::AsyncScreenShooter(CAPTURE_DELAY, this);
            connect(screenShooter, &ActionTools::AsyncScreenShooter::finishedSingle, this, [this](const QPixmap &result){
                setCapturePixmap(result);
                ui->captureImageLabel->setPixmap(result);
                ui->captureImageLabel->updateGeometry();
                emit completeChanged();
                sender()->deleteLater();
            });
            screenShooter->captureScreen(screenIndex);
        }
    }

    void ScreenshotWizardPage::on_captureScreenPartPushButton_clicked()
    {
        mDisableEscape = true;

        auto shownWindows = ActionTools::AsyncScreenShooter::hideTopLevelWindows();
#ifdef Q_OS_WIN
        QTimer::singleShot(CAPTURE_DELAY, this, [this, shownWindows](){
            // Wait for our windows to disappear, then capture before selection.
            // Closing the selector exposes the underlying application to mouseover
            // events, so capturing afterwards can include an unwanted hover state.
            const auto screens = ActionTools::ScreenShooter::captureScreens();
            auto targetWindow = new ActionTools::TargetWindow;

            connect(targetWindow, &ActionTools::TargetWindow::rectangleSelected, this,
                    [this, shownWindows, targetWindow, screens](const QRect &rect){
                targetWindow->deleteLater();

                if(!rect.isEmpty())
                {
                    QPixmap result;
                    for(const auto &screen : screens)
                    {
                        if(!screen.second.contains(rect.center()))
                            continue;

                        // Selection coordinates are logical; pixmap pixels may be
                        // scaled on high-DPI displays, including secondary screens.
                        const auto localRect = rect.translated(-screen.second.topLeft());
                        const auto ratio = screen.first.devicePixelRatio();
                        result = screen.first.copy(QRect(qRound(localRect.x() * ratio),
                                                         qRound(localRect.y() * ratio),
                                                         qRound(localRect.width() * ratio),
                                                         qRound(localRect.height() * ratio)));
                        break;
                    }

                    setCapturePixmap(result);
                    ui->captureImageLabel->setPixmap(result);
                    ui->captureImageLabel->updateGeometry();
                    emit completeChanged();
                }

                ActionTools::AsyncScreenShooter::showTopLevelWindows(shownWindows);
                mDisableEscape = false;
            });

            targetWindow->show();
        });
#else
        auto targetWindow = new ActionTools::TargetWindow;

        connect(targetWindow, &ActionTools::TargetWindow::rectangleSelected, this, [this, shownWindows, targetWindow](const QRect &rect){
            targetWindow->deleteLater();

            if(rect.isEmpty())
            {
                ActionTools::AsyncScreenShooter::showTopLevelWindows(shownWindows);
                mDisableEscape = false;
            }
            else
            {
                QTimer::singleShot(CAPTURE_DELAY, this, [this, rect, shownWindows](){
                    auto result = ActionTools::ScreenShooter::captureRect(rect);

                    ActionTools::AsyncScreenShooter::showTopLevelWindows(shownWindows);
                    mDisableEscape = false;

                    setCapturePixmap(result);
                    ui->captureImageLabel->setPixmap(result);
                    ui->captureImageLabel->updateGeometry();
                    emit completeChanged();
                });
            }
        });

        targetWindow->show();
#endif
    }

    void ScreenshotWizardPage::onWindowSearchEnded(const ActionTools::WindowHandle &handle)
    {
        QTimer::singleShot(200, this, [this, handle](){
            auto result = ActionTools::ScreenShooter::captureWindow(handle);

            ActionTools::AsyncScreenShooter::showTopLevelWindows(mShownWindows);

            setCapturePixmap(result);
            ui->captureImageLabel->setPixmap(result);
            ui->captureImageLabel->updateGeometry();
            emit completeChanged();
        });
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
