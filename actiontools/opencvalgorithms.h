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

#ifndef OPENCVALGORITHMS_H
#define OPENCVALGORITHMS_H

#include "actiontools_global.h"
#include "matchingpointlist.h"

#include <QObject>
#include <QList>
#include <QImage>
#include <QVector>
#include <QFuture>
#include <QFutureWatcher>
#include <QMetaType>

namespace cv
{
	class Mat;
}

namespace ActionTools
{
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

		explicit OpenCVAlgorithms(QObject *parent = 0);

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

		AlgorithmError error() const { return mError; }
		const QString &errorString() const { return mErrorString; }

	signals:
		void finished(const ActionTools::MatchingPointList &matchingPointList);

	private slots:
		void finished();

	private:
        bool checkInputImages(const QList<cv::Mat> &sources, const cv::Mat &target);

		/*=============================================================================
		  FastMatchTemplate
		  Performs a fast match template
		  Returns: true on success, false on failure
		  Parameters:
			source - source image (where we are searching)
			target - target image (what we are searching for)
			foundPointsList - contains a list of the points where the target was found
			confidencesList - contains a list of the confidence value (0-100) for each
							  found target
			matchPercentage - the minimum required match score to consider the target
							  found
			numMaxima - the maximum number of search locations to try before exiting
						(i.e. when image is down-sampled and searched, we collect the
						best numMaxima locations - those with the highest confidence -
						and search the original image at these locations)
			numDownPyrs - the number of times to down-sample the image (only increase
						  this number if your images are really large)
			searchExpansion - The original source image is searched at the top locations
							  with +/- searchExpansion pixels in both the x and y
							  directions
		*/
        MatchingPointList fastMatchTemplate(const QList<cv::Mat> &sources,
                                            const cv::Mat &target,
                                            int matchPercentage,
                                            int maximumMatches,
                                            int downPyrs,
                                            int searchExpansion,
                                            AlgorithmMethod method);

        static QVector<QPoint> multipleMinMaxLoc(const cv::Mat &image, int maximumMatches, AlgorithmMethod method);

        static QImage toQImage(const cv::Mat &image);
        static cv::Mat toCVMat(const QImage &image);
        static int toOpenCVMethod(AlgorithmMethod method);

		AlgorithmError mError;
		QString mErrorString;
		QFuture<MatchingPointList> mFuture;
		QFutureWatcher<MatchingPointList> mFutureWatcher;
	};
}

Q_DECLARE_METATYPE(ActionTools::MatchingPointList)

#endif // OPENCVALGORITHMS_H
