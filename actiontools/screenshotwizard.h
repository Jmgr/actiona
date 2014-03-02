/*
    Actionaz
    Copyright (C) 2008-2014 Jonathan Mercier-Ganady

    Actionaz is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Actionaz is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

    Contact : jmgr@jmgr.info
*/

#ifndef SCREENSHOTWIZARD_H
#define SCREENSHOTWIZARD_H

#include "actiontools_global.h"

#include <QWizard>

namespace ActionTools
{
    class Script;

    class ACTIONTOOLSSHARED_EXPORT ScreenshotWizard : public QWizard
    {
        Q_OBJECT

    public:
        enum SaveTarget
        {
            ResourceSaveTarget,
            FileSaveTarget,
            ClipboardSaveTarget
        };

        ScreenshotWizard(ActionTools::Script *script, bool allowSaveToClipboard, QWidget *parent = 0);
        ~ScreenshotWizard();

        QString resourceOrFilename() const { return mResourceOrFilename; }
        SaveTarget saveTarget() const { return mSaveTarget; }

    private:
        QPixmap mCapturePixmap;
        QString mResourceOrFilename;
        SaveTarget mSaveTarget;

        friend class ScreenshotWizardPage;
        friend class SaveScreenshotWizardPage;
    };
}

#endif // SCREENSHOTWIZARD_H
