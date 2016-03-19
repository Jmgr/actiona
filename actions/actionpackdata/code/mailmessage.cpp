/*
    Actiona
    Copyright (C) 2005-2016 Jonathan Mercier-Ganady

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

#include "mailmessage.h"
#include "mailattachment.h"

#include <QScriptValueIterator>

namespace Code
{
    QScriptValue MailMessage::constructor(QScriptContext *context, QScriptEngine *engine)
    {
        MailMessage *mailMessage = new MailMessage;

        QScriptValueIterator it(context->argument(0));

        while(it.hasNext())
        {
            it.next();

            if(it.name() == "sender")
                mailMessage->mMessage.setSender(it.value().toString());
            else if(it.name() == "subject")
                mailMessage->mMessage.setSubject(it.value().toString());
            else if(it.name() == "body")
                mailMessage->mMessage.setBody(it.value().toString());
        }

        return CodeClass::constructor(mailMessage, context, engine);
    }

    MailMessage::MailMessage()
        : CodeClass()
    {
    }

    QScriptValue MailMessage::attachments() const
    {
        const QHash<QString, QxtMailAttachment> &attachments = mMessage.attachments();
        QScriptValue back = engine()->newArray(attachments.size());

        int index = 0;
        for(QHash<QString, QxtMailAttachment>::ConstIterator it = attachments.constBegin(); it != attachments.constEnd(); ++it)
        {
            QScriptValue attachmentObject = engine()->newObject();

            attachmentObject.setProperty("filename", it.key());
            attachmentObject.setProperty("attachment", MailAttachment::constructor(it.value(), engine()));

            back.setProperty(index, attachmentObject);

            ++index;
        }

        return back;
    }

    QScriptValue MailMessage::attachment(const QString &filename) const
    {
        return MailAttachment::constructor(mMessage.attachment(filename), engine());
    }

    QScriptValue MailMessage::addAttachment(const QString &filename, const QScriptValue &attachment)
    {
        if(MailAttachment *mailAttachment = qobject_cast<MailAttachment*>(attachment.toQObject()))
            mMessage.addAttachment(filename, mailAttachment->attachment());
        else
            throwError("ParameterTypeError", tr("Incorrect parameter type: not a MailAttachment"));

        return thisObject();
    }

    QScriptValue MailMessage::removeAttachment(const QString &filename)
    {
        mMessage.removeAttachment(filename);

        return thisObject();
    }
}
