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

#include "mailmessage.hpp"
#include "mailattachment.hpp"
#include "actiontools/scriptengine.hpp"

#include <QJSValueIterator>

namespace Code
{
    MailMessage::MailMessage()
        : CodeClass()
    {
    }

    MailMessage::MailMessage(const QJSValue &parameters)
        : MailMessage()
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

            if(it.name() == QLatin1String("sender"))
                mMessage.setSender(it.value().toString());
            else if(it.name() == QLatin1String("subject"))
                mMessage.setSubject(it.value().toString());
            else if(it.name() == QLatin1String("body"))
                mMessage.setBody(it.value().toString());
        }
    }

    QJSValue MailMessage::attachments() const
    {
        const QHash<QString, QxtMailAttachment> &attachments = mMessage.attachments();
        QJSValue back = ActionTools::ScriptEngine::current()->newArray(attachments.size());

        int index = 0;
        for(QHash<QString, QxtMailAttachment>::ConstIterator it = attachments.constBegin(); it != attachments.constEnd(); ++it)
        {
            QJSValue attachmentObject = ActionTools::ScriptEngine::current()->newObject();

            attachmentObject.setProperty(QStringLiteral("filename"), it.key());
            attachmentObject.setProperty(QStringLiteral("attachment"), CodeClass::construct<MailAttachment>(it.value()));

            back.setProperty(index, attachmentObject);

            ++index;
        }

        return back;
    }

    QJSValue MailMessage::attachment(const QString &filename) const
    {
        return CodeClass::construct<MailAttachment>(mMessage.attachment(filename));
    }

    MailMessage *MailMessage::addAttachment(const QString &filename, const QJSValue &attachment)
    {
        if(auto mailAttachment = qobject_cast<MailAttachment*>(attachment.toQObject()))
            mMessage.addAttachment(filename, mailAttachment->attachment());
        else
			throwError(QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type: not a MailAttachment"));

        return this;
    }

    MailMessage *MailMessage::removeAttachment(const QString &filename)
    {
        mMessage.removeAttachment(filename);

        return this;
    }

    void MailMessage::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<MailMessage*>("const MailMessage *");

        CodeClass::registerClass<MailMessage>(QStringLiteral("MailMessage"), scriptEngine);
    }
}
