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

#include "actiontools_global.hpp"

#include <QWizardPage>

namespace Ui
{
    class ScreenshotWizardPage;
}

namespace ActionTools
{
    class WindowHandle;

    class ACTIONTOOLSSHARED_EXPORT ScreenshotWizardPage : public QWizardPage
    {
        Q_OBJECT

    public:
        explicit ScreenshotWizardPage(QWidget *parent = nullptr);
        ~ScreenshotWizardPage() override;

    protected:
        bool isComplete() const override;
        void keyPressEvent(QKeyEvent *event) override;

    private slots:
        void on_captureWholeScreenPushButton_clicked();
        void on_captureScreenPartPushButton_clicked();
        void onWindowSearchEnded(const ActionTools::WindowHandle &handle);

    private:
        void setCapturePixmap(const QPixmap &capturePixmap);
        QPixmap capturePixmap() const;

        Ui::ScreenshotWizardPage *ui;
        bool mDisableEscape{false};
        QList<QWindow*> mShownWindows;
    };
}

