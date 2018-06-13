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

#include "actioninstance.h"
#include "matchingpointlist.h"
#include "windowhandle.h"

#include <QImage>
#include <QTimer>

#include <limits>
#include <utility>

namespace ActionTools
{
	class OpenCVAlgorithms;
}

namespace Actions
{
	class FindImageInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Source)
        Q_ENUMS(Method)

	public:
		enum Source
		{
			ScreenshotSource,
			WindowSource,
			ImageSource
		};
        enum Method
        {
            CorrelationCoefficientMethod,
            CrossCorrelationMethod,
            SquaredDifferenceMethod
        };
		enum Exceptions
		{
            ErrorWhileSearchingException = ActionTools::ActionException::UserException,
            CannotFindTheImageException
		};

		FindImageInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr);
		~FindImageInstance() override;

        static Tools::StringListPair sources;
        static Tools::StringListPair methods;

		void startExecution() override;
        void stopExecution() override;

	private slots:
        void startSearching();
		void searchFinished(const ActionTools::MatchingPointList &matchingPointList);

	private:
		ActionTools::OpenCVAlgorithms *mOpenCVAlgorithms;
		QString mPositionVariableName;
        QString mConfidenceVariableName;
        Method mMethod;
		bool mWindowRelativePosition;
        int mConfidenceMinimum;
        QList<std::pair<QPixmap, QRect>> mImagesToSearchIn;
        QList<ActionTools::WindowHandle> mWindows;
        Source mSource;
        ActionTools::IfActionValue mIfFound;
        ActionTools::IfActionValue mIfNotFound;
        QImage mImageToFind;
		int mMaximumMatches;
        int mDownPyramidCount;
        int mSearchExpansion;
        int mSearchDelay;
        QTimer mWaitTimer;

		Q_DISABLE_COPY(FindImageInstance)
	};
}

