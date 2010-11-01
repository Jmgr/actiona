/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#include "image.h"
#include "rawdata.h"
#include "color.h"
#include "size.h"
#include "rect.h"
#include "window.h"
#include "qtimagefilters/QtImageFilterFactory"

#include <QBuffer>
#include <QScriptValueIterator>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>

namespace Code
{
	QScriptValue Image::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Image *image = 0;
		
		switch(context->argumentCount())
		{
		case 0:
			image = new Image;
			break;
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(Image *codeImage = qobject_cast<Image*>(object))
					image = new Image(*codeImage);
				else
					context->throwError("Incorrect parameter type");
			}
			break;
		default:
			context->throwError("Incorrect parameter count");
			break;
		}
		
		if(!image)
			return engine->undefinedValue();
	
		return engine->newQObject(image, QScriptEngine::ScriptOwnership);
	}
	
	QScriptValue Image::constructor(const QImage &image, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
	
		return engine->newQObject(new Image(image), QScriptEngine::ScriptOwnership);
	}

	QScriptValue Image::takeScreenshot(QScriptContext *context, QScriptEngine *engine)
	{
		WId windowId = QApplication::desktop()->winId();

		if(context->argumentCount() > 0)
		{
			Window *window = qobject_cast<Window *>(context->argument(0).toQObject());
			if(window)
				windowId = window->windowHandle().value();
			else
			{
#ifdef Q_WS_WIN
				context->throwError("Invalid window");
				return engine->undefinedValue();
#else
				windowId = context->argument(0).toInt32();
#endif
			}
		}

		QPixmap screenPixmap = QPixmap::grabWindow(windowId);

		return constructor(screenPixmap.toImage(), context, engine);
	}
	
	const QString Image::filterNames[] =
	{
		"ConvolutionFilter",
		"GaussianBlur",
		"Defocus",
		"Highlight",
		"Sharpen",
		"SharpenMore",
		"SharpenEvenMore",
		"EdgeDetect",
		"BigEdge",
		"Emboss",
		"EmbossColor",
		"Negative",
		"RemoveChannel",
		"Punch"
	};
	
	const QStringList Image::filterOptionsNames = QStringList() << "filterChannels" << "filterBorderPolicy" << "convolutionDivisor"
												  << "convolutionBias" << "" << "radius" << "force" << "center";
	
	Image::Image()
		: QObject(),
		QScriptable()
	{
	}
	
	Image::Image(const Image &other)
		: QObject(),
		QScriptable(),
		mImage(other.mImage)
	{
	}
	
	Image::Image(const QImage &image)
		: QObject(),
		QScriptable(),
		mImage(image)
	{
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
		return constructor(mImage, context(), engine());
	}
	
	bool Image::equals(const QScriptValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(Image *otherImage = qobject_cast<Image*>(object))
			return (otherImage == this || otherImage->mImage == mImage);
			
		return false;
	}

	QString Image::toString() const
	{
		return QString("Image [width: %1][height: %2]").arg(width()).arg(height());
	}
	
	QScriptValue Image::setData(const QScriptValue &data)
	{
		QObject *object = data.toQObject();
		if(RawData *codeRawData = qobject_cast<RawData*>(object))
		{
			if(!mImage.loadFromData(codeRawData->byteArray()))
			{
				context()->throwError(tr("Unable to set the image data"));
				return context()->thisObject();
			}
		}
		else
			mImage = data.toVariant().value<QImage>();
		
		return context()->thisObject();
	}
	
	QScriptValue Image::data() const
	{
		QByteArray dataByteArray;
		QBuffer dataBuffer(&dataByteArray);
		dataBuffer.open(QIODevice::WriteOnly);
		
		if(!mImage.save(&dataBuffer, "BMP"))
		{
			context()->throwError(tr("Unable to get the image data"));
			return engine()->undefinedValue();
		}
		
		return RawData::constructor(dataByteArray, context(), engine());
	}
	
	QScriptValue Image::loadFromFile(const QString &filename)
	{
		if(!mImage.load(filename))
		{
			context()->throwError(tr("Unable to load image from file %1").arg(filename));
			return context()->thisObject();
		}
	
		return context()->thisObject();
	}
	
	QScriptValue Image::saveToFile(const QString &filename) const
	{
		if(!mImage.save(filename))
		{
			context()->throwError(tr("Unable to save image to file %1").arg(filename));
			return context()->thisObject();
		}
	
		return context()->thisObject();
	}
	
	QScriptValue Image::applyFilter(Filter filter, const QScriptValue &options)
	{
		QtImageFilter *imageFilter = QtImageFilterFactory::createImageFilter(filterNames[filter]);
		if(!imageFilter)
		{
			context()->throwError(tr("Unable to apply filter"));
			return context()->thisObject();
		}
		
		QScriptValueIterator it(options);
		while(it.hasNext())
		{
			it.next();
			
			int optionIndex = filterOptionsNames.indexOf(it.name());
			if(optionIndex == -1)
				continue;
			
			QtImageFilter::FilterOption option = static_cast<QtImageFilter::FilterOption>(optionIndex + 1);
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
				value = QPointF(it.value().property("x").toNumber(), it.value().property("y").toNumber());
				break;
			default:
				break;
			}
			
			if(!imageFilter->setOption(option, value))
			{
				context()->throwError(tr("Cannot set filter option %1 %2").arg(it.name()).arg(value.toString()));
				return context()->thisObject();
			}
		}
		
		mImage = imageFilter->apply(mImage);
		
		return context()->thisObject();
	}
	
	QScriptValue Image::pixel(int x, int y) const
	{
		return Color::constructor(mImage.pixel(x, y), context(), engine());
	}
	
	QScriptValue Image::setPixel(int x, int y, const QScriptValue &color)
	{
		if(context()->argumentCount() == 3)
		{
			QObject *object = color.toQObject();
			if(Color *codeColor = qobject_cast<Color*>(object))
				mImage.setPixel(x, y, codeColor->color().rgb());
			else
				mImage.setPixel(x, y, QColor(color.toString()).rgb());
		}
		else if(context()->argumentCount() == 5)
		{
			QColor color(context()->argument(2).toInt32(), context()->argument(3).toInt32(), context()->argument(4).toInt32());
			mImage.setPixel(x, y, color.rgb());
		}

		return context()->thisObject();
	}
	
	QScriptValue Image::mirror(MirrorOrientation mirrorOrientation)
	{
		mImage = mImage.mirrored(mirrorOrientation & Horizontal, mirrorOrientation & Vertical);
		
		return context()->thisObject();
	}
	
	QScriptValue Image::setSize()
	{
		mImage = mImage.scaled(Size::parameter(context()));
		
		return context()->thisObject();
	}

	QScriptValue Image::size() const
	{
		return Size::constructor(mImage.size(), context(), engine());
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
			return Image::constructor(mImage, context(), engine());
		else
			return Image::constructor(mImage.copy(Rect::parameter(context())), context(), engine());
	}
}
