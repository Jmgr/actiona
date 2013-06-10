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

#ifndef FINDIMAGEINSTANCE_H
#define FINDIMAGEINSTANCE_H

#include "actioninstance.h"
#include "matchingpointlist.h"
#include "windowhandle.h"

#include <limits>
#include <QTimer>

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

	public:
		enum Source
		{
			ScreenshotSource,
			WindowSource,
			ImageSource
		};
		enum Exceptions
		{
			ErrorWhileSearchingException = ActionTools::ActionException::UserException
		};

		FindImageInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0);
		~FindImageInstance();

		static ActionTools::StringListPair sources;

		void startExecution();
		void stopExecution();

	private slots:
		void searchFinished(const ActionTools::MatchingPointList &matchingPointList);
		void checkImage();

	private:
		void validateParameterRange(bool &ok, int parameter, const QString &parameterName, const QString &parameterTranslatedName, int minimum, int maximum = std::numeric_limits<int>::max());

		void searchImage();

		ActionTools::OpenCVAlgorithms *mOpenCVAlgorithms;
		QString mPositionVariableName;
		bool mWindowRelativePosition;
		ActionTools::WindowHandle mWindow;
		Source mSource;
		int mMaximumMatches;
		ActionTools::IfActionValue mIfFalse;
		ActionTools::IfActionValue mIfTrue;
		QTimer mTimer;

		QString mImageToFindFilename;
		bool mInactivateTimer;
		int mPause;
		int mConfidenceMinimum;
		int mDownPyramidCount;
		int mSearchExpansion;

		Q_DISABLE_COPY(FindImageInstance)
	};
}

#endif // FINDIMAGEINSTANCE_H
