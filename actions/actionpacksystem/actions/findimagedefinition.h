/*
	Actiona
	Copyright (C) 2008-2015 Jonathan Mercier-Ganady

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

#ifndef FINDIMAGEDEFINITION_H
#define FINDIMAGEDEFINITION_H

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
	class FindImageDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit FindImageDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("FindImageInstance::sources", FindImageInstance::sources);
            translateItems("FindImageInstance::methods", FindImageInstance::methods);

			ActionTools::ListParameterDefinition *source = new ActionTools::ListParameterDefinition(ActionTools::Name("source", tr("Source")), this);
			source->setTooltip(tr("The source of the image to search in"));
			source->setItems(FindImageInstance::sources);
			source->setDefaultValue(FindImageInstance::sources.second.at(FindImageInstance::ScreenshotSource));
			addElement(source);

            ActionTools::GroupDefinition *windowGroup = new ActionTools::GroupDefinition(this);
            windowGroup->setMasterList(source);
            windowGroup->setMasterValues(QStringList() << FindImageInstance::sources.first.at(FindImageInstance::WindowSource));

			ActionTools::WindowParameterDefinition *windowName = new ActionTools::WindowParameterDefinition(ActionTools::Name("windowName", tr("Window name")), this);
			windowName->setTooltip(tr("The title of the window to search in, you can use wildcards like * (any number of characters) or ? (one character) here"));
            windowGroup->addMember(windowName);

            ActionTools::BooleanParameterDefinition *relativePosition = new ActionTools::BooleanParameterDefinition(ActionTools::Name("windowRelativePosition", tr("Window relative position")), this);
            relativePosition->setTooltip(tr("The position is relative to the window\nIf this parameter is set to false (not checked) then the position is absolute"));
            windowGroup->addMember(relativePosition);

            addElement(windowGroup);

			ActionTools::GroupDefinition *imageToSearchInGroup = new ActionTools::GroupDefinition(this);
			imageToSearchInGroup->setMasterList(source);
			imageToSearchInGroup->setMasterValues(QStringList() << FindImageInstance::sources.first.at(FindImageInstance::ImageSource));

            ActionTools::ImageParameterDefinition *imageToSearchIn = new ActionTools::ImageParameterDefinition(ActionTools::Name("imageToSearchIn", tr("Image to search in")), this);
			imageToSearchIn->setTooltip(tr("The image to search in"));
			imageToSearchIn->setMode(ActionTools::FileEdit::FileOpen);
			imageToSearchIn->setCaption(tr("Choose an image file"));
			imageToSearchIn->setFilter(tr("Image files (*.bmp *.gif *.jpg *.jpeg *.mng *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm *.svg)\nAll files (*.*)"));
			imageToSearchInGroup->addMember(imageToSearchIn);

			addElement(imageToSearchInGroup);

            ActionTools::ImageParameterDefinition *imageToFind = new ActionTools::ImageParameterDefinition(ActionTools::Name("imageToFind", tr("Image to find")), this);
			imageToFind->setTooltip(tr("The image to search for"));
			imageToFind->setMode(ActionTools::FileEdit::FileOpen);
			imageToFind->setCaption(tr("Choose an image file"));
			imageToFind->setFilter(tr("Image files (*.bmp *.gif *.jpg *.jpeg *.mng *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm *.svg)\nAll files (*.*)"));
			addElement(imageToFind);

            ActionTools::IfActionParameterDefinition *ifFound = new ActionTools::IfActionParameterDefinition(ActionTools::Name("ifFound", tr("If found")), this);
            ifFound->setTooltip(tr("What to do if the image is found"));
            ifFound->setAllowWait(true);
            addElement(ifFound);

            ActionTools::IfActionParameterDefinition *ifNotFound = new ActionTools::IfActionParameterDefinition(ActionTools::Name("ifNotFound", tr("If not found")), this);
            ifNotFound->setTooltip(tr("What to do if the image is not found"));
            ifNotFound->setAllowWait(true);
            addElement(ifNotFound);

			ActionTools::VariableParameterDefinition *position = new ActionTools::VariableParameterDefinition(ActionTools::Name("position", tr("Position")), this);
			position->setTooltip(tr("The name of the variable where to store the coordinates of the center of the found image"));
			addElement(position);

            ActionTools::ListParameterDefinition *method = new ActionTools::ListParameterDefinition(ActionTools::Name("method", tr("Method")), this);
            method->setTooltip(tr("The matching method to use"));
            method->setItems(FindImageInstance::methods);
            method->setDefaultValue(FindImageInstance::methods.second.at(FindImageInstance::CorrelationCoefficientMethod));
            addElement(method, 1);

			ActionTools::NumberParameterDefinition *confidenceMinimum = new ActionTools::NumberParameterDefinition(ActionTools::Name("confidenceMinimum", tr("Confidence minimum")), this);
			confidenceMinimum->setTooltip(tr("The minimum confidence percentage required to select a possible matching image"));
			confidenceMinimum->setMinimum(0);
			confidenceMinimum->setMaximum(100);
			confidenceMinimum->setDefaultValue(70);
			addElement(confidenceMinimum, 1);

			ActionTools::NumberParameterDefinition *maximumMatches = new ActionTools::NumberParameterDefinition(ActionTools::Name("maximumMatches", tr("Maximum matches")), this);
			maximumMatches->setTooltip(tr("The maximum matching images count\nSetting this parameter higher will increase the duration of the search"));
			maximumMatches->setMinimum(1);
			maximumMatches->setMaximum(std::numeric_limits<int>::max());
			maximumMatches->setDefaultValue(1);
			addElement(maximumMatches, 1);

			ActionTools::NumberParameterDefinition *downPyramidCount = new ActionTools::NumberParameterDefinition(ActionTools::Name("downPyramidCount", tr("Downsampling")), this);
			downPyramidCount->setTooltip(tr("The downsampling value to use\nDownsampling is used to accelerate the search when using large images"));
			downPyramidCount->setMinimum(1);
			downPyramidCount->setMaximum(std::numeric_limits<int>::max());
			downPyramidCount->setDefaultValue(1);
			addElement(downPyramidCount, 1);

			ActionTools::NumberParameterDefinition *searchExpansion = new ActionTools::NumberParameterDefinition(ActionTools::Name("searchExpansion", tr("Search expansion")), this);
			searchExpansion->setTooltip(tr("The number of pixels to shift when searching for another matching image"));
			searchExpansion->setMinimum(1);
			searchExpansion->setMaximum(std::numeric_limits<int>::max());
			searchExpansion->setDefaultValue(15);
			addElement(searchExpansion, 1);

            ActionTools::NumberParameterDefinition *searchDelay = new ActionTools::NumberParameterDefinition(ActionTools::Name("searchDelay", tr("Delay between two searches when waiting")), this);
            searchDelay->setTooltip(tr("The delay between two searches"));
            searchDelay->setMinimum(0);
            searchDelay->setMaximum(std::numeric_limits<int>::max());
            searchDelay->setDefaultValue(100);
            searchDelay->setSuffix(tr(" ms", "milliseconds"));
            addElement(searchDelay, 1);

            ActionTools::VariableParameterDefinition *confidence = new ActionTools::VariableParameterDefinition(ActionTools::Name("confidence", tr("Confidence")), this);
            confidence->setTooltip(tr("The name of the variable where to store the confidence value found image"));
            addElement(confidence, 1);

			addException(FindImageInstance::ErrorWhileSearchingException, tr("Error while searching"));
		}

		QString name() const													{ return QObject::tr("Find image"); }
		QString id() const														{ return "ActionFindImage"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Finds an image on the screen, on a window or on another image"); }
        Tools::Version version() const                                          { return Tools::Version(1, 1, 0); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new FindImageInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::System; }
		QPixmap icon() const													{ return QPixmap(":/icons/findimage.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

        void updateAction(ActionTools::ActionInstance *actionInstance, const Tools::Version &version) const;

	private:
        Q_DISABLE_COPY(FindImageDefinition)
    };
}

#endif // FINDIMAGEDEFINITION_H
