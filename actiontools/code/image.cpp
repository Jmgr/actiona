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

#include "image.h"
#include "rawdata.h"
#include "color.h"
#include "size.h"
#include "rect.h"
#include "point.h"
#include "window.h"
#include "codetools.h"
#include "opencvalgorithms.h"
#include "qtimagefilters/QtImageFilterFactory"
#include "screenshooter.h"

#include <QBuffer>
#include <QScriptValueIterator>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>

#include <algorithm>

namespace Code
{
	QScriptValue Image::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Image *image = nullptr;
		
		switch(context->argumentCount())
		{
		case 0:
			image = new Image;
			break;
		case 1:
			{
				const QScriptValue &argument = context->argument(0);

				QObject *object = argument.toQObject();
				if(auto codeImage = qobject_cast<Image*>(object))
					image = new Image(*codeImage);
				else if(argument.isString())
				{
					image = new Image(argument.toString());

					if(image->image().isNull())
						throwError(context, engine, QStringLiteral("LoadImageError"), tr("Unable to load image from file %1").arg(argument.toString()));
				}
				else
					throwError(context, engine, QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			}
			break;
		default:
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			break;
		}
		
		if(!image)
			return engine->undefinedValue();

		return CodeClass::constructor(image, context, engine);
	}

	QScriptValue Image::constructor(const QImage &image, QScriptEngine *engine)
	{
        return CodeClass::constructor(new Image(image), engine);
	}

	QScriptValue Image::takeScreenshot(QScriptContext *context, QScriptEngine *engine)
	{
		if(context->argumentCount() > 0)
		{
            WId windowId;

			if(auto window = qobject_cast<Window *>(context->argument(0).toQObject()))
				windowId = window->windowHandle().value();
			else
			{
#ifdef Q_OS_WIN
                throwError(context, engine, QStringLiteral("InvalidWindowError"), tr("Invalid window"));
				return engine->undefinedValue();
#else
				windowId = context->argument(0).toInt32();
#endif
			}

            return constructor(QGuiApplication::primaryScreen()->grabWindow(windowId).toImage(), engine);
		}

        return constructor(ActionTools::ScreenShooter::captureAllScreens().toImage(), engine);
    }

    QScriptValue Image::takeScreenshotUsingScreenIndex(QScriptContext *context, QScriptEngine *engine)
    {
        if(context->argumentCount() == 0)
        {
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
            return engine->undefinedValue();
        }

        int screenIndex = context->argument(0).toInt32();
        QDesktopWidget *desktop = QApplication::desktop();

        if(screenIndex < 0 || screenIndex >= desktop->screenCount())
        {
			throwError(context, engine, QStringLiteral("InvalidScreenIndexError"), tr("Invalid screen index"));
            return engine->undefinedValue();
        }

        QRect screenGeometry = desktop->screenGeometry(screenIndex);
        QPixmap screenPixmap = QGuiApplication::primaryScreen()->grabWindow(0, screenGeometry.x(), screenGeometry.y(), screenGeometry.width(), screenGeometry.height());

        return constructor(screenPixmap.toImage(), engine);
    }

