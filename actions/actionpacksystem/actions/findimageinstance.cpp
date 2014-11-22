/*
	Actiona
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#include "findimageinstance.h"
#include "opencvalgorithms.h"
#include "code/point.h"
#include "screenshooter.h"

#include <QPixmap>
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
    ActionTools::StringListPair FindImageInstance::methods = qMakePair(
            QStringList() << "correlationcoefficient" << "crosscorrelation" << "squareddifference",
            QStringList()
            << QT_TRANSLATE_NOOP("FindImageInstance::sources", "Correlation Coefficient")
            << QT_TRANSLATE_NOOP("FindImageInstance::sources", "Cross Correlation")
            << QT_TRANSLATE_NOOP("FindImageInstance::sources", "Squared Difference"));

	FindImageInstance::FindImageInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		  mOpenCVAlgorithms(new ActionTools::OpenCVAlgorithms(this)),
          mMethod(CorrelationCoefficientMethod),
		  mWindowRelativePosition(false),
          mConfidenceMinimum(0),
          mSource(ScreenshotSource),
          mMaximumMatches(1),
          mDownPyramidCount(0),
          mSearchExpansion(0)
	{
		connect(mOpenCVAlgorithms, SIGNAL(finished(ActionTools::MatchingPointList)), this, SLOT(searchFinished(ActionTools::MatchingPointList)));
        connect(&mWaitTimer, SIGNAL(timeout()), this, SLOT(startSearching()));

        mWaitTimer.setSingleShot(true);
	}

	FindImageInstance::~FindImageInstance()
	{
	}

	void FindImageInstance::startExecution()
	{
		bool ok = true;

		mSource = evaluateListElement<Source>(ok, sources, "source");
        mImageToFind = evaluateImage(ok, "imageToFind");
        mIfFound = evaluateIfAction(ok, "ifFound");
        mIfNotFound = evaluateIfAction(ok, "ifNotFound");
		mPositionVariableName = evaluateVariable(ok, "position");
        mMethod = evaluateListElement<Method>(ok, methods, "method");
		mWindowRelativePosition = evaluateBoolean(ok, "windowRelativePosition");
        mConfidenceMinimum = evaluateInteger(ok, "confidenceMinimum");
		mMaximumMatches = evaluateInteger(ok, "maximumMatches");
        mDownPyramidCount = evaluateInteger(ok, "downPyramidCount");
        mSearchExpansion = evaluateInteger(ok, "searchExpansion");
        mConfidenceVariableName = evaluateVariable(ok, "confidence");
        mSearchDelay = evaluateInteger(ok, "searchDelay");

		if(!ok)
			return;

        validateParameterRange(ok, mConfidenceMinimum, "confidenceMinimum", tr("minimum confidence"), 0, 100);
		validateParameterRange(ok, mMaximumMatches, "maximumMatches", tr("maximum matches"), 1);
		validateParameterRange(ok, mDownPyramidCount, "downPyramidCount", tr("downsampling"), 1);
        validateParameterRange(ok, mSearchExpansion, "searchExpansion", tr("search expansion"), 1);

		if(!ok)
			return;

        if(mImageToFind.isNull())
		{
            emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid image to find"));

			return;
		}

        startSearching();
    }

    void FindImageInstance::stopExecution()
    {
        mOpenCVAlgorithms->cancelSearch();

        mWaitTimer.stop();
    }

    void FindImageInstance::startSearching()
    {
        mOpenCVAlgorithms->cancelSearch();

        mImagesToSearchIn.clear();

        switch(mSource)
        {
        case ScreenshotSource:
            mImagesToSearchIn = ActionTools::ScreenShooter::captureScreens();
            break;
        case WindowSource:
            {
                bool ok = true;

                QString windowName = evaluateString(ok, "windowName");

                if(!ok)
                    return;

                mWindows = ActionTools::WindowHandle::findWindows(QRegExp(windowName, Qt::CaseSensitive, QRegExp::WildcardUnix));

                if(mWindows.isEmpty())
                {
                    emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Unable to find any window named %1").arg(windowName));

                    return;
                }

                mImagesToSearchIn = ActionTools::ScreenShooter::captureWindows(mWindows);
            }
            break;
        case ImageSource:
            {
                bool ok = true;

                QImage imageToSearchIn = evaluateImage(ok, "imageToSearchIn");

                if(!ok)
                    return;

                if(imageToSearchIn.isNull())
                {
                    emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid image to search in"));

                    return;
                }

                mImagesToSearchIn.append(qMakePair(QPixmap::fromImage(imageToSearchIn), imageToSearchIn.rect()));
            }
            break;
        }

        QList<QImage> sourceImages;
        sourceImages.reserve(mImagesToSearchIn.size());

        typedef QPair<QPixmap, QRect> PixmapRectPair;
        for(const PixmapRectPair &imageToSearchIn: mImagesToSearchIn)
            sourceImages.append(imageToSearchIn.first.toImage());

        if(!mOpenCVAlgorithms->findSubImageAsync(sourceImages,
                                                 mImageToFind,
                                                 mConfidenceMinimum,
                                                 mMaximumMatches,
                                                 mDownPyramidCount,
                                                 mSearchExpansion,
                                                 static_cast<ActionTools::OpenCVAlgorithms::AlgorithmMethod>(mMethod)))
        {
            emit executionException(ErrorWhileSearchingException, tr("Error while searching: %1").arg(mOpenCVAlgorithms->errorString()));

            return;
        }
    }

	void FindImageInstance::searchFinished(const ActionTools::MatchingPointList &matchingPointList)
	{
        bool ok = true;

        if(matchingPointList.empty())
        {
            setCurrentParameter("ifNotFound", "line");

            QString line = evaluateSubParameter(ok, mIfNotFound.actionParameter());
            if(!ok)
                return;

            if(mIfNotFound.action() == ActionTools::IfActionValue::GOTO)
            {
                setNextLine(line);

                emit executionEnded();
            }
            else if(mIfNotFound.action() == ActionTools::IfActionValue::CALLPROCEDURE)
            {
                if(!callProcedure(line))
                    return;

                emit executionEnded();
            }
            else if(mIfNotFound.action() == ActionTools::IfActionValue::WAIT)
            {
                mWaitTimer.start(mSearchDelay);
            }
            else
                emit executionEnded();

            return;
        }

		if(mMaximumMatches == 1)
		{
            const ActionTools::MatchingPoint &bestMatchingPoint = matchingPointList.first();
            QPoint position = bestMatchingPoint.position;

            if(mSource != WindowSource || !mWindowRelativePosition)
                position += mImagesToSearchIn.at(bestMatchingPoint.imageIndex).second.topLeft();

			setVariable(mPositionVariableName, Code::Point::constructor(position, scriptEngine()));
            setVariable(mConfidenceVariableName, bestMatchingPoint.confidence);
		}
		else
		{
			QScriptValue arrayResult = scriptEngine()->newArray(matchingPointList.size());
            QScriptValue arrayConfidenceResult = scriptEngine()->newArray(matchingPointList.size());

			for(int i = 0; i < matchingPointList.size(); ++i)
            {
                const ActionTools::MatchingPoint &matchingPoint = matchingPointList.at(i);
                QPoint position = matchingPoint.position;

                if(mSource != WindowSource || !mWindowRelativePosition)
                    position += mImagesToSearchIn.at(matchingPoint.imageIndex).second.topLeft();

                arrayResult.setProperty(i, Code::Point::constructor(position, scriptEngine()));
                arrayConfidenceResult.setProperty(i, matchingPoint.confidence);
            }

			setVariable(mPositionVariableName, arrayResult);
            setVariable(mConfidenceVariableName, arrayConfidenceResult);
		}

        setCurrentParameter("ifFound", "line");

        QString line = evaluateSubParameter(ok, mIfFound.actionParameter());
        if(!ok)
            return;

        if(mIfFound.action() == ActionTools::IfActionValue::GOTO)
        {
            setNextLine(line);

            emit executionEnded();
        }
        else if(mIfFound.action() == ActionTools::IfActionValue::CALLPROCEDURE)
        {
            if(!callProcedure(line))
                return;

            emit executionEnded();
        }
        else if(mIfFound.action() == ActionTools::IfActionValue::WAIT)
        {
            mWaitTimer.start(mSearchDelay);
        }
        else
            emit executionEnded();
    }

	void FindImageInstance::validateParameterRange(bool &ok, int parameter, const QString &parameterName, const QString &parameterTranslatedName, int minimum, int maximum)
	{
		if(ok && (parameter < minimum || parameter > maximum))
		{
			ok = false;

			setCurrentParameter(parameterName);
			emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid %1 value : %2").arg(parameterTranslatedName).arg(parameter));
			return;
		}
	}
}

