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

#include "actiontools/code/image.hpp"
#include "actiontools/code/rawdata.hpp"
#include "actiontools/code/color.hpp"
#include "actiontools/code/size.hpp"
#include "actiontools/code/rect.hpp"
#include "actiontools/code/point.hpp"
#include "actiontools/code/window.hpp"
#include "actiontools/opencvalgorithms.hpp"
#include "actiontools/qtimagefilters/QtImageFilterFactory"
#include "actiontools/screenshooter.hpp"
#include "actiontools/scriptengine.hpp"

#include <QBuffer>
#include <QJSValueIterator>
#include <QPixmap>
#include <QApplication>
#include <QScreen>

#include <algorithm>

namespace Code
{
	const QString Image::filterNames[] =
	{
		QStringLiteral("ConvolutionFilter"),
		QStringLiteral("GaussianBlur"),
		QStringLiteral("Defocus"),
		QStringLiteral("Highlight"),
		QStringLiteral("Sharpen"),
		QStringLiteral("SharpenMore"),
		QStringLiteral("SharpenEvenMore"),
		QStringLiteral("EdgeDetect"),
		QStringLiteral("BigEdge"),
		QStringLiteral("Emboss"),
		QStringLiteral("EmbossColor"),
		QStringLiteral("Negative"),
		QStringLiteral("RemoveChannel"),
		QStringLiteral("Punch")
	};
	
    const QStringList Image::filterOptionsNames =
    {
        QStringLiteral("filterChannels"),
        QStringLiteral("filterBorderPolicy"),
        QStringLiteral("convolutionDivisor"),
        QStringLiteral("convolutionBias"),
        QStringLiteral(""),
        QStringLiteral("radius"),
        QStringLiteral("force"),
        QStringLiteral("center")
    };
	
	Image::Image()
		: CodeClass(),
          mOpenCVAlgorithms(new ActionTools::OpenCVAlgorithms(this))
	{
        connect(mOpenCVAlgorithms, &ActionTools::OpenCVAlgorithms::finished, this, &Image::findSubImageAsyncFinished);
	}
	
	Image::Image(const Image &other)
		: CodeClass(),
		  mImage(other.mImage),
		  mOpenCVAlgorithms(new ActionTools::OpenCVAlgorithms(this)),
		  mFindSubImageSearchForOne(false)
	{
        connect(mOpenCVAlgorithms, &ActionTools::OpenCVAlgorithms::finished, this, &Image::findSubImageAsyncFinished);
	}
	
	Image::Image(const QImage &image)
		: CodeClass(),
		  mImage(image),
		  mOpenCVAlgorithms(new ActionTools::OpenCVAlgorithms(this)),
		  mFindSubImageSearchForOne(false)
	{
        connect(mOpenCVAlgorithms, &ActionTools::OpenCVAlgorithms::finished, this, &Image::findSubImageAsyncFinished);
	}

	Image::Image(const QString &filename)
		: CodeClass(),
		  mImage(filename),
		  mOpenCVAlgorithms(new ActionTools::OpenCVAlgorithms(this)),
		  mFindSubImageSearchForOne(false)
    {
        if(mImage.isNull())
            throwError(QStringLiteral("LoadImageError"), tr("Unable to load image from file %1").arg(filename));

        connect(mOpenCVAlgorithms, &ActionTools::OpenCVAlgorithms::finished, this, &Image::findSubImageAsyncFinished);
    }

	Image &Image::operator=(Image other)
	{
		swap(other);
		
		return *this;
	}
	
	Image &Image::operator=(QImage image)
	{
		swap(image);
		
		return *this;
	}
	
	void Image::swap(Image &other)
	{
		std::swap(mImage, other.mImage);
	}
	
	void Image::swap(QImage &image)
	{
		std::swap(mImage, image);
	}
			
	const QImage &Image::image() const
	{
		return mImage;
	}
	
	QJSValue Image::clone() const
	{
        return CodeClass::clone<Image>();
	}
	
	bool Image::equals(const QJSValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(auto otherImage = qobject_cast<Image*>(object))
			return (otherImage == this || otherImage->mImage == mImage);
			
		return false;
	}

	QString Image::toString() const
	{
		return QStringLiteral("Image {width: %1, height: %2}").arg(width()).arg(height());
	}
	
