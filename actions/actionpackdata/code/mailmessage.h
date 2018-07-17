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

#include "code/codeclass.h"
#include "qxtsmtp/qxtmailmessage.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
    class MailMessage : public CodeClass
    {
        Q_OBJECT
        Q_ENUMS(RecipientType)
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

        static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

        MailMessage();

        QString sender() const                                                      { return mMessage.sender(); }
        void setSender(const QString &sender)                                       { mMessage.setSender(sender); }

        QString subject() const                                                     { return mMessage.subject(); }
        void setSubject(const QString &subject)                                     { mMessage.setSubject(subject); }

        QString body() const                                                        { return mMessage.body(); }
        void setBody(const QString &body)                                           { mMessage.setBody(body); }

        QxtMailMessage &message()                                                   { return mMessage; }

    public slots:
		QString toString() const override                                                    { return QStringLiteral("MailMessage"); }
        bool equals(const QScriptValue &other) const override                        { return defaultEqualsImplementation<MailMessage>(other); }

        QScriptValue setExtraHeader(const QString &name, const QString &value)      { mMessage.setExtraHeader(name, value); return thisObject(); }
        QScriptValue removeExtraHeader(const QString &name)                         { mMessage.removeExtraHeader(name); return thisObject(); }
        bool hasExtraHeader(const QString &name) const                              { return mMessage.hasExtraHeader(name); }
        QString extraHeader(const QString &name) const                              { return mMessage.extraHeader(name); }

        QStringList recipients(RecipientType type = To) const                       { return mMessage.recipients(static_cast<QxtMailMessage::RecipientType>(type)); }
        QScriptValue addRecipient(const QString &recipient, RecipientType type = To){ mMessage.addRecipient(recipient, static_cast<QxtMailMessage::RecipientType>(type)); return thisObject(); }
        QScriptValue removeRecipient(const QString &recipient)                      { mMessage.removeRecipient(recipient); return thisObject(); }

        QScriptValue attachments() const;
        QScriptValue attachment(const QString &filename) const;
        QScriptValue addAttachment(const QString &filename, const QScriptValue &attachment);
        QScriptValue removeAttachment(const QString &filename);

    private:
        QxtMailMessage mMessage;
    };
}


