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

namespace Code
{
	QScriptValue Clipboard::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
		
		return engine->newQObject(new Clipboard, QScriptEngine::ScriptOwnership);
	}
	
	Clipboard::Clipboard()
		: mMode(QClipboard::Clipboard)
	{
	}
	
	QScriptValue Clipboard::setMode(Mode mode)
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
			break;
		}
		
		mMode = static_cast<QClipboard::Mode>(mode);
	
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
}
