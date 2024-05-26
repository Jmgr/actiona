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
#include "qxtsmtp/qxtmailattachment.h"

#include <QJSValue>

namespace Code
{
    class MailAttachment : public CodeClass
    {
        Q_OBJECT
        Q_PROPERTY(QString contentType READ contentType WRITE setContentType)
        Q_PROPERTY(QJSValue content READ content WRITE setContent)

    public:
        Q_INVOKABLE MailAttachment();
        Q_INVOKABLE MailAttachment(const QJSValue &parameters);
        MailAttachment(const MailAttachment &other);
        MailAttachment(const QxtMailAttachment &attachment);

        QString contentType() const                                             { return mAttachment.contentType(); }
        QJSValue content() const;

        void setContentType(const QString &contentType)                         { mAttachment.setContentType(contentType); }
        MailAttachment *setContent(const QJSValue &content);

        const QxtMailAttachment &attachment() const                             { return mAttachment; }

        Q_INVOKABLE QJSValue clone() const;
        Q_INVOKABLE QString toString() const override                                                { return QStringLiteral("MailAttachment"); }
        Q_INVOKABLE MailAttachment *setExtraHeader(const QString &name, const QString &value)  { mAttachment.setExtraHeader(name, value); return this; }
        Q_INVOKABLE MailAttachment *removeExtraHeader(const QString &name)                     { mAttachment.removeExtraHeader(name); return this; }
        Q_INVOKABLE bool hasExtraHeader(const QString &name) const                          { return mAttachment.hasExtraHeader(name); }
        Q_INVOKABLE QString extraHeader(const QString &name) const                          { return mAttachment.extraHeader(name); }

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

    private:
        QxtMailAttachment mAttachment;
        QByteArray mContent;
    };
}

