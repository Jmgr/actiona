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

#include "actiondefinition.h"
#include "findimageinstance.h"
#include "listparameterdefinition.h"
#include "fileparameterdefinition.h"
#include "imageparameterdefinition.h"
#include "variableparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "groupdefinition.h"
#include "windowparameterdefinition.h"
#include "booleanparameterdefinition.h"
#include "ifactionparameterdefinition.h"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class FindImageDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit FindImageDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("FindImageInstance::sources", FindImageInstance::sources);
            translateItems("FindImageInstance::methods", FindImageInstance::methods);

            auto &source = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("source"), tr("Source")});
            source.setTooltip(tr("The source of the image to search in"));
            source.setItems(FindImageInstance::sources);
            source.setDefaultValue(FindImageInstance::sources.second.at(FindImageInstance::ScreenshotSource));

            auto &windowGroup = addGroup();
            windowGroup.setMasterList(source);
            windowGroup.setMasterValues({FindImageInstance::sources.first.at(FindImageInstance::WindowSource)});

            auto &windowName = windowGroup.addParameter<ActionTools::WindowParameterDefinition>({QStringLiteral("windowName"), tr("Window name")});
            windowName.setTooltip(tr("The title of the window to search in, you can use wildcards like * (any number of characters) or ? (one character) here"));

            auto &relativePosition = windowGroup.addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("windowRelativePosition"), tr("Window relative position")});
            relativePosition.setTooltip(tr("The position is relative to the window\nIf this parameter is set to false (not checked) then the position is absolute"));

            auto &imageToSearchInGroup = addGroup();
            imageToSearchInGroup.setMasterList(source);
            imageToSearchInGroup.setMasterValues({FindImageInstance::sources.first.at(FindImageInstance::ImageSource)});

            auto &imageToSearchIn = imageToSearchInGroup.addParameter<ActionTools::ImageParameterDefinition>({QStringLiteral("imageToSearchIn"), tr("Image to search in")});
            imageToSearchIn.setTooltip(tr("The image to search in"));
            imageToSearchIn.setMode(ActionTools::FileEdit::FileOpen);
            imageToSearchIn.setCaption(tr("Choose an image file"));
            imageToSearchIn.setFilter(tr("Image files (*.bmp *.gif *.jpg *.jpeg *.mng *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm *.svg)\nAll files (*.*)"));

            auto &imageToFind = addParameter<ActionTools::ImageParameterDefinition>({QStringLiteral("imageToFind"), tr("Image to find")});
            imageToFind.setTooltip(tr("The image to search for"));
            imageToFind.setMode(ActionTools::FileEdit::FileOpen);
            imageToFind.setCaption(tr("Choose an image file"));
            imageToFind.setFilter(tr("Image files (*.bmp *.gif *.jpg *.jpeg *.mng *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm *.svg)\nAll files (*.*)"));

            auto &ifFound = addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifFound"), tr("If found")});
            ifFound.setTooltip(tr("What to do if the image is found"));
            ifFound.setAllowWait(true);

            auto &ifNotFound = addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifNotFound"), tr("If not found")});
            ifNotFound.setTooltip(tr("What to do if the image is not found"));
            ifNotFound.setAllowWait(true);

            auto &position = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("position"), tr("Position")});
            position.setTooltip(tr("The name of the variable where to store the coordinates of the center of the found image"));

            auto &method = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("method"), tr("Method")}, 1);
            method.setTooltip(tr("The matching method to use"));
            method.setItems(FindImageInstance::methods);
            method.setDefaultValue(FindImageInstance::methods.second.at(FindImageInstance::CorrelationCoefficientMethod));

            auto &confidenceMinimum = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("confidenceMinimum"), tr("Confidence minimum")}, 1);
            confidenceMinimum.setTooltip(tr("The minimum confidence percentage required to select a possible matching image"));
            confidenceMinimum.setMinimum(0);
            confidenceMinimum.setMaximum(100);
            confidenceMinimum.setDefaultValue(QStringLiteral("70"));

            auto &maximumMatches = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("maximumMatches"), tr("Maximum amount of images to find")}, 1);
            maximumMatches.setTooltip(tr("The maximum amount of images to find\nSetting this parameter higher will increase the search duration"));
            maximumMatches.setMinimum(1);
            maximumMatches.setMaximum(std::numeric_limits<int>::max());
            maximumMatches.setDefaultValue(QStringLiteral("1"));

            auto &downPyramidCount = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("downPyramidCount"), tr("Downsampling")}, 1);
            downPyramidCount.setTooltip(tr("The downsampling value to use\nDownsampling is used to accelerate the search when using large images"));
            downPyramidCount.setMinimum(1);
            downPyramidCount.setMaximum(std::numeric_limits<int>::max());
            downPyramidCount.setDefaultValue(QStringLiteral("1"));

            auto &searchExpansion = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("searchExpansion"), tr("Search expansion")}, 1);
            searchExpansion.setTooltip(tr("The number of pixels to shift when searching for another matching image"));
            searchExpansion.setMinimum(1);
            searchExpansion.setMaximum(std::numeric_limits<int>::max());
            searchExpansion.setDefaultValue(QStringLiteral("15"));

            auto &searchDelay = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("searchDelay"), tr("Delay between two searches when waiting")}, 1);
            searchDelay.setTooltip(tr("The delay between two searches"));
            searchDelay.setMinimum(0);
            searchDelay.setMaximum(std::numeric_limits<int>::max());
            searchDelay.setDefaultValue(QStringLiteral("100"));
            searchDelay.setSuffix(tr(" ms", "milliseconds"));

            auto &confidence = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("confidence"), tr("Confidence")}, 1);
            confidence.setTooltip(tr("The name of the variable where to store the confidence value found image"));

			addException(FindImageInstance::ErrorWhileSearchingException, tr("Error while searching"));
		}

		QString name() const override													{ return QObject::tr("Find image"); }
		QString id() const override														{ return QStringLiteral("ActionFindImage"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Finds an image on the screen, on a window or on another image"); }
        Tools::Version version() const override                                          { return Tools::Version(1, 1, 0); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new FindImageInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::System; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/findimage.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

        void updateAction(ActionTools::ActionInstance *actionInstance, const Tools::Version &version) const override;

	private:
        Q_DISABLE_COPY(FindImageDefinition)
    };
}

