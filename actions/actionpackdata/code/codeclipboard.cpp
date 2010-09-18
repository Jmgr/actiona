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

#include "codeclipboard.h"
#include "codeimage.h"

#include <QApplication>
#include <QImage>
#include <QMimeData>

QScriptValue CodeClipboard::constructor(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(context)
	
	return engine->newQObject(new CodeClipboard, QScriptEngine::ScriptOwnership);
}

CodeClipboard::CodeClipboard()
	: mMode(Clipboard)
{
}

QScriptValue CodeClipboard::setMode(Mode mode)
{
	switch(mode)
	{
	case Selection:
		if(!QApplication::clipboard()->supportsSelection())
		{
			context()->throwError(tr("Selection mode is not supported by your operating system"));
			return context()->thisObject();
		}
		break;
	case FindBuffer:
		if(!QApplication::clipboard()->supportsFindBuffer())
		{
			context()->throwError(tr("Find buffer mode is not supported by your operating system"));
			return context()->thisObject();
		}
		break;
	default:
		mMode = mode;
		break;
	}

	return context()->thisObject();
}

QScriptValue CodeClipboard::writeText(const QString &value) const
{
	QClipboard *clipboard = QApplication::clipboard();
	QClipboard::Mode clipboardMode = static_cast<QClipboard::Mode>(mMode);
	
	clipboard->setText(value, clipboardMode);
	
	return context()->thisObject();
}

QScriptValue CodeClipboard::writeImage(const QScriptValue &data) const
{
	QClipboard *clipboard = QApplication::clipboard();
	QClipboard::Mode clipboardMode = static_cast<QClipboard::Mode>(mMode);
	
	QObject *object = data.toQObject();
	if(CodeImage *codeImage = qobject_cast<CodeImage*>(object))
		clipboard->setImage(codeImage->image(), clipboardMode);
	else
		clipboard->setImage(data.toVariant().value<QImage>(), clipboardMode);

	return context()->thisObject();
}

QString CodeClipboard::readText() const
{
	QClipboard *clipboard = QApplication::clipboard();
	QClipboard::Mode clipboardMode = static_cast<QClipboard::Mode>(mMode);
	
	return clipboard->text(clipboardMode);
}

QScriptValue CodeClipboard::readImage() const
{
	QClipboard *clipboard = QApplication::clipboard();
	QClipboard::Mode clipboardMode = static_cast<QClipboard::Mode>(mMode);
	
	return CodeImage::constructor(clipboard->image(clipboardMode), context(), engine());
}

CodeClipboard::DataType CodeClipboard::dataType() const
{
	QClipboard *clipboard = QApplication::clipboard();
	QClipboard::Mode clipboardMode = static_cast<QClipboard::Mode>(mMode);
	const QMimeData *mimeData = clipboard->mimeData(clipboardMode);

	if(mimeData->hasImage())
		return Image;
	else
		return Text;
}
