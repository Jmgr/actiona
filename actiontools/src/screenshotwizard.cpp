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

#include "actiontools/screenshotwizard.hpp"
#include "actiontools/screenshotwizardpage.hpp"
#include "actiontools/savescreenshotwizardpage.hpp"

#include <QIcon>

namespace ActionTools
{
    ScreenshotWizard::ScreenshotWizard(ActionTools::Script *script, bool allowSaveToClipboard, QWidget *parent)
      : QWizard(parent),
        mSaveTarget(ResourceSaveTarget)
    {
        setWindowTitle(tr("Screenshot wizard"));
		setWindowIcon(QIcon(QStringLiteral(":/images/capture.png")));

        addPage(new ScreenshotWizardPage(this));
        addPage(new SaveScreenshotWizardPage(script, allowSaveToClipboard, this));
    }

    ScreenshotWizard::~ScreenshotWizard() = default;
}