    Image *Image::setData(const QJSValue &data)
	{
		QObject *object = data.toQObject();
		if(auto codeRawData = qobject_cast<RawData*>(object))
		{
			if(!mImage.loadFromData(codeRawData->byteArray()))
			{
				throwError(QStringLiteral("ImageDataError"), tr("Unable to set the image data"));
                return this;
			}
		}
		else
			mImage = data.toVariant().value<QImage>();
		
        return this;
	}
	
    QJSValue Image::data(const QString &format) const
	{
        QBuffer dataBuffer;
		dataBuffer.open(QIODevice::WriteOnly);
		
		if(!mImage.save(&dataBuffer, format.toLatin1().constData()))
		{
            throwError(QStringLiteral("ImageDataError"), tr("Unable to get the image data"));
            return {};
        }

        return CodeClass::construct<RawData>(dataBuffer.buffer());
	}
	
    Image *Image::loadFromFile(const QString &filename)
	{
		if(!mImage.load(filename))
		{
			throwError(QStringLiteral("LoadImageError"), tr("Unable to load image from file %1").arg(filename));
            return this;
		}
	
        return this;
	}
	
    Image *Image::saveToFile(const QString &filename)
	{
		if(!mImage.save(filename))
		{
			throwError(QStringLiteral("SaveImageError"), tr("Unable to save image to file %1").arg(filename));
            return this;
		}
	
        return this;
	}
	
    Image *Image::applyFilter(Filter filter, const QJSValue &options)
	{
		QtImageFilter *imageFilter = QtImageFilterFactory::createImageFilter(filterNames[filter]);
		if(!imageFilter)
		{
			throwError(QStringLiteral("ApplyFilterError"), tr("Unable to apply filter"));
            return this;
		}
		
		QJSValueIterator it(options);
		while(it.hasNext())
		{
			it.next();
			
			int optionIndex = filterOptionsNames.indexOf(it.name());
			if(optionIndex == -1)
				continue;
			
			auto option = static_cast<QtImageFilter::FilterOption>(optionIndex + 1);
			QVariant value;

			switch(option)
			{
			case QtImageFilter::FilterChannels:
			case QtImageFilter::FilterBorderPolicy:
				value = it.value().toString();
				break;
			case QtImageFilter::ConvolutionDivisor:
			case QtImageFilter::ConvolutionBias:
			case QtImageFilter::Radius:
			case QtImageFilter::Force:
				value = it.value().toNumber();
				break;
			case QtImageFilter::Center:
				value = QPointF(it.value().property(QStringLiteral("x")).toNumber(), it.value().property(QStringLiteral("y")).toNumber());
				break;
			default:
				break;
			}
			
			if(!imageFilter->setOption(option, value))
			{
				throwError(QStringLiteral("ApplyFilterError"), tr("Cannot set filter option %1 %2").arg(it.name()).arg(value.toString()));
                return this;
			}
		}
		
		mImage = imageFilter->apply(mImage);
		
        return this;
	}
	
	QJSValue Image::pixel(int x, int y) const
	{
        return CodeClass::construct<Color>(mImage.pixel(x, y));
	}

    Image *Image::setPixel(int x, int y, const Color *color)
    {
        mImage.setPixel(x, y, color->color().rgba());

        return this;
    }

    Image *Image::setPixel(int x, int y, const QString &colorString)
    {
        mImage.setPixel(x, y, QColor(colorString).rgba());

        return this;
    }

    Image *Image::setPixel(int x, int y, int red, int green, int blue, int alpha)
    {
        mImage.setPixel(x, y, QColor(red, green, blue, alpha).rgba());

        return this;
    }

    QJSValue Image::pixels() const
    {
        if(mImage.isNull())
            return QJSValue();

        QImage argbImage = mImage.convertToFormat(QImage::Format_ARGB32);
        int pixelCount = argbImage.width() * argbImage.height();
        QJSValue pixelArray = ActionTools::ScriptEngine::current()->newArray(pixelCount);
        const QRgb *pixelData = reinterpret_cast<const QRgb *>(argbImage.constBits());

        for(int pixelIndex = 0; pixelIndex < pixelCount; ++pixelIndex)
            pixelArray.setProperty(pixelIndex, CodeClass::construct<Color>(QColor(pixelData[pixelIndex])));

        return pixelArray;
    }

