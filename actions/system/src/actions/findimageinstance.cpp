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

	Contact: jmgr@jmgr.info
*/

#include "findimageinstance.hpp"
#include "actiontools/opencvalgorithms.hpp"
#include "actiontools/code/point.hpp"
#include "actiontools/screenshooter.hpp"

#include <QPixmap>
#include <QApplication>
#include <QRegularExpression>

namespace Actions
{
    Tools::StringListPair FindImageInstance::sources =
    {
        {
            QStringLiteral("screenshot"),
            QStringLiteral("window"),
            QStringLiteral("image")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("FindImageInstance::sources", "Screenshot")),
            QStringLiteral(QT_TRANSLATE_NOOP("FindImageInstance::sources", "Window")),
            QStringLiteral(QT_TRANSLATE_NOOP("FindImageInstance::sources", "Image"))
        }
    };
    Tools::StringListPair FindImageInstance::methods =
    {
        {
            QStringLiteral("correlationcoefficient"),
            QStringLiteral("crosscorrelation"),
            QStringLiteral("squareddifference")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("FindImageInstance::sources", "Correlation Coefficient")),
            QStringLiteral(QT_TRANSLATE_NOOP("FindImageInstance::sources", "Cross Correlation")),
            QStringLiteral(QT_TRANSLATE_NOOP("FindImageInstance::sources", "Squared Difference"))
        }
    };

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
        mWaitTimer.setSingleShot(true);
        mWaitTimer.setTimerType(Qt::PreciseTimer);

        connect(mOpenCVAlgorithms, static_cast<void (ActionTools::OpenCVAlgorithms::*)(const ActionTools::MatchingPointList &)>(&ActionTools::OpenCVAlgorithms::finished),
                this, &FindImageInstance::searchFinished);
        connect(&mWaitTimer, &QTimer::timeout, this, &FindImageInstance::startSearching);
	}

    FindImageInstance::~FindImageInstance() = default;

	void FindImageInstance::startExecution()
	{
		bool ok = true;

		mSource = evaluateListElement<Source>(ok, sources, QStringLiteral("source"));
		mImageToFind = evaluateImage(ok, QStringLiteral("imageToFind"));
		mIfFound = evaluateIfAction(ok, QStringLiteral("ifFound"));
		mIfNotFound = evaluateIfAction(ok, QStringLiteral("ifNotFound"));
		mPositionVariableName = evaluateVariable(ok, QStringLiteral("position"));
		mMethod = evaluateListElement<Method>(ok, methods, QStringLiteral("method"));
		mWindowRelativePosition = evaluateBoolean(ok, QStringLiteral("windowRelativePosition"));
		mConfidenceMinimum = evaluateInteger(ok, QStringLiteral("confidenceMinimum"));
		mMaximumMatches = evaluateInteger(ok, QStringLiteral("maximumMatches"));
		mDownPyramidCount = evaluateInteger(ok, QStringLiteral("downPyramidCount"));
		mSearchExpansion = evaluateInteger(ok, QStringLiteral("searchExpansion"));
		mConfidenceVariableName = evaluateVariable(ok, QStringLiteral("confidence"));
		mSearchDelay = evaluateInteger(ok, QStringLiteral("searchDelay"));

		if(!ok)
			return;

		validateParameterRange(ok, mConfidenceMinimum, QStringLiteral("confidenceMinimum"), tr("minimum confidence"), 0, 100);
		validateParameterRange(ok, mMaximumMatches, QStringLiteral("maximumMatches"), tr("maximum matches"), 1);
		validateParameterRange(ok, mDownPyramidCount, QStringLiteral("downPyramidCount"), tr("downsampling"), 1);
		validateParameterRange(ok, mSearchExpansion, QStringLiteral("searchExpansion"), tr("search expansion"), 1);

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

    QString wildcardToRegex(const QString &wildcard) {
        QString pattern = QRegularExpression::escape(wildcard);
        pattern.replace(QStringLiteral("\\*"), QStringLiteral(".*"));
        pattern.replace(QStringLiteral("\\?"), QStringLiteral("."));
        return pattern;
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

				QString windowName = evaluateString(ok, QStringLiteral("windowName"));

                if(!ok)
                    return;

                mWindows = ActionTools::WindowHandle::findWindows(QRegularExpression::fromWildcard(windowName, Qt::CaseSensitive));

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

				QImage imageToSearchIn = evaluateImage(ok, QStringLiteral("imageToSearchIn"));

                if(!ok)
                    return;

                if(imageToSearchIn.isNull())
                {
                    emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid image to search in"));

                    return;
                }

                mImagesToSearchIn.append(std::make_pair(QPixmap::fromImage(imageToSearchIn), imageToSearchIn.rect()));
            }
            break;
        }

        QList<QImage> sourceImages;
        sourceImages.reserve(mImagesToSearchIn.size());

        for(const auto &imageToSearchIn: qAsConst(mImagesToSearchIn))
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
			setCurrentParameter(QStringLiteral("ifNotFound"), QStringLiteral("line"));

            QString line = evaluateSubParameter(ok, mIfNotFound.actionParameter());
            if(!ok)
                return;

            bool stopScript = false;
            if(mIfNotFound.action() == ActionTools::IfActionValue::GOTO)
            {
                setNextLine(line);
            }
            else if(mIfNotFound.action() == ActionTools::IfActionValue::CALLPROCEDURE)
            {
                if(!callProcedure(line))
                    return;
            }
            else if(mIfNotFound.action() == ActionTools::IfActionValue::STOPEXECUTION)
            {
                stopScript = true;
            }
            else if(mIfNotFound.action() == ActionTools::IfActionValue::WAIT)
            {
                mWaitTimer.start(mSearchDelay);
                return;
            }

            executionEnded(stopScript);
            return;
        }

		if(mMaximumMatches == 1)
		{
            const ActionTools::MatchingPoint &bestMatchingPoint = matchingPointList.first();
            QPoint position = bestMatchingPoint.position;

            if(mSource != WindowSource || !mWindowRelativePosition)
                position += mImagesToSearchIn.at(bestMatchingPoint.imageIndex).second.topLeft();

            setVariable(mPositionVariableName, Code::CodeClass::construct<Code::Point>(position, *scriptEngine()));
            setVariable(mConfidenceVariableName, bestMatchingPoint.confidence);
		}
		else
		{
            QJSValue arrayResult = scriptEngine()->engine().newArray(matchingPointList.size());
            QJSValue arrayConfidenceResult = scriptEngine()->engine().newArray(matchingPointList.size());

			for(int i = 0; i < matchingPointList.size(); ++i)
            {
                const ActionTools::MatchingPoint &matchingPoint = matchingPointList.at(i);
                QPoint position = matchingPoint.position;

                if(mSource != WindowSource || !mWindowRelativePosition)
                    position += mImagesToSearchIn.at(matchingPoint.imageIndex).second.topLeft();

                arrayResult.setProperty(i, Code::CodeClass::construct<Code::Point>(position, *scriptEngine()));
                arrayConfidenceResult.setProperty(i, matchingPoint.confidence);
            }

			setVariable(mPositionVariableName, arrayResult);
            setVariable(mConfidenceVariableName, arrayConfidenceResult);
		}

		setCurrentParameter(QStringLiteral("ifFound"), QStringLiteral("line"));

        QString line = evaluateSubParameter(ok, mIfFound.actionParameter());
        if(!ok)
            return;

        bool stopScript = false;
        if(mIfFound.action() == ActionTools::IfActionValue::GOTO)
        {
            setNextLine(line);
        }
        else if(mIfFound.action() == ActionTools::IfActionValue::CALLPROCEDURE)
        {
            if(!callProcedure(line))
                return;
        }
        else if(mIfFound.action() == ActionTools::IfActionValue::STOPEXECUTION)
        {
            stopScript = true;
        }
        else if(mIfFound.action() == ActionTools::IfActionValue::WAIT)
        {
            mWaitTimer.start(mSearchDelay);
            return;
        }

        executionEnded(stopScript);
    }
}

