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

#include "opencvalgorithms.hpp"

#include <opencv2/opencv.hpp>

#include <QList>

#if CV_MAJOR_VERSION == 2
namespace cv
{
    class Mat;
}

using Mat = cv::Mat;
#elif CV_MAJOR_VERSION >= 3
namespace cv
{
    class UMat;
}

using Mat = cv::UMat;
#endif

namespace ActionTools
{
    struct OpenCVAlgorithmsPrivate
    {
        bool checkInputImages(const QList<Mat> &sources, const Mat &target);

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
        MatchingPointList fastMatchTemplate(const QList<Mat> &sources,
                                            const Mat &target,
                                            int matchPercentage,
                                            int maximumMatches,
                                            int downPyrs,
                                            int searchExpansion,
                                            OpenCVAlgorithms::AlgorithmMethod method);

        static QVector<QPoint> multipleMinMaxLoc(const Mat &image, int maximumMatches, OpenCVAlgorithms::AlgorithmMethod method);

        static QImage toQImage(const Mat &image);
        static Mat toCVMat(const QImage &image);
        static int toOpenCVMethod(OpenCVAlgorithms::AlgorithmMethod method);

        OpenCVAlgorithms::AlgorithmError mError{OpenCVAlgorithms::NoError};
        QString mErrorString;
        QFuture<MatchingPointList> mFuture;
        QFutureWatcher<MatchingPointList> mFutureWatcher;
    };
}
