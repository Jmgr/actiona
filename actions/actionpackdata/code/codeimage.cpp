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

#include "codeimage.h"
#include "coderawdata.h"

#include <QBuffer>

QScriptValue CodeImage::constructor(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(context)

	return engine->newQObject(new CodeImage, QScriptEngine::ScriptOwnership);
}

QScriptValue CodeImage::constructor(const QImage &image, QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(context)

	return engine->newQObject(new CodeImage(image), QScriptEngine::ScriptOwnership);
}

CodeImage::CodeImage()
	: QObject(),
	QScriptable()
{
}

CodeImage::CodeImage(const CodeImage &other)
	: QObject(),
	QScriptable(),
	mImage(other.mImage)
{
}

CodeImage::CodeImage(const QImage &image)
	: QObject(),
	QScriptable(),
	mImage(image)
{
}

CodeImage &CodeImage::operator=(CodeImage other)
{
	swap(other);
	
	return *this;
}

CodeImage &CodeImage::operator=(QImage image)
{
	swap(image);
	
	return *this;
}

void CodeImage::swap(CodeImage &other)
{
	std::swap(mImage, other.mImage);
}

void CodeImage::swap(QImage &image)
{
	std::swap(mImage, image);
}
		
QImage CodeImage::image() const
{
	return mImage;
}

QScriptValue CodeImage::setData(const QScriptValue &data)
{
	QObject *object = data.toQObject();
	if(CodeRawData *codeRawData = qobject_cast<CodeRawData*>(object))
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

QScriptValue CodeImage::data() const
{
	QByteArray dataByteArray;
	QBuffer dataBuffer(&dataByteArray);
	dataBuffer.open(QIODevice::WriteOnly);
	
	if(!mImage.save(&dataBuffer, "BMP"))
	{
		context()->throwError(tr("Unable to get the image data"));
		return QScriptValue();
	}
	
	return CodeRawData::constructor(dataByteArray, context(), engine());
}

QScriptValue CodeImage::loadFromFile(const QString &filename)
{
	if(!mImage.load(filename))
	{
		context()->throwError(tr("Unable to load image from file %1").arg(filename));
		return context()->thisObject();
	}

	return context()->thisObject();
}

QScriptValue CodeImage::saveToFile(const QString &filename) const
{
	if(!mImage.save(filename))
	{
		context()->throwError(tr("Unable to save image to file %1").arg(filename));
		return context()->thisObject();
	}

	return context()->thisObject();
}
