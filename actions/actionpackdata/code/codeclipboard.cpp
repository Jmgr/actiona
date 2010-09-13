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

QScriptValue CodeClipboard::constructor(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(context)
	
	return engine->newQObject(new CodeClipboard, QScriptEngine::ScriptOwnership);
}

QScriptValue CodeClipboard::writeText(const QString &value, Mode mode) const
{
	if(!isModeValid(mode))
		return context()->thisObject();
	
	QClipboard *clipboard = QApplication::clipboard();
	QClipboard::Mode clipboardMode = static_cast<QClipboard::Mode>(mode);
	
	clipboard->setText(value, clipboardMode);
	
	return context()->thisObject();
}

QScriptValue CodeClipboard::writeImage(const QScriptValue &data, Mode mode) const
{
	if(!isModeValid(mode))
		return context()->thisObject();
	
	QClipboard *clipboard = QApplication::clipboard();
	QClipboard::Mode clipboardMode = static_cast<QClipboard::Mode>(mode);
	
	QObject *object = data.toQObject();
	if(CodeImage *codeImage = qobject_cast<CodeImage*>(object))
		clipboard->setImage(codeImage->image(), clipboardMode);
	else
		clipboard->setImage(data.toVariant().value<QImage>(), clipboardMode);

	return context()->thisObject();
}

QString CodeClipboard::readText(Mode mode) const
{
	if(!isModeValid(mode))
		return QString();
	
	QClipboard *clipboard = QApplication::clipboard();
	QClipboard::Mode clipboardMode = static_cast<QClipboard::Mode>(mode);
	
	return clipboard->text(clipboardMode);
}

QScriptValue CodeClipboard::readImage(Mode mode) const
{
	if(!isModeValid(mode))
		return QScriptValue();
	
	QClipboard *clipboard = QApplication::clipboard();
	QClipboard::Mode clipboardMode = static_cast<QClipboard::Mode>(mode);
	
	return CodeImage::constructor(clipboard->image(clipboardMode), context(), engine());
}

bool CodeClipboard::isModeValid(Mode mode) const
{
	switch(mode)
	{
	case Clipboard:
		return true;
	case Selection:
		return QApplication::clipboard()->supportsSelection();
	case FindBuffer:
		return QApplication::clipboard()->supportsFindBuffer();
	default:
		return false;
	}
}
