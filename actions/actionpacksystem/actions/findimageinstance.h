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

#ifndef FINDIMAGEINSTANCE_H
#define FINDIMAGEINSTANCE_H

#include "actioninstance.h"
#include "matchingpointlist.h"
#include "windowhandle.h"

#include <QImage>
#include <QTimer>

#include <limits>

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

		FindImageInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0);
		~FindImageInstance();

		static ActionTools::StringListPair sources;
        static ActionTools::StringListPair methods;

		void startExecution();
        void stopExecution();

	private slots:
        void startSearching();
		void searchFinished(const ActionTools::MatchingPointList &matchingPointList);

	private:
		void validateParameterRange(bool &ok, int parameter, const QString &parameterName, const QString &parameterTranslatedName, int minimum, int maximum = std::numeric_limits<int>::max());

		ActionTools::OpenCVAlgorithms *mOpenCVAlgorithms;
		QString mPositionVariableName;
        QString mConfidenceVariableName;
        Method mMethod;
		bool mWindowRelativePosition;
        int mConfidenceMinimum;
        QList< QPair<QPixmap, QRect> > mImagesToSearchIn;
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

#endif // FINDIMAGEINSTANCE_H
