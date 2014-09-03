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

#include <QtGlobal>

#include <opencv2/opencv.hpp>

#include "opencvalgorithms.h"

#include <boost/bind.hpp>

#include <QtConcurrentRun>

namespace ActionTools
{
	OpenCVAlgorithms::OpenCVAlgorithms(QObject *parent)
		: QObject(parent),
		  mError(NoError)
	{
		qRegisterMetaType<MatchingPointList>("MatchingPointList");
	}

    bool OpenCVAlgorithms::findSubImageAsync(const QList<QImage> &sources,
                      const QImage &target,
                      int matchPercentage,
                      int maximumMatches,
                      int downPyrs,
                      int searchExpansion,
                      AlgorithmMethod method)
	{
		mError = NoError;
		mErrorString.clear();

		if(mFuture.isRunning())
		{
			mError = AlreadyRunningError;
			mErrorString = tr("FindSubImage is already running");

			return false;
		}

        QList<cv::Mat> sourcesMat;
        sourcesMat.reserve(sources.size());

        foreach(const QImage &source, sources)
            sourcesMat.append(toCVMat(source));

        cv::Mat targetMat = toCVMat(target);

        if(!checkInputImages(sourcesMat, targetMat))
			return false;

        connect(&mFutureWatcher, SIGNAL(finished()), this, SLOT(finished()));

        mFuture = QtConcurrent::run(boost::bind(&OpenCVAlgorithms::fastMatchTemplate, this, sourcesMat, targetMat, matchPercentage, maximumMatches, downPyrs, searchExpansion, method));
		mFutureWatcher.setFuture(mFuture);

		return true;
	}

    bool OpenCVAlgorithms::findSubImage(const QList<QImage> &sources,
                      const QImage &target,
                      MatchingPointList &matchingPoints,
                      int matchPercentage,
                      int maximumMatches,
                      int downPyrs,
                      int searchExpansion,
                      AlgorithmMethod method)
	{
		mError = NoError;
		mErrorString.clear();

        QList<cv::Mat> sourcesMat;
        sourcesMat.reserve(sources.size());

        foreach(const QImage &source, sources)
            sourcesMat.append(toCVMat(source));

        cv::Mat targetMat = toCVMat(target);

        if(!checkInputImages(sourcesMat, targetMat))
			return false;

        matchingPoints = OpenCVAlgorithms::fastMatchTemplate(sourcesMat, targetMat, matchPercentage, maximumMatches, downPyrs, searchExpansion, method);

        return true;
    }

    void OpenCVAlgorithms::cancelSearch()
    {
        mFutureWatcher.cancel();
        mFutureWatcher.disconnect();
    }

	void OpenCVAlgorithms::finished()
	{
		emit finished(mFutureWatcher.result());
	}

    bool OpenCVAlgorithms::checkInputImages(const QList<cv::Mat> &sources, const cv::Mat &target)
	{
        foreach(const cv::Mat &source, sources)
        {
            // make sure that the template image is smaller than the source
            if(target.size().width > source.size().width ||
               target.size().height > source.size().height)
            {
                mError = SourceImageSmallerThanTargerImageError;
                mErrorString = tr("Source images must be larger than target image");

                return false;
            }

            if(source.depth() != target.depth())
            {
                mError = NotSameDepthError;
                mErrorString = tr("Source images and target image must have same depth");

                return false;
            }

            if(source.channels() != target.channels())
            {
                mError = NotSameChannelCountError;
                mErrorString = tr("Source images and target image must have same number of channels");

                return false;
            }
        }

		return true;
	}