    QJSValue Image::pixelData() const
    {
        if(mImage.isNull())
            return QJSValue();

        QImage argbImage = mImage.convertToFormat(QImage::Format_ARGB32);
        int pixelCount = argbImage.width() * argbImage.height();
        QJSValue pixelArray = ActionTools::ScriptEngine::current()->newArray(pixelCount * 4);
        const QRgb *pixelData = reinterpret_cast<const QRgb *>(argbImage.constBits());

        for(int pixelIndex = 0; pixelIndex < pixelCount; ++pixelIndex)
        {
            QRgb pixel = pixelData[pixelIndex];

            pixelArray.setProperty(pixelIndex * 4 + 0, qRed(pixel) / 255.0f);
            pixelArray.setProperty(pixelIndex * 4 + 1, qGreen(pixel) / 255.0f);
            pixelArray.setProperty(pixelIndex * 4 + 2, qBlue(pixel) / 255.0f);
            pixelArray.setProperty(pixelIndex * 4 + 3, qAlpha(pixel) / 255.0f);
        }

        return pixelArray;
    }
	
    Image *Image::mirror(MirrorOrientation mirrorOrientation)
	{
		mImage = mImage.mirrored(mirrorOrientation & Horizontal, mirrorOrientation & Vertical);
		
        return this;
	}
	
    Image *Image::setSize(const Size *size)
    {
        mImage = mImage.scaled(size->size());
		
        return this;
	}

	QJSValue Image::size() const
	{
        return CodeClass::construct<Size>(mImage.size());
	}
	
	int Image::width() const
	{
		return mImage.width();
	}
	
	int Image::height() const
	{
		return mImage.height();
	}

	QJSValue Image::copy() const
	{
        return CodeClass::construct<Image>(mImage);
	}

    QJSValue Image::copy(const Rect *rect) const
    {
        return CodeClass::construct<Image>(mImage.copy(rect->rect()));
    }