	void Image::registerClass(QScriptEngine *scriptEngine)
	{
		CodeTools::addClassToScriptEngine<Image>(scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<Image>(&takeScreenshot, QStringLiteral("takeScreenshot"), scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<Image>(&takeScreenshotUsingScreenIndex, QStringLiteral("takeScreenshotUsingScreenIndex"), scriptEngine);
	}
	
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
	
	QScriptValue Image::clone() const
	{
		return constructor(mImage, engine());
	}
	
	bool Image::equals(const QScriptValue &other) const
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
	
	QScriptValue Image::setData(const QScriptValue &data)
	{
		QObject *object = data.toQObject();
		if(auto codeRawData = qobject_cast<RawData*>(object))
		{
			if(!mImage.loadFromData(codeRawData->byteArray()))
			{
				throwError(QStringLiteral("ImageDataError"), tr("Unable to set the image data"));
				return thisObject();
			}
		}
		else
			mImage = data.toVariant().value<QImage>();
		
		return thisObject();
	}
	
    QScriptValue Image::data(const QString &format) const
	{
        QBuffer dataBuffer;
		dataBuffer.open(QIODevice::WriteOnly);
		
		if(!mImage.save(&dataBuffer, format.toLatin1().constData()))
		{
			throwError(QStringLiteral("ImageDataError"), tr("Unable to get the image data"));
			return engine()->undefinedValue();
		}
		
        return RawData::constructor(dataBuffer.buffer(), engine());
	}
	
	QScriptValue Image::loadFromFile(const QString &filename)
	{
		if(!mImage.load(filename))
		{
			throwError(QStringLiteral("LoadImageError"), tr("Unable to load image from file %1").arg(filename));
			return thisObject();
		}
	
		return thisObject();
	}
	
	QScriptValue Image::saveToFile(const QString &filename) const
	{
		if(!mImage.save(filename))
		{
			throwError(QStringLiteral("SaveImageError"), tr("Unable to save image to file %1").arg(filename));
			return thisObject();
		}
	
		return thisObject();
	}
	
	QScriptValue Image::applyFilter(Filter filter, const QScriptValue &options)
	{
		QtImageFilter *imageFilter = QtImageFilterFactory::createImageFilter(filterNames[filter]);
		if(!imageFilter)
		{
			throwError(QStringLiteral("ApplyFilterError"), tr("Unable to apply filter"));
			return thisObject();
		}
		
		QScriptValueIterator it(options);
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
				return thisObject();
			}
		}
		
		mImage = imageFilter->apply(mImage);
		
		return thisObject();
	}
	
	QScriptValue Image::pixel(int x, int y) const
	{
		return Color::constructor(mImage.pixel(x, y), engine());
	}
	
	QScriptValue Image::setPixel(int x, int y, const QScriptValue &color)
	{
		if(context()->argumentCount() == 3)
		{
			QObject *object = color.toQObject();
			if(auto codeColor = qobject_cast<Color*>(object))
				mImage.setPixel(x, y, codeColor->color().rgb());
			else
				mImage.setPixel(x, y, QColor(color.toString()).rgb());
		}
		else if(context()->argumentCount() == 5)
		{
			QColor color(context()->argument(2).toInt32(), context()->argument(3).toInt32(), context()->argument(4).toInt32());
			mImage.setPixel(x, y, color.rgb());
		}
		else if(context()->argumentCount() == 6)
		{
			QColor color(context()->argument(2).toInt32(), context()->argument(3).toInt32(), context()->argument(4).toInt32());
			mImage.setPixel(x, y, color.rgb());
		}

        return thisObject();
    }

    QScriptValue Image::pixels() const
    {
        if(mImage.isNull())
            return QScriptValue();

        QImage argbImage = mImage.convertToFormat(QImage::Format_ARGB32);
        int pixelCount = argbImage.width() * argbImage.height();
        QScriptValue pixelArray = engine()->newArray(pixelCount);
        const QRgb *pixelData = reinterpret_cast<const QRgb *>(argbImage.constBits());

        for(int pixelIndex = 0; pixelIndex < pixelCount; ++pixelIndex)
            pixelArray.setProperty(pixelIndex, Color::constructor(QColor(pixelData[pixelIndex]), engine()));

        return pixelArray;
    }