    MatchingPointList OpenCVAlgorithms::fastMatchTemplate(const QList<cv::Mat> &sources,
                                                                            const cv::Mat &target,
																			int matchPercentage,
																			int maximumMatches,
																			int downPyrs,
                                                                            int searchExpansion,
                                                                            AlgorithmMethod method)
	{
        MatchingPointList matchingPointList;
        int sourceIndex = 0;

        foreach(const cv::Mat &source, sources)
        {
            try
            {
                // create copies of the images to modify
                cv::Mat copyOfSource = source.clone();
                cv::Mat copyOfTarget = target.clone();

                cv::Size sourceSize = source.size();
                cv::Size targetSize = target.size();

                // down pyramid the images
                for(int ii = 0; ii < downPyrs; ii++)
                {
                    // start with the source image
                    sourceSize.width  = (sourceSize.width  + 1) / 2;
                    sourceSize.height = (sourceSize.height + 1) / 2;

                    cv::Mat smallSource(sourceSize, source.type());
                    cv::pyrDown(copyOfSource, smallSource);

                    // prepare for next loop, if any
                    copyOfSource = smallSource.clone();

                    // next, do the target
                    targetSize.width  = (targetSize.width  + 1) / 2;
                    targetSize.height = (targetSize.height + 1) / 2;

                    cv::Mat smallTarget(targetSize, target.type());
                    pyrDown(copyOfTarget, smallTarget);

                    // prepare for next loop, if any
                    copyOfTarget = smallTarget.clone();
                }

                // perform the match on the shrunken images
                cv::Size smallTargetSize = copyOfTarget.size();
                cv::Size smallSourceSize = copyOfSource.size();

                cv::Size resultSize;
                resultSize.width = smallSourceSize.width - smallTargetSize.width + 1;
                resultSize.height = smallSourceSize.height - smallTargetSize.height + 1;

                cv::Mat result(resultSize, CV_32FC1);
                cv::matchTemplate(copyOfSource, copyOfTarget, result, toOpenCVMethod(method));

                // find the top match locations
                QVector<QPoint> locations = multipleMinMaxLoc(result, maximumMatches, method);

                // search the large images at the returned locations
                sourceSize = source.size();
                targetSize = target.size();

                int twoPowerNumDownPyrs = std::pow(2.0f, downPyrs);

                // create a copy of the source in order to adjust its ROI for searching
                for(int currMax = 0; currMax < maximumMatches; ++currMax)
                {
                    // transform the point to its corresponding point in the larger image
                    QPoint &currMaxLocation = locations[currMax];
                    currMaxLocation *= twoPowerNumDownPyrs;
                    currMaxLocation.setX(currMaxLocation.x() + targetSize.width / 2);
                    currMaxLocation.setY(currMaxLocation.y() + targetSize.height / 2);

                    const QPoint &searchPoint = locations.at(currMax);

                    // if we are searching for multiple targets and we have found a target or
                    //  multiple targets, we don't want to search in the same location(s) again
                    if(maximumMatches > 1 && !matchingPointList.isEmpty())
                    {
                        bool thisTargetFound = false;

                        for(int currPoint = 0; currPoint < matchingPointList.size(); currPoint++)
                        {
                            const QPoint &foundPoint = matchingPointList.at(currPoint).position;
                            if(std::abs(searchPoint.x() - foundPoint.x()) <= searchExpansion * 2 &&
                               std::abs(searchPoint.y() - foundPoint.y()) <= searchExpansion * 2)
                            {
                                thisTargetFound = true;
                                break;
                            }
                        }

                        // if the current target has been found, continue onto the next point
                        if(thisTargetFound)
                            continue;
                    }

                    // set the source image's ROI to slightly larger than the target image,
                    //  centred at the current point
                    cv::Rect searchRoi;
                    searchRoi.x = searchPoint.x() - (target.size().width) / 2 - searchExpansion;
                    searchRoi.y = searchPoint.y() - (target.size().height) / 2 - searchExpansion;
                    searchRoi.width = target.size().width + searchExpansion * 2;
                    searchRoi.height = target.size().height + searchExpansion * 2;

                    // make sure ROI doesn't extend outside of image
                    if(searchRoi.x < 0)
                        searchRoi.x = 0;

                    if(searchRoi.y < 0)
                        searchRoi.y = 0;

                    if((searchRoi.x + searchRoi.width) > (sourceSize.width - 1))
                    {
                        int numPixelsOver = (searchRoi.x + searchRoi.width) - (sourceSize.width - 1);

                        searchRoi.width -= numPixelsOver;
                    }

                    if((searchRoi.y + searchRoi.height) > (sourceSize.height - 1))
                    {
                        int numPixelsOver = (searchRoi.y + searchRoi.height) - (sourceSize.height - 1);

                        searchRoi.height -= numPixelsOver;
                    }

                    cv::Mat searchImage(source, searchRoi);

                    // perform the search on the large images
                    resultSize.width = searchRoi.width - target.size().width + 1;
                    resultSize.height = searchRoi.height - target.size().height + 1;

                    result = cv::Mat(resultSize, CV_32FC1);
                    cv::matchTemplate(searchImage, target, result, toOpenCVMethod(method));

                    // find the best match location
                    double minValue;
                    double maxValue;
                    cv::Point minLoc;
                    cv::Point maxLoc;

                    cv::minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc);

                    double &value = (method == SquaredDifferenceMethod) ? minValue : maxValue;
                    cv::Point &loc = (method == SquaredDifferenceMethod) ? minLoc : maxLoc;

                    value *= 100.0;

                    // transform point back to original image
                    loc.x += searchRoi.x + target.size().width / 2;
                    loc.y += searchRoi.y + target.size().height / 2;

                    if(method == SquaredDifferenceMethod)
                        value = 100.0f - value;

                    if(value >= matchPercentage)
                    {
                        // add the point to the list
                        matchingPointList.append(MatchingPoint(QPoint(loc.x, loc.y), value, sourceIndex));

                        // if we are only looking for a single target, we have found it, so we
                        //  can return
                        if(maximumMatches <= 1)
                            break;
                    }
                    else
                        break; // skip the rest
                }
            }
            catch(const cv::Exception &e)
            {
                mError = OpenCVException;
                mErrorString = tr("OpenCV exception: %1").arg(e.what());

                return MatchingPointList();
            }

            ++sourceIndex;
        }

