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

#include <QtGlobal>

#include "actiontools/opencvalgorithms.hpp"
#include "actiontools/opencvalgorithms_private.hpp"

#include <QtConcurrent/QtConcurrentRun>

namespace ActionTools
{
	OpenCVAlgorithms::OpenCVAlgorithms(QObject *parent)
		: QObject(parent),
          mPrivate(new OpenCVAlgorithmsPrivate()) // This should be replaced with make_unique once we add a c++14 requirement
	{
        qRegisterMetaType<MatchingPointList>("MatchingPointList");
    }

    OpenCVAlgorithms::~OpenCVAlgorithms() = default;

    bool OpenCVAlgorithms::findSubImageAsync(const QList<QImage> &sources,
                      const QImage &target,
                      int matchPercentage,
                      int maximumMatches,
                      int downPyrs,
                      int searchExpansion,
                      AlgorithmMethod method)
	{
        mPrivate->mError = NoError;
        mPrivate->mErrorString.clear();

        if(mPrivate->mFuture.isRunning())
		{
            mPrivate->mError = AlreadyRunningError;
            mPrivate->mErrorString = tr("FindSubImage is already running");

			return false;
		}

        QList<Mat> sourcesMat;
        sourcesMat.reserve(sources.size());

        for(const QImage &source: sources)
            sourcesMat.append(mPrivate->toCVMat(source));

        auto targetMat = mPrivate->toCVMat(target);

        if(!mPrivate->checkInputImages(sourcesMat, targetMat))
			return false;

        connect(&mPrivate->mFutureWatcher, &QFutureWatcher<MatchingPointList>::finished, this, &OpenCVAlgorithms::onFinished);

        mPrivate->mFuture = QtConcurrent::run(std::bind(&OpenCVAlgorithmsPrivate::fastMatchTemplate, mPrivate.get(), sourcesMat, targetMat, matchPercentage, maximumMatches, downPyrs, searchExpansion, method));
        mPrivate->mFutureWatcher.setFuture(mPrivate->mFuture);

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
        mPrivate->mError = NoError;
        mPrivate->mErrorString.clear();

        QList<Mat> sourcesMat;
        sourcesMat.reserve(sources.size());

        for(const QImage &source: sources)
            sourcesMat.append(mPrivate->toCVMat(source));

        auto targetMat = mPrivate->toCVMat(target);

        if(!mPrivate->checkInputImages(sourcesMat, targetMat))
			return false;

        matchingPoints = mPrivate->fastMatchTemplate(sourcesMat, targetMat, matchPercentage, maximumMatches, downPyrs, searchExpansion, method);

        return true;
    }

    void OpenCVAlgorithms::cancelSearch()
    {
        mPrivate->mFutureWatcher.cancel();
        mPrivate->mFutureWatcher.disconnect();
    }

    OpenCVAlgorithms::AlgorithmError OpenCVAlgorithms::error() const
    {
        return mPrivate->mError;
    }

    const QString &OpenCVAlgorithms::errorString() const
    {
        return mPrivate->mErrorString;
    }

    void OpenCVAlgorithms::onFinished()
    {
        emit finished(mPrivate->mFutureWatcher.result());
    }

    bool OpenCVAlgorithmsPrivate::checkInputImages(const QList<Mat> &sources, const Mat &target)
	{
        for(const auto &source: sources)
        {
            // make sure that the template image is smaller than the source
            if(target.size().width > source.size().width ||
               target.size().height > source.size().height)
            {
                mError = OpenCVAlgorithms::SourceImageSmallerThanTargerImageError;
                mErrorString = QObject::tr("Source images must be larger than target image");

                return false;
            }

            if(source.depth() != target.depth())
            {
                mError = OpenCVAlgorithms::NotSameDepthError;
                mErrorString = QObject::tr("Source images and target image must have same depth");

                return false;
            }

            if(source.channels() != target.channels())
            {
                mError = OpenCVAlgorithms::NotSameChannelCountError;
                mErrorString = QObject::tr("Source images and target image must have same number of channels");

                return false;
            }
        }

		return true;
	}

