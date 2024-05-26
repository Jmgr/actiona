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

#pragma once

#include "actiontools/actiontools_global.hpp"
#include "actiontools/qtimagefilters/QtImageFilter"
#include "actiontools/code/codeclass.hpp"
#include "actiontools/matchingpointlist.hpp"

#include <QJSValue>
#include <QImage>

namespace ActionTools
{
	class OpenCVAlgorithms;
}

namespace Code
{
    class Size;
    class Window;
    class Rect;
    class Color;

	class ACTIONTOOLSSHARED_EXPORT Image : public CodeClass
	{
		Q_OBJECT
		
	public:
		enum Filter
		{
			ConvolutionFilter,
			GaussianBlur,
			Defocus,
			Highlight,
			Sharpen,
			SharpenMore,
			SharpenEvenMore,
			EdgeDetect,
			BigEdge,
			Emboss,
			EmbossColor,
			Negative,
			RemoveChannel,
			Punch
		};
        Q_ENUM(Filter)
		enum MirrorOrientation
		{
			Vertical = 1,
			Horizontal = 2
		};
        Q_ENUM(MirrorOrientation)
        enum AlgorithmMethod
        {
            CorrelationCoefficient,
            CrossCorrelation,
            SquaredDifference
        };
        Q_ENUM(AlgorithmMethod)

        Q_INVOKABLE Image();
        Q_INVOKABLE Image(const Image &other);
		Image(const QImage &image);
        Q_INVOKABLE Image(const QString &filename);

		Image &operator=(Image other);
		Image &operator=(QImage image);
		
		void swap(Image &other);
		void swap(QImage &image);
		
		const QImage &image() const;

        Q_INVOKABLE QJSValue clone() const;
        Q_INVOKABLE bool equals(const QJSValue &other) const;
        Q_INVOKABLE QString toString() const override;
        Q_INVOKABLE Image *setData(const QJSValue &data);
        Q_INVOKABLE QJSValue data(const QString &format = QStringLiteral("BMP")) const;
        Q_INVOKABLE Image *loadFromFile(const QString &filename);
        Q_INVOKABLE Image *saveToFile(const QString &filename);
        Q_INVOKABLE Image *applyFilter(Filter filter, const QJSValue &options = QJSValue());
        Q_INVOKABLE QJSValue pixel(int x, int y) const;
        Q_INVOKABLE Image *setPixel(int x, int y, const Color *color);
        Q_INVOKABLE Image *setPixel(int x, int y, const QString &colorString);
        Q_INVOKABLE Image *setPixel(int x, int y, int red, int green, int blue, int alpha = 255);
        Q_INVOKABLE QJSValue pixels() const;
        Q_INVOKABLE QJSValue pixelData() const;
        Q_INVOKABLE Image *mirror(MirrorOrientation mirrorOrientation);
        Q_INVOKABLE Image *setSize(const Size *size);
        Q_INVOKABLE QJSValue size() const;
        Q_INVOKABLE int width() const;
        Q_INVOKABLE int height() const;
        Q_INVOKABLE QJSValue copy() const;
        Q_INVOKABLE QJSValue copy(const Rect *rect) const;
        Q_INVOKABLE QJSValue findSubImage(const QJSValue &otherImage, const QJSValue &options = QJSValue()) const;
        Q_INVOKABLE QJSValue findSubImages(const QJSValue &otherImage, const QJSValue &options = QJSValue()) const;
        Q_INVOKABLE Image *findSubImageAsync(const QJSValue &otherImage, const QJSValue &callback, const QJSValue &options = QJSValue());
        Q_INVOKABLE Image *findSubImagesAsync(const QJSValue &otherImage, const QJSValue &callback, const QJSValue &options = QJSValue());

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

	private slots:
		void findSubImageAsyncFinished(const ActionTools::MatchingPointList &matchingPointList);

	private:
        bool findSubImageOptions(const QJSValue &options, int *confidenceMinimum, int *downPyramidCount, int *searchExpansion, AlgorithmMethod *method, int *maximumMatches = nullptr) const;

		enum FilterOption
		{
			FilterChannels = QtImageFilter::FilterChannels,
			FilterBorderPolicy = QtImageFilter::FilterBorderPolicy,
			ConvolutionDivisor = QtImageFilter::ConvolutionDivisor,
			ConvolutionBias = QtImageFilter::ConvolutionBias,
			Radius = QtImageFilter::Radius,
			Force = QtImageFilter::Force,
			Center = QtImageFilter::Center
		};
		
		static const QString filterNames[];
		static const QStringList filterOptionsNames;
		
		QImage mImage;
		ActionTools::OpenCVAlgorithms *mOpenCVAlgorithms;
		QJSValue mFindSubImageAsyncFunction;
		bool mFindSubImageSearchForOne{false};
	};

    class ACTIONTOOLSSHARED_EXPORT StaticImage : public CodeClass
    {
        Q_OBJECT

    public:
        StaticImage(QObject *parent): CodeClass(parent) {}

        Q_INVOKABLE QString toString() const override { return QStringLiteral("StaticImage"); }
        Q_INVOKABLE QJSValue takeScreenshot();
        Q_INVOKABLE QJSValue takeScreenshot(const Window *window);
        Q_INVOKABLE QJSValue takeScreenshotUsingScreenIndex(int screenIndex);
    };
}