		return matchingPointList;
	}

    QVector<QPoint> OpenCVAlgorithms::multipleMinMaxLoc(const cv::Mat &image, int maximumMatches, AlgorithmMethod method)
	{
		QVector<QPoint> locations(maximumMatches);
        QVector<float> matches(maximumMatches, (method == SquaredDifferenceMethod) ? std::numeric_limits<float>::max() : -std::numeric_limits<float>::max());
		cv::Size size = image.size();

		// extract the raw data for analysis
		for(int y = 0; y < size.height; ++y)
		{
			for(int x = 0; x < size.width; ++x)
			{
				float data = image.at<float>(y, x);

				// insert the data value into the array if it is greater than any of the
				//  other array values, and bump the other values below it, down
				for(int j = 0; j < maximumMatches; ++j)
				{
					// require at least 50% confidence on the sub-sampled image
					// in order to make this as fast as possible
                    if((method == SquaredDifferenceMethod && data < 0.5f && data < matches.at(j)) ||
                       (method != SquaredDifferenceMethod && data > 0.5f && data > matches.at(j)))
					{
						// move the maxima down
						for(int k = maximumMatches - 1; k > j; --k)
						{
                            matches[k] = matches.at(k-1);
							locations[k] = locations.at(k-1);
						}

						// insert the value
                        matches[j] = data;
						locations[j].setX(x);
						locations[j].setY(y);
						break;
					}
				}
			}
		}

		return locations;
	}

    QImage OpenCVAlgorithms::toQImage(const cv::Mat &image)
	{
		return QImage(image.data, image.size().width, image.size().height, image.step, QImage::Format_RGB888).rgbSwapped();
	}

    cv::Mat OpenCVAlgorithms::toCVMat(const QImage &image)
    {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, const_cast<uchar *>(image.bits()), image.bytesPerLine());
        cv::Mat back(mat.rows, mat.cols, CV_8UC3);
        int from_to[] = {0,0,  1,1,  2,2};

        cv::mixChannels(&mat, 1, &back, 1, from_to, 3);

        return back;
    }

    int OpenCVAlgorithms::toOpenCVMethod(OpenCVAlgorithms::AlgorithmMethod method)
    {
        switch(method)
        {
        default:
        case ActionTools::OpenCVAlgorithms::CorrelationCoefficientMethod:
            return CV_TM_CCOEFF_NORMED;
        case ActionTools::OpenCVAlgorithms::CrossCorrelationMethod:
            return CV_TM_CCORR_NORMED;
        case ActionTools::OpenCVAlgorithms::SquaredDifferenceMethod:
            return CV_TM_SQDIFF_NORMED;
        }
    }
}
