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

#include "actiontools_global.h"

#include <QWizardPage>

namespace Ui
{
    class SaveScreenshotWizardPage;
}

namespace ActionTools
{
    class ScreenshotWizard;
    class Script;

    class ACTIONTOOLSSHARED_EXPORT SaveScreenshotWizardPage : public QWizardPage
    {
        Q_OBJECT

    public:
        SaveScreenshotWizardPage(ActionTools::Script *script, bool allowSaveToClipboard, QWidget *parent = nullptr);
        ~SaveScreenshotWizardPage() override;

    protected:
        bool isComplete() const override;

    private slots:
        void on_saveToFileCommandLinkButton_clicked();
        void on_saveToResourceCommandLinkButton_clicked();
        void on_saveToClipboardCommandLinkButton_clicked();

    private:
        QPixmap capturePixmap() const;
        ScreenshotWizard *screenshotWizard();

        Ui::SaveScreenshotWizardPage *ui;
        ActionTools::Script *mScript;
    };
}

