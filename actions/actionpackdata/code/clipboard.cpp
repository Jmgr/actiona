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
		auto clipboard = new Clipboard;

		if(context->argumentCount() > 0)
		{
			Mode mode = static_cast<Mode>(context->argument(0).toInt32());
			clipboard->setModePrivate(context, engine, mode);
		}

		return CodeClass::constructor(clipboard, context, engine);
	}
	
	Clipboard::Clipboard()
		: CodeClass()
		
	{
	}

	QString Clipboard::text() const
	{
		QClipboard *clipboard = QApplication::clipboard();

		return clipboard->text(mMode);
	}

	QScriptValue Clipboard::image() const
	{
		QClipboard *clipboard = QApplication::clipboard();

        return Image::constructor(clipboard->image(mMode), engine());
    }


	
	QScriptValue Clipboard::setMode(Mode mode)
	{
		setModePrivate(context(), engine(), mode);
	
		return thisObject();
	}
	
	QScriptValue Clipboard::setText(const QString &value) const
	{
		QClipboard *clipboard = QApplication::clipboard();

		clipboard->setText(value, mMode);
		
		return thisObject();
	}
	
	QScriptValue Clipboard::setImage(const QScriptValue &data) const
	{
		QClipboard *clipboard = QApplication::clipboard();

		QObject *object = data.toQObject();
		if(auto image = qobject_cast<Code::Image*>(object))
			clipboard->setImage(image->image(), mMode);
		else
			clipboard->setImage(data.toVariant().value<QImage>(), mMode);
	
		return thisObject();
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
				throwError(context, engine, QStringLiteral("UnsupportedSelectionModeError"), tr("Selection mode is not supported by your operating system"));
				return;
			}
			break;
		case FindBuffer:
			if(!QApplication::clipboard()->supportsFindBuffer())
			{
				throwError(context, engine, QStringLiteral("UnsupportedSelectionModeError"), tr("Find buffer mode is not supported by your operating system"));
				return;
			}
			break;
		default:
			break;
		}
		
		mMode = static_cast<QClipboard::Mode>(mode);
	}
}
