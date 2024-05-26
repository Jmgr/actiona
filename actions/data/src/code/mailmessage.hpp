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

#pragma once

#include "actiontools/code/codeclass.hpp"
#include "qxtsmtp/qxtmailmessage.h"

#include <QJSValue>

namespace Code
{
    class MailMessage : public CodeClass
    {
        Q_OBJECT
        Q_PROPERTY(QString sender READ sender WRITE setSender)
        Q_PROPERTY(QString subject READ subject WRITE setSubject)
        Q_PROPERTY(QString body READ body WRITE setBody)

    public:
        enum RecipientType
        {
            To,
            Cc,
            Bcc
        };
        Q_ENUM(RecipientType)

        Q_INVOKABLE MailMessage();
        Q_INVOKABLE explicit MailMessage(const QJSValue &parameters);

        QString sender() const                                                      { return mMessage.sender(); }
        void setSender(const QString &sender)                                       { mMessage.setSender(sender); }

        QString subject() const                                                     { return mMessage.subject(); }
        void setSubject(const QString &subject)                                     { mMessage.setSubject(subject); }

        QString body() const                                                        { return mMessage.body(); }
        void setBody(const QString &body)                                           { mMessage.setBody(body); }

        QxtMailMessage &message()                                                   { return mMessage; }

        Q_INVOKABLE QString toString() const override                                                    { return QStringLiteral("MailMessage"); }

        Q_INVOKABLE MailMessage *setExtraHeader(const QString &name, const QString &value)      { mMessage.setExtraHeader(name, value); return this; }
        Q_INVOKABLE MailMessage *removeExtraHeader(const QString &name)                         { mMessage.removeExtraHeader(name); return this; }
        Q_INVOKABLE bool hasExtraHeader(const QString &name) const                              { return mMessage.hasExtraHeader(name); }
        Q_INVOKABLE QString extraHeader(const QString &name) const                              { return mMessage.extraHeader(name); }

        Q_INVOKABLE QStringList recipients(RecipientType type = To) const                       { return mMessage.recipients(static_cast<QxtMailMessage::RecipientType>(type)); }
        Q_INVOKABLE MailMessage *addRecipient(const QString &recipient, RecipientType type = To){ mMessage.addRecipient(recipient, static_cast<QxtMailMessage::RecipientType>(type)); return this; }
        Q_INVOKABLE MailMessage *removeRecipient(const QString &recipient)                      { mMessage.removeRecipient(recipient); return this; }

        Q_INVOKABLE QJSValue attachments() const;
        Q_INVOKABLE QJSValue attachment(const QString &filename) const;
        Q_INVOKABLE MailMessage *addAttachment(const QString &filename, const QJSValue &attachment);
        Q_INVOKABLE MailMessage *removeAttachment(const QString &filename);

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

    private:
        QxtMailMessage mMessage;
    };
}


