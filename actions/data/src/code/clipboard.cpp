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

#include "clipboard.hpp"
#include "actiontools/code/image.hpp"

#include <QApplication>
#include <QImage>
#include <QMimeData>

namespace Code
{
	Clipboard::Clipboard()
		: CodeClass()
		
	{
	}

    Clipboard::Clipboard(Mode mode)
        : CodeClass()
    {
        setModePrivate(mode);
    }

	QString Clipboard::text() const
	{
		QClipboard *clipboard = QApplication::clipboard();

		return clipboard->text(mMode);
	}

	QJSValue Clipboard::image() const
	{
		QClipboard *clipboard = QApplication::clipboard();

        return CodeClass::construct<Code::Image>(clipboard->image(mMode));
    }

    Clipboard *Clipboard::setMode(Mode mode)
	{
        setModePrivate(mode);
	
        return this;
	}
	
    Clipboard *Clipboard::setText(const QString &value)
	{
		QClipboard *clipboard = QApplication::clipboard();

		clipboard->setText(value, mMode);
		
        return this;
	}
	
    Clipboard *Clipboard::setImage(const QJSValue &data)
	{
		QClipboard *clipboard = QApplication::clipboard();

		QObject *object = data.toQObject();
		if(auto image = qobject_cast<Code::Image*>(object))
			clipboard->setImage(image->image(), mMode);
		else
			clipboard->setImage(data.toVariant().value<QImage>(), mMode);
	
        return this;
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

    void Clipboard::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Clipboard*>("const Clipboard *");

        CodeClass::registerClass<Clipboard>(QStringLiteral("Clipboard"), scriptEngine);
    }
	
    void Clipboard::setModePrivate(Mode mode)
	{
		switch(mode)
		{
		case Selection:
			if(!QApplication::clipboard()->supportsSelection())
			{
                throwError(QStringLiteral("UnsupportedSelectionModeError"), tr("Selection mode is not supported by your operating system"));
				return;
			}
			break;
		case FindBuffer:
			if(!QApplication::clipboard()->supportsFindBuffer())
			{
                throwError(QStringLiteral("UnsupportedSelectionModeError"), tr("Find buffer mode is not supported by your operating system"));
				return;
			}
			break;
		default:
			break;
		}
		
		mMode = static_cast<QClipboard::Mode>(mode);
	}
}
