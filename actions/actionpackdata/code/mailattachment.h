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
#include "qxtsmtp/qxtmailattachment.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
    class MailAttachment : public CodeClass
    {
        Q_OBJECT
        Q_PROPERTY(QString contentType READ contentType WRITE setContentType)
        Q_PROPERTY(QScriptValue content READ content WRITE setContent)

    public:
        static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
        static QScriptValue constructor(const QxtMailAttachment &attachment, QScriptEngine *engine);

        MailAttachment();
        MailAttachment(const QxtMailAttachment &attachment);

        QString contentType() const                                             { return mAttachment.contentType(); }
        QScriptValue content() const;

        void setContentType(const QString &contentType)                         { mAttachment.setContentType(contentType); }
        QScriptValue setContent(const QScriptValue &content);

        const QxtMailAttachment &attachment() const                             { return mAttachment; }

    public slots:
        QScriptValue clone() const;
		QString toString() const override                                                { return QStringLiteral("MailAttachment"); }
        bool equals(const QScriptValue &other) const override                    { return defaultEqualsImplementation<MailAttachment>(other); }

        QScriptValue setExtraHeader(const QString &name, const QString &value)  { mAttachment.setExtraHeader(name, value); return thisObject(); }
        QScriptValue removeExtraHeader(const QString &name)                     { mAttachment.removeExtraHeader(name); return thisObject(); }
        bool hasExtraHeader(const QString &name) const                          { return mAttachment.hasExtraHeader(name); }
        QString extraHeader(const QString &name) const                          { return mAttachment.extraHeader(name); }

    private:
        QxtMailAttachment mAttachment;
        QByteArray mContent;
    };
}

