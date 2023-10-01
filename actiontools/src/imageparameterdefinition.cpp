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

#include "actiontools/imageparameterdefinition.hpp"
#include "actiontools/screenshotpushbutton.hpp"
#include "actiontools/screenshotwizard.hpp"

namespace ActionTools
{
    ImageParameterDefinition::ImageParameterDefinition(const Name &name, QObject *parent)
      : FileParameterDefinition(name, parent),
        mScript(nullptr),
        mParent(nullptr)
    {
    }

    void ImageParameterDefinition::buildEditors(Script *script, QWidget *parent)
    {
        FileParameterDefinition::buildEditors(script, parent);

        mScript = script;// Is this really safe?
        mParent = parent;

        auto screenshotPushButton = new ScreenshotPushButton(parent);
        connect(screenshotPushButton, &ScreenshotPushButton::clicked, this, &ImageParameterDefinition::onCaptureClicked);

        addEditor(screenshotPushButton);
    }

    void ImageParameterDefinition::onCaptureClicked()
    {
        ScreenshotWizard screenshotWizard(mScript, false, mParent);

        screenshotWizard.setWindowFlag(Qt::WindowContextHelpButtonHint, false);

        if(screenshotWizard.exec() == QDialog::Accepted)
        {
            switch(screenshotWizard.saveTarget())
            {
            case ScreenshotWizard::ResourceSaveTarget:
				fileEdit()->setText(QStringLiteral("$") + screenshotWizard.resourceOrFilename());
                break;
            case ScreenshotWizard::FileSaveTarget:
                fileEdit()->setText(screenshotWizard.resourceOrFilename());
                break;
            default:
                return;
            }
        }
    }
}
