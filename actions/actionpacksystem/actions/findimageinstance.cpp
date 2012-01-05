/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#include "findimageinstance.h"
#include "opencvalgorithms.h"
#include "code/point.h"

#include <QPixmap>
#include <QImage>
#include <QApplication>
#include <QDesktopWidget>

namespace Actions
{
	ActionTools::StringListPair FindImageInstance::sources = qMakePair(
			QStringList() << "screenshot" << "window" << "image",
			QStringList()
			<< QT_TRANSLATE_NOOP("FindImageInstance::sources", "Screenshot")
			<< QT_TRANSLATE_NOOP("FindImageInstance::sources", "Window")
			<< QT_TRANSLATE_NOOP("FindImageInstance::sources", "Image"));

	FindImageInstance::FindImageInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		  mOpenCVAlgorithms(new ActionTools::OpenCVAlgorithms(this)),
		  mWindowRelativePosition(false),
		  mSource(ScreenshotSource),
		  mMaximumMatches(1)
	{
		connect(mOpenCVAlgorithms, SIGNAL(finished(ActionTools::MatchingPointList)), this, SLOT(searchFinished(ActionTools::MatchingPointList)));
	}

	FindImageInstance::~FindImageInstance()
	{
	}

	void FindImageInstance::startExecution()
	{
		bool ok = true;

		mSource = evaluateListElement<Source>(ok, sources, "source");
		QString imageToFindFilename = evaluateString(ok, "imageToFind");
		mPositionVariableName = evaluateVariable(ok, "position");
		mWindowRelativePosition = evaluateBoolean(ok, "windowRelativePosition");
		int confidenceMinimum = evaluateInteger(ok, "confidenceMinimum");
		mMaximumMatches = evaluateInteger(ok, "maximumMatches");
		int downPyramidCount = evaluateInteger(ok, "downPyramidCount");
		int searchExpansion = evaluateInteger(ok, "searchExpansion");

		if(!ok)
			return;

		validateParameterRange(ok, confidenceMinimum, "confidenceMinimum", tr("minimum confidence"), 0, 100);
		validateParameterRange(ok, mMaximumMatches, "maximumMatches", tr("maximum matches"), 1);
		validateParameterRange(ok, downPyramidCount, "downPyramidCount", tr("down pyramid count"), 1);
		validateParameterRange(ok, searchExpansion, "searchExpansion", tr("search expansion"), 1);

		if(!ok)
			return;

		QImage imageToFind;
		QImage imageToSearchIn;

		if(!imageToFind.load(imageToFindFilename))
		{
			emit executionException(ActionTools::ActionException::BadParameterException, tr("Unable to load image to find from file %1").arg(imageToFindFilename));

			return;
		}

		switch(mSource)
		{
		case ScreenshotSource:
			imageToSearchIn = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();
			break;
		case WindowSource:
			{
				bool ok = true;

				QString windowName = evaluateString(ok, "windowName");

				if(!ok)
					return;

				mWindow = ActionTools::WindowHandle::findWindow(QRegExp(windowName, Qt::CaseSensitive, QRegExp::WildcardUnix));

				if(!mWindow.isValid())
				{
					emit executionException(ActionTools::ActionException::BadParameterException, tr("Unable to find any window named %1").arg(windowName));

					return;
				}

				imageToSearchIn = QPixmap::grabWindow(mWindow.value()).toImage();

				if(imageToSearchIn.isNull())
				{
					emit executionException(ActionTools::ActionException::BadParameterException, tr("Unable to take a screenshot of the window named %1").arg(mWindow.title()));

					return;
				}
			}
			break;
		case ImageSource:
			{
				bool ok = true;

				QString imageToSearchInFilename = evaluateString(ok, "imageToSearchIn");

				if(!ok)
					return;

				if(!imageToSearchIn.load(imageToSearchInFilename))
				{
					emit executionException(ActionTools::ActionException::BadParameterException, tr("Unable to load image to search in from file %1").arg(imageToSearchInFilename));

					return;
				}
			}
			break;
		}

		if(!mOpenCVAlgorithms->findSubImageAsync(imageToSearchIn, imageToFind, confidenceMinimum, mMaximumMatches, downPyramidCount, searchExpansion))
		{
			emit executionException(ErrorWhileSearchingException, tr("Error while searching: %1").arg(mOpenCVAlgorithms->errorString()));

			return;
		}
	}
	void FindImageInstance::searchFinished(const ActionTools::MatchingPointList &matchingPointList)
	{
		if(matchingPointList.empty())
		{
			emit executionException(CannotFindTheImageException, tr("Cannot find the image"));

			return;
		}

		if(mMaximumMatches == 1)
		{
			QPoint position = matchingPointList.first().first;

			if(mSource == WindowSource && !mWindowRelativePosition)
				position += mWindow.rect().topLeft();

			setVariableFromScriptValue(mPositionVariableName, Code::Point::constructor(position, scriptEngine()));
		}
		else
		{
			QScriptValue arrayResult = scriptEngine()->newArray(matchingPointList.size());

			for(int i = 0; i < matchingPointList.size(); ++i)
				arrayResult.setProperty(i, Code::Point::constructor(matchingPointList.at(i).first, scriptEngine()));

			setVariableFromScriptValue(mPositionVariableName, arrayResult);
		}

		emit executionEnded();
	}

	void FindImageInstance::validateParameterRange(bool &ok, int parameter, const QString &parameterName, const QString &parameterTranslatedName, int minimum, int maximum)
	{
		if(ok && (parameter < minimum || parameter > maximum))
		{
			ok = false;

			setCurrentParameter(parameterName);
			emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid %1 value : %2").arg(parameterTranslatedName).arg(parameter));
			return;
		}
	}
}

