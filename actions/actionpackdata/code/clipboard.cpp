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

#include "clipboard.h"
#include "code/image.h"

#include <QApplication>
#include <QImage>
#include <QMimeData>
#include <QScriptValueIterator>

namespace Code
{
	QScriptValue Clipboard::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Clipboard *clipboard = new Clipboard;

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == "mode")
				clipboard->setModePrivate(context, engine, static_cast<Mode>(it.value().toInt32()));
		}

		return CodeClass::constructor(clipboard, context, engine);
	}
	
	Clipboard::Clipboard()
		: CodeClass(),
		mMode(QClipboard::Clipboard)
	{
	}
	
	QScriptValue Clipboard::setMode(Mode mode)
	{
		setModePrivate(context(), engine(), mode);
	
		return context()->thisObject();
	}
	
	QScriptValue Clipboard::writeText(const QString &value) const
	{
		QClipboard *clipboard = QApplication::clipboard();

		clipboard->setText(value, mMode);
		
		return context()->thisObject();
	}
	
	QScriptValue Clipboard::writeImage(const QScriptValue &data) const
	{
		QClipboard *clipboard = QApplication::clipboard();

		QObject *object = data.toQObject();
		if(Code::Image *image = qobject_cast<Code::Image*>(object))
			clipboard->setImage(image->image(), mMode);
		else
			clipboard->setImage(data.toVariant().value<QImage>(), mMode);
	
		return context()->thisObject();
	}
	
	QString Clipboard::readText() const
	{
		QClipboard *clipboard = QApplication::clipboard();
		
		return clipboard->text(mMode);
	}

	QScriptValue Clipboard::readImage() const
	{
		QClipboard *clipboard = QApplication::clipboard();

		return Image::constructor(clipboard->image(mMode), context(), engine());
	}
	
	Clipboard::DataType Clipboard::dataType() const
	{
		QClipboard *clipboard = QApplication::clipboard();
		const QMimeData *mimeData = clipboard->mimeData(mMode);
	
		if(mimeData->hasImage())
			return Image;
		else
			return Text;
	}
	
	void Clipboard::setModePrivate(QScriptContext *context, QScriptEngine *engine, Mode mode)
	{
		switch(mode)
		{
		case Selection:
			if(!QApplication::clipboard()->supportsSelection())
			{
				throwError(context, engine, "UnsupportedSelectionModeError", tr("Selection mode is not supported by your operating system"));
				return;
			}
			break;
		case FindBuffer:
			if(!QApplication::clipboard()->supportsFindBuffer())
			{
				throwError(context, engine, "UnsupportedSelectionModeError", tr("Find buffer mode is not supported by your operating system"));
				return;
			}
			break;
		default:
			break;
		}
		
		mMode = static_cast<QClipboard::Mode>(mode);
	}
}