    MatchingPointList OpenCVAlgorithmsPrivate::fastMatchTemplate(const QList<Mat> &sources,
                                                                            const Mat &target,
																			int matchPercentage,
																			int maximumMatches,
																			int downPyrs,
                                                                            int searchExpansion,
                                                                            OpenCVAlgorithms::AlgorithmMethod method)
	{
        MatchingPointList matchingPointList;
        int sourceIndex = 0;

        for(const auto &source: sources)
        {
            try
            {
                // create copies of the images to modify
                auto copyOfSource = source.clone();
                auto copyOfTarget = target.clone();

                cv::Size sourceSize = source.size();
                cv::Size targetSize = target.size();

                // down pyramid the images
                for(int ii = 0; ii < downPyrs; ii++)
                {
                    // start with the source image
                    sourceSize.width  = (sourceSize.width  + 1) / 2;
                    sourceSize.height = (sourceSize.height + 1) / 2;

                    Mat smallSource(sourceSize, source.type());
                    cv::pyrDown(copyOfSource, smallSource);

                    // prepare for next loop, if any
                    copyOfSource = smallSource.clone();

                    // next, do the target
                    targetSize.width  = (targetSize.width  + 1) / 2;
                    targetSize.height = (targetSize.height + 1) / 2;

                    Mat smallTarget(targetSize, target.type());
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

                Mat result(resultSize, CV_32FC1);
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

                        for(auto matchingPoint: matchingPointList)
                        {
                            const QPoint &foundPoint = matchingPoint.position;
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

                    Mat searchImage(source, searchRoi);

                    // perform the search on the large images
                    resultSize.width = searchRoi.width - target.size().width + 1;
                    resultSize.height = searchRoi.height - target.size().height + 1;

                    result = Mat(resultSize, CV_32FC1);
                    cv::matchTemplate(searchImage, target, result, toOpenCVMethod(method));

                    // find the best match location
                    double minValue;
                    double maxValue;
                    cv::Point minLoc;
                    cv::Point maxLoc;

                    cv::minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc);

                    double &value = (method == OpenCVAlgorithms::SquaredDifferenceMethod) ? minValue : maxValue;
                    cv::Point &loc = (method == OpenCVAlgorithms::SquaredDifferenceMethod) ? minLoc : maxLoc;

                    value *= 100.0;

                    // transform point back to original image
                    loc.x += searchRoi.x + target.size().width / 2;
                    loc.y += searchRoi.y + target.size().height / 2;

                    if(method == OpenCVAlgorithms::SquaredDifferenceMethod)
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
                mError = OpenCVAlgorithms::OpenCVException;
                mErrorString = QObject::tr("OpenCV exception: %1").arg(QLatin1String(e.what()));

                return MatchingPointList();
            }

            ++sourceIndex;
        }

		return matchingPointList;
	}

    QVector<QPoint> OpenCVAlgorithmsPrivate::multipleMinMaxLoc(const Mat &image, int maximumMatches, OpenCVAlgorithms::AlgorithmMethod method)
	{
		QVector<QPoint> locations(maximumMatches);
        QVector<float> matches(maximumMatches, (method == OpenCVAlgorithms::SquaredDifferenceMethod) ? std::numeric_limits<float>::max() : -std::numeric_limits<float>::max());
		cv::Size size = image.size();

#if CV_MAJOR_VERSION == 2
        const Mat &matImage = image;
#elif CV_MAJOR_VERSION >= 3
        cv::Mat matImage = image.getMat(cv::ACCESS_READ);
#endif

		// extract the raw data for analysis
		for(int y = 0; y < size.height; ++y)
		{
			for(int x = 0; x < size.width; ++x)
			{
                float data = matImage.at<float>(y, x);

				// insert the data value into the array if it is greater than any of the
				//  other array values, and bump the other values below it, down
				for(int j = 0; j < maximumMatches; ++j)
				{
					// require at least 50% confidence on the sub-sampled image
					// in order to make this as fast as possible
                    if((method == OpenCVAlgorithms::SquaredDifferenceMethod && data < 0.5f && data < matches.at(j)) ||
                       (method != OpenCVAlgorithms::SquaredDifferenceMethod && data > 0.5f && data > matches.at(j)))
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

    QImage OpenCVAlgorithmsPrivate::toQImage(const Mat &image)
	{
#if CV_MAJOR_VERSION == 2
        const cv::Mat &matImage = image;
#elif CV_MAJOR_VERSION >= 3
        cv::Mat matImage = image.getMat(cv::ACCESS_READ);
#endif

        return QImage(matImage.data, image.size().width, image.size().height, image.step, QImage::Format_RGB888).rgbSwapped();
	}

    QImage prepareImageForOpenCV(const QImage &image)
    {
        // Note: this is incomplete and doesn't take into account many other potential issues, but for now I prefer adding this fix rather than risking adding more instability.
        if(image.format() != QImage::Format_Indexed8)
            return image;

        return image.convertToFormat(QImage::Format_RGB32);
    }

    Mat OpenCVAlgorithmsPrivate::toCVMat(const QImage &image)
    {
        if(image.isNull() || image.format() == QImage::Format_Invalid)
            return {};

        auto preparedImage = prepareImageForOpenCV(image);
        cv::Mat mat(preparedImage.height(), preparedImage.width(), CV_8UC4, const_cast<uchar *>(preparedImage.bits()), preparedImage.bytesPerLine());
        cv::Mat back(mat.rows, mat.cols, CV_8UC3);
        int from_to[] = {0,0,  1,1,  2,2};

        cv::mixChannels(&mat, 1, &back, 1, from_to, 3);

#if CV_MAJOR_VERSION == 2
        return back;
#elif CV_MAJOR_VERSION >= 3
        return back.getUMat(cv::ACCESS_RW).clone();
#endif
    }

    int OpenCVAlgorithmsPrivate::toOpenCVMethod(OpenCVAlgorithms::AlgorithmMethod method)
    {
        switch(method)
        {
        default:
#if CV_MAJOR_VERSION < 4
        case ActionTools::OpenCVAlgorithms::CorrelationCoefficientMethod:
            return CV_TM_CCOEFF_NORMED;
        case ActionTools::OpenCVAlgorithms::CrossCorrelationMethod:
            return CV_TM_CCORR_NORMED;
        case ActionTools::OpenCVAlgorithms::SquaredDifferenceMethod:
            return CV_TM_SQDIFF_NORMED;
#else
        case ActionTools::OpenCVAlgorithms::CorrelationCoefficientMethod:
            return cv::TM_CCOEFF_NORMED;
        case ActionTools::OpenCVAlgorithms::CrossCorrelationMethod:
            return cv::TM_CCORR_NORMED;
        case ActionTools::OpenCVAlgorithms::SquaredDifferenceMethod:
            return cv::TM_SQDIFF_NORMED;
#endif

        }
    }
}