    QScriptValue Image::pixelData() const
    {
        if(mImage.isNull())
            return QScriptValue();

        QImage argbImage = mImage.convertToFormat(QImage::Format_ARGB32);
        int pixelCount = argbImage.width() * argbImage.height();
        QScriptValue pixelArray = engine()->newArray(pixelCount * 4);
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
	
	QScriptValue Image::mirror(MirrorOrientation mirrorOrientation)
	{
		mImage = mImage.mirrored(mirrorOrientation & Horizontal, mirrorOrientation & Vertical);
		
		return thisObject();
	}
	
	QScriptValue Image::setSize()
	{
		mImage = mImage.scaled(Size::parameter(context(), engine()));
		
		return thisObject();
	}

	QScriptValue Image::size() const
	{
		return Size::constructor(mImage.size(), engine());
	}
	
	int Image::width() const
	{
		return mImage.width();
	}
	
	int Image::height() const
	{
		return mImage.height();
	}

	QScriptValue Image::copy() const
	{
		if(context()->argumentCount() == 0)
			return Image::constructor(mImage, engine());
		else
			return Image::constructor(mImage.copy(Rect::parameter(context(), engine())), engine());
	}

	QScriptValue Image::findSubImage(const QScriptValue &otherImage, const QScriptValue &options) const
	{
		if(auto codeImage = qobject_cast<Image*>(otherImage.toQObject()))
		{
			ActionTools::MatchingPointList matchingPointList;

			int confidenceMinimum;
			int downPyramidCount;
			int searchExpansion;
            AlgorithmMethod method;

            findSubImageOptions(options, &confidenceMinimum, &downPyramidCount, &searchExpansion, &method);

            if(!mOpenCVAlgorithms->findSubImage(QList<QImage>() << mImage, codeImage->image(), matchingPointList, confidenceMinimum, 1, downPyramidCount, searchExpansion, static_cast<ActionTools::OpenCVAlgorithms::AlgorithmMethod>(method)))
			{
				throwError(QStringLiteral("FindSubImageError"), tr("Error while searching for a sub-image: %1").arg(mOpenCVAlgorithms->errorString()));
				return QScriptValue();
			}

			if(matchingPointList.isEmpty())
				return QScriptValue();

			const ActionTools::MatchingPoint &matchingPoint = matchingPointList.first();
			QScriptValue back = engine()->newObject();

			back.setProperty(QStringLiteral("position"), Point::constructor(matchingPoint.position, engine()));
			back.setProperty(QStringLiteral("confidence"), matchingPoint.confidence);

			return back;
		}
		else
		{
			throwError(QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			return QScriptValue();
		}
	}

	bool matchingPointGreaterThan(const ActionTools::MatchingPoint &matchingPoint1, const ActionTools::MatchingPoint &matchingPoint2)
	{
        return matchingPoint1.confidence > matchingPoint2.confidence;
	}

	QScriptValue Image::findSubImages(const QScriptValue &otherImage, const QScriptValue &options) const
	{
		if(auto codeImage = qobject_cast<Image*>(otherImage.toQObject()))
		{
			ActionTools::MatchingPointList matchingPointList;

			int confidenceMinimum;
			int downPyramidCount;
			int searchExpansion;
            AlgorithmMethod method;
			int maximumMatches;

            findSubImageOptions(options, &confidenceMinimum, &downPyramidCount, &searchExpansion, &method, &maximumMatches);

            if(!mOpenCVAlgorithms->findSubImage(QList<QImage>() << mImage, codeImage->image(), matchingPointList, confidenceMinimum, maximumMatches, downPyramidCount, searchExpansion, static_cast<ActionTools::OpenCVAlgorithms::AlgorithmMethod>(method)))
			{
				throwError(QStringLiteral("FindSubImageError"), tr("Error while searching for a sub-image: %1").arg(mOpenCVAlgorithms->errorString()));
				return QScriptValue();
			}

			if(matchingPointList.isEmpty())
				return QScriptValue();

            std::sort(matchingPointList.begin(), matchingPointList.end(), matchingPointGreaterThan);

			ActionTools::MatchingPointList::ConstIterator matchingPointIt = matchingPointList.constBegin();
			QScriptValue back = engine()->newArray(matchingPointList.size());

			int index = 0;
			while(matchingPointIt != matchingPointList.constEnd())
			{
				QScriptValue object = engine()->newObject();

				object.setProperty(QStringLiteral("position"), Point::constructor(matchingPointIt->position, engine()));
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
			return QScriptValue();
		}
	}

	QScriptValue Image::findSubImageAsync(const QScriptValue &otherImage, const QScriptValue &callback, const QScriptValue &options)
	{
		mFindSubImageSearchForOne = true;

		if(!callback.isFunction())
		{
			throwError(QStringLiteral("FindSubImageError"), tr("Parameter \"callback\" is not a function"));
			return thisObject();
		}

		if(auto codeImage = qobject_cast<Image*>(otherImage.toQObject()))
		{
			int confidenceMinimum;
			int downPyramidCount;
			int searchExpansion;
            AlgorithmMethod method;

            findSubImageOptions(options, &confidenceMinimum, &downPyramidCount, &searchExpansion, &method);

            if(!mOpenCVAlgorithms->findSubImageAsync(QList<QImage>() << mImage, codeImage->image(), confidenceMinimum, 1, downPyramidCount, searchExpansion, static_cast<ActionTools::OpenCVAlgorithms::AlgorithmMethod>(method)))
			{
				throwError(QStringLiteral("FindSubImageError"), tr("Error while searching for a sub-image: %1").arg(mOpenCVAlgorithms->errorString()));
				return thisObject();
			}

			mFindSubImageAsyncFunction = callback;

			return thisObject();
		}
		else
		{
			throwError(QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			return thisObject();
		}
	}

	QScriptValue Image::findSubImagesAsync(const QScriptValue &otherImage, const QScriptValue &callback, const QScriptValue &options)
	{
		mFindSubImageSearchForOne = false;

		if(!callback.isFunction())
		{
			throwError(QStringLiteral("FindSubImageError"), tr("Parameter \"callback\" is not a function"));
			return thisObject();
		}

		if(auto codeImage = qobject_cast<Image*>(otherImage.toQObject()))
		{
			int confidenceMinimum;
			int downPyramidCount;
			int searchExpansion;
            AlgorithmMethod method;
			int maximumMatches;

            findSubImageOptions(options, &confidenceMinimum, &downPyramidCount, &searchExpansion, &method, &maximumMatches);

            if(!mOpenCVAlgorithms->findSubImageAsync(QList<QImage>() << mImage, codeImage->image(), confidenceMinimum, maximumMatches, downPyramidCount, searchExpansion, static_cast<ActionTools::OpenCVAlgorithms::AlgorithmMethod>(method)))
			{
				throwError(QStringLiteral("FindSubImageError"), tr("Error while searching for a sub-image: %1").arg(mOpenCVAlgorithms->errorString()));
				return thisObject();
			}

			mFindSubImageAsyncFunction = callback;

			return thisObject();
		}
		else
		{
			throwError(QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			return thisObject();
		}
	}

	void Image::findSubImageAsyncFinished(const ActionTools::MatchingPointList &matchingPointList)
	{
		if(mFindSubImageAsyncFunction.isValid())
		{
			if(matchingPointList.isEmpty())
			{
				mFindSubImageAsyncFunction.call(thisObject(), QScriptValueList() << QScriptValue());

				return;
			}

			if(mFindSubImageSearchForOne)
			{
				const ActionTools::MatchingPoint &matchingPoint = matchingPointList.first();
				QScriptValue back = mFindSubImageAsyncFunction.engine()->newObject();

				back.setProperty(QStringLiteral("position"), CodeClass::constructor(new Point(matchingPoint.position), mFindSubImageAsyncFunction.engine()));
				back.setProperty(QStringLiteral("confidence"), matchingPoint.confidence);

				mFindSubImageAsyncFunction.call(thisObject(), QScriptValueList() << back);
			}
			else
			{
				ActionTools::MatchingPointList matchingPointListCopy(matchingPointList);
                std::sort(matchingPointListCopy.begin(), matchingPointListCopy.end(), matchingPointGreaterThan);

				ActionTools::MatchingPointList::ConstIterator matchingPointIt = matchingPointListCopy.constBegin();
				QScriptValue back = mFindSubImageAsyncFunction.engine()->newArray(matchingPointListCopy.size());

				int index = 0;
				while(matchingPointIt != matchingPointListCopy.constEnd())
				{
					QScriptValue object = mFindSubImageAsyncFunction.engine()->newObject();

					object.setProperty(QStringLiteral("position"), CodeClass::constructor(new Point(matchingPointIt->position), mFindSubImageAsyncFunction.engine()));
					object.setProperty(QStringLiteral("confidence"), matchingPointIt->confidence);

					back.setProperty(index, object);

					++matchingPointIt;
					++index;
				}

				mFindSubImageAsyncFunction.call(thisObject(), QScriptValueList() << back);
			}
		}
	}

    void Image::findSubImageOptions(const QScriptValue &options, int *confidenceMinimum, int *downPyramidCount, int *searchExpansion, AlgorithmMethod *method, int *maximumMatches) const
	{
		QScriptValueIterator it(options);

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
				*confidenceMinimum = it.value().toInt32();
			else if(maximumMatches && it.name() == QStringLiteral("maximumMatches"))
				*maximumMatches = it.value().toInt32();
			else if(downPyramidCount && it.name() == QStringLiteral("downPyramidCount"))
				*downPyramidCount = it.value().toInt32();
			else if(searchExpansion && it.name() == QStringLiteral("searchExpansion"))
				*searchExpansion = it.value().toInt32();
			else if(searchExpansion && it.name() == QStringLiteral("method"))
                *method = static_cast<AlgorithmMethod>(it.value().toInt32());
		}
	}
}
