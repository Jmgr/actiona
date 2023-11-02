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

/***************************************************************************
 *            FastMatchTemplate.h
 *
 *
 *  Copyright  2010  Tristen Georgiou
 *  tristen_georgiou@hotmail.com
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


	How fast match template works:
	1. Both target and source image are down sampled numDownPyrs times
	2. cvMatchTemplate() function is called on shrunken images
	   (uses CCORR_NORMED algorithm)
	3. The numMaxima best locations are found
	4. For each point where a maxima was located:
	   a) Original source image is searched at point +/- searchExpansion
		  pixels in both x and y direction
	5. If match score is above matchPercentage then the location and score is
	   saved in the foundPointsList and confidencesList, respectively
	6. If findMultipleTargets is true, an attempt will be made to find up to
	   numMaxima targets
	7. (Optional) The targets can be drawn to a color version of the source
	   image using the DrawFoundTargets() function
 */
/*
  Adapted to the Actiona project by Jonathan Mercier-Ganady
  Original files: http://opencv.willowgarage.com/wiki/FastMatchTemplate
*/

#pragma once

#include "actiontools_global.hpp"
#include "matchingpointlist.hpp"

#include <QObject>
#include <QList>
#include <QImage>
#include <QVector>
#include <QFuture>
#include <QFutureWatcher>
#include <QMetaType>

#include <memory>

namespace ActionTools
{
    struct OpenCVAlgorithmsPrivate;

	class ACTIONTOOLSSHARED_EXPORT OpenCVAlgorithms : public QObject
	{
		Q_OBJECT

	public:
		enum AlgorithmError
		{
			NoError,
			AlreadyRunningError,
			SourceImageSmallerThanTargerImageError,
			NotSameDepthError,
			NotSameChannelCountError,
			OpenCVException
		};

        enum AlgorithmMethod
        {
            CorrelationCoefficientMethod,
            CrossCorrelationMethod,
            SquaredDifferenceMethod
        };

		explicit OpenCVAlgorithms(QObject *parent = nullptr);
        ~OpenCVAlgorithms() override ;

        bool findSubImageAsync(const QList<QImage> &sources,
						  const QImage &target,
						  int matchPercentage = 70,
						  int maximumMatches = 10,
						  int downPyrs = 2,
                          int searchExpansion = 15,
                          AlgorithmMethod method = CorrelationCoefficientMethod);
        bool findSubImage(const QList<QImage> &sources,
						  const QImage &target,
						  MatchingPointList &matchingPoints,
						  int matchPercentage = 70,
						  int maximumMatches = 10,
						  int downPyrs = 2,
                          int searchExpansion = 15,
                          AlgorithmMethod method = CorrelationCoefficientMethod);
        void cancelSearch();

        AlgorithmError error() const;
        const QString &errorString() const;

	signals:
		void finished(const ActionTools::MatchingPointList &matchingPointList);

	private slots:
        void onFinished();

    private:
        std::unique_ptr<OpenCVAlgorithmsPrivate> mPrivate;
	};
}
