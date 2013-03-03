/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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

#ifndef FINDIMAGEDEFINITION_H
#define FINDIMAGEDEFINITION_H

#include "actiondefinition.h"
#include "findimageinstance.h"
#include "listparameterdefinition.h"
#include "fileparameterdefinition.h"
#include "variableparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "groupdefinition.h"
#include "windowparameterdefinition.h"
#include "booleanparameterdefinition.h"

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

			ActionTools::ListParameterDefinition *source = new ActionTools::ListParameterDefinition(ActionTools::Name("source", tr("Source")), this);
			source->setTooltip(tr("The source of the image to search in"));
			source->setItems(FindImageInstance::sources);
			source->setDefaultValue(FindImageInstance::sources.second.at(FindImageInstance::ScreenshotSource));
			addElement(source);

			ActionTools::GroupDefinition *windowNameGroup = new ActionTools::GroupDefinition(this);
			windowNameGroup->setMasterList(source);
			windowNameGroup->setMasterValues(QStringList() << FindImageInstance::sources.first.at(FindImageInstance::WindowSource));

			ActionTools::WindowParameterDefinition *windowName = new ActionTools::WindowParameterDefinition(ActionTools::Name("windowName", tr("Window name")), this);
			windowName->setTooltip(tr("The title of the window to search in, you can use wildcards like * (any number of characters) or ? (one character) here"));
			windowNameGroup->addMember(windowName);

			addElement(windowNameGroup);

			ActionTools::GroupDefinition *imageToSearchInGroup = new ActionTools::GroupDefinition(this);
			imageToSearchInGroup->setMasterList(source);
			imageToSearchInGroup->setMasterValues(QStringList() << FindImageInstance::sources.first.at(FindImageInstance::ImageSource));

			ActionTools::FileParameterDefinition *imageToSearchIn = new ActionTools::FileParameterDefinition(ActionTools::Name("imageToSearchIn", tr("Image to search in")), this);
			imageToSearchIn->setTooltip(tr("The image to search in"));
			imageToSearchIn->setMode(ActionTools::FileEdit::FileOpen);
			imageToSearchIn->setCaption(tr("Choose an image file"));
			imageToSearchIn->setFilter(tr("Image files (*.bmp *.gif *.jpg *.jpeg *.mng *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm *.svg)\nAll files (*.*)"));
			imageToSearchInGroup->addMember(imageToSearchIn);

			addElement(imageToSearchInGroup);

			ActionTools::FileParameterDefinition *imageToFind = new ActionTools::FileParameterDefinition(ActionTools::Name("imageToFind", tr("Image to find")), this);
			imageToFind->setTooltip(tr("The image to search for"));
			imageToFind->setMode(ActionTools::FileEdit::FileOpen);
			imageToFind->setCaption(tr("Choose an image file"));
			imageToFind->setFilter(tr("Image files (*.bmp *.gif *.jpg *.jpeg *.mng *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm *.svg)\nAll files (*.*)"));
			addElement(imageToFind);

			ActionTools::VariableParameterDefinition *position = new ActionTools::VariableParameterDefinition(ActionTools::Name("position", tr("Position")), this);
			position->setTooltip(tr("The name of the variable where to store the coordinates of the center of the found image"));
			addElement(position);

			ActionTools::GroupDefinition *relativePositionGroup = new ActionTools::GroupDefinition(this);
			relativePositionGroup->setMasterList(source);
			relativePositionGroup->setMasterValues(QStringList() << FindImageInstance::sources.first.at(FindImageInstance::WindowSource));

			ActionTools::BooleanParameterDefinition *relativePosition = new ActionTools::BooleanParameterDefinition(ActionTools::Name("windowRelativePosition", tr("Window relative position")), this);
			relativePosition->setTooltip(tr("The position is relative to the window\nIf this parameter is set to false (not checked) then the position is absolute"));
			relativePositionGroup->addMember(relativePosition);

			addElement(relativePositionGroup, 1);

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

			ActionTools::NumberParameterDefinition *downPyramidCount = new ActionTools::NumberParameterDefinition(ActionTools::Name("downPyramidCount", tr("Down pyramid count")), this);
			downPyramidCount->setTooltip(tr("The number of down pyramids to use\nA pyramid is a subdivision of the image used to accelerate the search\nEnter 1 here if the searched image is not very different from the source image"));
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

			addException(FindImageInstance::ErrorWhileSearchingException, tr("Error while searching"));
			addException(FindImageInstance::CannotFindTheImageException, tr("Cannot find the image"));
		}

		QString name() const													{ return QObject::tr("Find image"); }
		QString id() const														{ return "ActionFindImage"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Finds an image on the screen, on a window or on another image"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new FindImageInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::System; }
		QPixmap icon() const													{ return QPixmap(":/icons/findimage.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(FindImageDefinition)
	};
}

#endif // FINDIMAGEDEFINITION_H
