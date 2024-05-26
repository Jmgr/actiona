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

#include "mailattachment.hpp"
#include "actiontools/code/rawdata.hpp"

#include <QJSValueIterator>

namespace Code
{
    MailAttachment::MailAttachment()
        : CodeClass()
    {
    }

    MailAttachment::MailAttachment(const QJSValue &parameters)
        : MailAttachment()
    {
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return;
        }

        QJSValueIterator it(parameters);

        while(it.hasNext())
        {
            it.next();

            if(it.name() == QLatin1String("contentType"))
                setContentType(it.value().toString());
            else if(it.name() == QLatin1String("content"))
                setContent(it.value());
        }
    }

    MailAttachment::MailAttachment(const MailAttachment &other)
        : CodeClass(),
          mAttachment(other.mAttachment)
    {
    }

    MailAttachment::MailAttachment(const QxtMailAttachment& attachment)
        : CodeClass(),
          mAttachment(attachment)
    {
    }

    QJSValue MailAttachment::content() const
    {
        return CodeClass::construct<RawData>(mContent);
    }

    MailAttachment *MailAttachment::setContent(const QJSValue &content)
    {
        if(auto rawData = qobject_cast<RawData*>(content.toQObject()))
            mContent = rawData->byteArray();
        else
            mContent = content.toVariant().toByteArray();

        mAttachment.setContent(mContent);

        return this;
    }

    QJSValue MailAttachment::clone() const
    {
        return CodeClass::clone<MailAttachment>();
    }

    void MailAttachment::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<MailAttachment*>("const MailAttachment *");

        CodeClass::registerClass<MailAttachment>(QStringLiteral("MailAttachment"), scriptEngine);
    }
}
