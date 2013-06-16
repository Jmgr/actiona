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

#ifndef IMAGE_H
#define IMAGE_H

#include "actiontools_global.h"
#include "qtimagefilters/QtImageFilter"
#include "code/codeclass.h"
#include "matchingpointlist.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QImage>

namespace ActionTools
{
	class OpenCVAlgorithms;
}

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Image : public CodeClass
	{
		Q_OBJECT
		Q_ENUMS(Filter)
		Q_ENUMS(MirrorOrientation)
        Q_ENUMS(AlgorithmMethod)
		
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
		enum MirrorOrientation
		{
			Vertical = 1,
			Horizontal = 2
		};
        enum AlgorithmMethod
        {
            CorrelationCoefficient,
            CrossCorrelation,
            SquaredDifference
        };
		
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue constructor(const QImage &image, QScriptEngine *engine);

		static QScriptValue takeScreenshot(QScriptContext *context, QScriptEngine *engine);
        static QScriptValue takeScreenshotUsingScreenIndex(QScriptContext *context, QScriptEngine *engine);

		static void registerClass(QScriptEngine *scriptEngine);
		
		Image();
		Image(const Image &other);
		Image(const QImage &image);
		Image(const QString &filename);

		Image &operator=(Image other);
		Image &operator=(QImage image);
		
		void swap(Image &other);
		void swap(QImage &image);
		
		const QImage &image() const;

        virtual int additionalMemoryCost() const { return mImage.byteCount(); }
	
	public slots:
		QScriptValue clone() const;
		bool equals(const QScriptValue &other) const;
		QString toString() const;
		QScriptValue setData(const QScriptValue &data);
		QScriptValue data() const;
		QScriptValue loadFromFile(const QString &filename);
		QScriptValue saveToFile(const QString &filename) const;
		QScriptValue applyFilter(Filter filter, const QScriptValue &options = QScriptValue());
		QScriptValue pixel(int x, int y) const;
		QScriptValue setPixel(int x, int y, const QScriptValue &color);
		QScriptValue mirror(MirrorOrientation mirrorOrientation);
		QScriptValue setSize();
		QScriptValue size() const;
		int width() const;
		int height() const;
		QScriptValue copy() const;
		QScriptValue findSubImage(const QScriptValue &otherImage, const QScriptValue &options = QScriptValue()) const;
		QScriptValue findSubImages(const QScriptValue &otherImage, const QScriptValue &options = QScriptValue()) const;
		QScriptValue findSubImageAsync(const QScriptValue &otherImage, const QScriptValue &callback, const QScriptValue &options = QScriptValue());
		QScriptValue findSubImagesAsync(const QScriptValue &otherImage, const QScriptValue &callback, const QScriptValue &options = QScriptValue());

	private slots:
		void findSubImageAsyncFinished(const ActionTools::MatchingPointList &matchingPointList);

	private:
        void findSubImageOptions(const QScriptValue &options, int *confidenceMinimum, int *downPyramidCount, int *searchExpansion, AlgorithmMethod *method, int *maximumMatches = 0) const;

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
		QScriptValue mFindSubImageAsyncFunction;
		bool mFindSubImageSearchForOne;
	};
}

#endif // IMAGE_H