	QJSValue Image::findSubImage(const QJSValue &otherImage, const QJSValue &options) const
	{
		if(auto codeImage = qobject_cast<Image*>(otherImage.toQObject()))
		{
			ActionTools::MatchingPointList matchingPointList;

			int confidenceMinimum;
			int downPyramidCount;
			int searchExpansion;
            AlgorithmMethod method;

            if(!findSubImageOptions(options, &confidenceMinimum, &downPyramidCount, &searchExpansion, &method))
                return QJSValue();

            if(!mOpenCVAlgorithms->findSubImage(QList<QImage>() << mImage, codeImage->image(), matchingPointList, confidenceMinimum, 1, downPyramidCount, searchExpansion, static_cast<ActionTools::OpenCVAlgorithms::AlgorithmMethod>(method)))
			{
				throwError(QStringLiteral("FindSubImageError"), tr("Error while searching for a sub-image: %1").arg(mOpenCVAlgorithms->errorString()));
				return QJSValue();
			}

			if(matchingPointList.isEmpty())
				return QJSValue();

			const ActionTools::MatchingPoint &matchingPoint = matchingPointList.first();
            QJSValue back = ActionTools::ScriptEngine::current()->newObject();

            back.setProperty(QStringLiteral("position"), CodeClass::construct<Point>(matchingPoint.position));
			back.setProperty(QStringLiteral("confidence"), matchingPoint.confidence);

			return back;
		}
		else
		{
			throwError(QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			return QJSValue();
		}
	}

	bool matchingPointGreaterThan(const ActionTools::MatchingPoint &matchingPoint1, const ActionTools::MatchingPoint &matchingPoint2)
	{
        return matchingPoint1.confidence > matchingPoint2.confidence;
	}

	QJSValue Image::findSubImages(const QJSValue &otherImage, const QJSValue &options) const
	{
		if(auto codeImage = qobject_cast<Image*>(otherImage.toQObject()))
		{
			ActionTools::MatchingPointList matchingPointList;

			int confidenceMinimum;
			int downPyramidCount;
			int searchExpansion;
            AlgorithmMethod method;
			int maximumMatches;

            if(!findSubImageOptions(options, &confidenceMinimum, &downPyramidCount, &searchExpansion, &method, &maximumMatches))
                return QJSValue();

            if(!mOpenCVAlgorithms->findSubImage(QList<QImage>() << mImage, codeImage->image(), matchingPointList, confidenceMinimum, maximumMatches, downPyramidCount, searchExpansion, static_cast<ActionTools::OpenCVAlgorithms::AlgorithmMethod>(method)))
			{
				throwError(QStringLiteral("FindSubImageError"), tr("Error while searching for a sub-image: %1").arg(mOpenCVAlgorithms->errorString()));
				return QJSValue();
			}

			if(matchingPointList.isEmpty())
				return QJSValue();

            std::sort(matchingPointList.begin(), matchingPointList.end(), matchingPointGreaterThan);

            ActionTools::MatchingPointList::ConstIterator matchingPointIt = matchingPointList.constBegin();
            QJSValue back = ActionTools::ScriptEngine::current()->newArray(matchingPointList.size());

			int index = 0;
			while(matchingPointIt != matchingPointList.constEnd())
			{
                QJSValue object = ActionTools::ScriptEngine::current()->newObject();

                object.setProperty(QStringLiteral("position"), CodeClass::construct<Point>(matchingPointIt->position));
				object.setProperty(QStringLiteral("confidence"), matchingPointIt->confidence);

				back.setProperty(index, object);

				++matchingPointIt;
				++index;
			}

			return back;
		}
		else
		{
			throwError(QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			return QJSValue();
		}
	}

    Image *Image::findSubImageAsync(const QJSValue &otherImage, const QJSValue &callback, const QJSValue &options)
	{
		mFindSubImageSearchForOne = true;

        if(!callback.isCallable())
		{
			throwError(QStringLiteral("FindSubImageError"), tr("Parameter \"callback\" is not a function"));
            return this;
		}

		if(auto codeImage = qobject_cast<Image*>(otherImage.toQObject()))
		{
			int confidenceMinimum;
			int downPyramidCount;
			int searchExpansion;
            AlgorithmMethod method;

            if(!findSubImageOptions(options, &confidenceMinimum, &downPyramidCount, &searchExpansion, &method))
                return this;

            if(!mOpenCVAlgorithms->findSubImageAsync(QList<QImage>() << mImage, codeImage->image(), confidenceMinimum, 1, downPyramidCount, searchExpansion, static_cast<ActionTools::OpenCVAlgorithms::AlgorithmMethod>(method)))
			{
				throwError(QStringLiteral("FindSubImageError"), tr("Error while searching for a sub-image: %1").arg(mOpenCVAlgorithms->errorString()));
                return this;
			}

			mFindSubImageAsyncFunction = callback;

            return this;
		}
		else
		{
			throwError(QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
            return this;
		}
	}

    Image *Image::findSubImagesAsync(const QJSValue &otherImage, const QJSValue &callback, const QJSValue &options)
	{
		mFindSubImageSearchForOne = false;

        if(!callback.isCallable())
		{
			throwError(QStringLiteral("FindSubImageError"), tr("Parameter \"callback\" is not a function"));
            return this;
		}

		if(auto codeImage = qobject_cast<Image*>(otherImage.toQObject()))
		{
			int confidenceMinimum;
			int downPyramidCount;
			int searchExpansion;
            AlgorithmMethod method;
			int maximumMatches;

            if(!findSubImageOptions(options, &confidenceMinimum, &downPyramidCount, &searchExpansion, &method, &maximumMatches))
                return this;

            if(!mOpenCVAlgorithms->findSubImageAsync(QList<QImage>() << mImage, codeImage->image(), confidenceMinimum, maximumMatches, downPyramidCount, searchExpansion, static_cast<ActionTools::OpenCVAlgorithms::AlgorithmMethod>(method)))
			{
				throwError(QStringLiteral("FindSubImageError"), tr("Error while searching for a sub-image: %1").arg(mOpenCVAlgorithms->errorString()));
                return this;
			}

			mFindSubImageAsyncFunction = callback;

            return this;
		}
		else
		{
			throwError(QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
            return this;
		}
	}

    void Image::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Image*>("const Image *");

        CodeClass::registerClassWithStaticFunctions<Image, StaticImage>(
            QStringLiteral("Image"),
            {QStringLiteral("takeScreenshot"), QStringLiteral("takeScreenshotUsingScreenIndex")},
            scriptEngine
        );
    }

	void Image::findSubImageAsyncFinished(const ActionTools::MatchingPointList &matchingPointList)
	{
        if(mFindSubImageAsyncFunction.isUndefined())
            return;

        if(matchingPointList.isEmpty())
        {
            mFindSubImageAsyncFunction.call();

            return;
        }

        if(mFindSubImageSearchForOne)
        {
            const ActionTools::MatchingPoint &matchingPoint = matchingPointList.first();
            QJSValue back = ActionTools::ScriptEngine::current()->newObject();

            back.setProperty(QStringLiteral("position"), CodeClass::construct<Point>(matchingPoint.position));
            back.setProperty(QStringLiteral("confidence"), matchingPoint.confidence);

            mFindSubImageAsyncFunction.call({back});
        }
        else
        {
            ActionTools::MatchingPointList matchingPointListCopy(matchingPointList);
            std::sort(matchingPointListCopy.begin(), matchingPointListCopy.end(), matchingPointGreaterThan);

            ActionTools::MatchingPointList::ConstIterator matchingPointIt = matchingPointListCopy.constBegin();
            QJSValue back = ActionTools::ScriptEngine::current()->newArray(matchingPointListCopy.size());

            int index = 0;
            while(matchingPointIt != matchingPointListCopy.constEnd())
            {
                QJSValue object = ActionTools::ScriptEngine::current()->newObject();

                object.setProperty(QStringLiteral("position"), CodeClass::construct<Point>(matchingPointIt->position));
                object.setProperty(QStringLiteral("confidence"), matchingPointIt->confidence);

                back.setProperty(index, object);

                ++matchingPointIt;
                ++index;
            }

            mFindSubImageAsyncFunction.call({back});
        }
	}

    bool Image::findSubImageOptions(const QJSValue &options, int *confidenceMinimum, int *downPyramidCount, int *searchExpansion, AlgorithmMethod *method, int *maximumMatches) const
	{
        if(!options.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("options has to be an object"));
            return false;
        }

		QJSValueIterator it(options);

		if(confidenceMinimum)
			*confidenceMinimum = 70;

		if(maximumMatches)
			*maximumMatches = 10;

		if(downPyramidCount)
			*downPyramidCount = 2;

		if(searchExpansion)
			*searchExpansion = 15;

        if(method)
            *method = CorrelationCoefficient;

		while(it.hasNext())
		{
			it.next();

			if(confidenceMinimum && it.name() == QStringLiteral("confidenceMinimum"))
                *confidenceMinimum = it.value().toInt();
			else if(maximumMatches && it.name() == QStringLiteral("maximumMatches"))
                *maximumMatches = it.value().toInt();
			else if(downPyramidCount && it.name() == QStringLiteral("downPyramidCount"))
                *downPyramidCount = it.value().toInt();
			else if(searchExpansion && it.name() == QStringLiteral("searchExpansion"))
                *searchExpansion = it.value().toInt();
			else if(searchExpansion && it.name() == QStringLiteral("method"))
                *method = static_cast<AlgorithmMethod>(it.value().toInt());
		}

        return true;
	}

    QJSValue StaticImage::takeScreenshot()
    {
        return CodeClass::construct<Image>(ActionTools::ScreenShooter::captureAllScreens().toImage());
    }

    QJSValue StaticImage::takeScreenshot(const Window *window)
    {
        WId windowId = window->windowHandle().value();
        auto pos = window->windowHandle().rect().topLeft();

        auto screen = QGuiApplication::screenAt(pos);
        if(!screen)
            return {};

        auto geometry = screen->geometry();

        return CodeClass::construct<Image>(screen->grabWindow(windowId).toImage());
    }

    QJSValue StaticImage::takeScreenshotUsingScreenIndex(int screenIndex)
    {
        auto screens = QGuiApplication::screens();

        if(screenIndex < 0 || screenIndex >= screens.size())
        {
            throwError(QStringLiteral("InvalidScreenIndexError"), tr("Invalid screen index"));
            return {};
        }

        QPixmap screenPixmap = screens[screenIndex]->grabWindow(0);

        return CodeClass::construct<Image>(screenPixmap.toImage());
    }
}
