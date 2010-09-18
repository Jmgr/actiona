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

#include <QBuffer>

namespace Code
{
	QScriptValue Image::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
	
		return engine->newQObject(new Image, QScriptEngine::ScriptOwnership);
	}
	
	QScriptValue Image::constructor(const QImage &image, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
	
		return engine->newQObject(new Image(image), QScriptEngine::ScriptOwnership);
	}
	
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
			
	QImage Image::image() const
	{
		return mImage;
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
			return QScriptValue();
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
}
