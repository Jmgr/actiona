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
#include "qxtsmtp/qxtsmtp.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
    class Mail : public CodeClass
    {
        Q_OBJECT
        Q_PROPERTY(QString username READ username WRITE setUsername)
        Q_PROPERTY(QString password READ password WRITE setPassword)
        Q_PROPERTY(QScriptValue onConnected READ onConnected WRITE setOnConnected)
        Q_PROPERTY(QScriptValue onConnectionFailed READ onConnectionFailed WRITE setOnConnectionFailed)
        Q_PROPERTY(QScriptValue onEncrypted READ onEncrypted WRITE setOnEncrypted)
        Q_PROPERTY(QScriptValue onEncryptionFailed READ onEncryptionFailed WRITE setOnEncryptionFailed)
        Q_PROPERTY(QScriptValue onAuthenticated READ onAuthenticated WRITE setOnAuthenticated)
        Q_PROPERTY(QScriptValue onAuthenticationFailed READ onAuthenticationFailed WRITE setOnAuthenticationFailed)
        Q_PROPERTY(QScriptValue onSenderRejected READ onSenderRejected WRITE setOnSenderRejected)
        Q_PROPERTY(QScriptValue onRecipientRejected READ onRecipientRejected WRITE setOnRecipientRejected)
        Q_PROPERTY(QScriptValue onMailFailed READ onMailFailed WRITE setOnMailFailed)
        Q_PROPERTY(QScriptValue onMailSent READ onMailSent WRITE setOnMailSent)
        Q_PROPERTY(QScriptValue onFinished READ onFinished WRITE setOnFinished)
        Q_PROPERTY(QScriptValue onDisconnected READ onDisconnected WRITE setOnDisconnected)
        Q_PROPERTY(int pendingMessageCount READ pendingMessageCount)
        Q_PROPERTY(bool startTlsDisabled READ startTlsDisabled WRITE setStartTlsDisabled)

    public:
        static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

        Mail();

		QString username() const                                                    { return QString::fromUtf8(mSmtp.username()); }
        void setUsername(const QString &username)                                   { mSmtp.setUsername(username.toUtf8()); }

		QString password() const                                                    { return QString::fromUtf8(mSmtp.password()); }
        void setPassword(const QString &password)                                   { mSmtp.setPassword(password.toUtf8()); }

        void setOnConnected(const QScriptValue &onConnected)                        { mOnConnected = onConnected; }
        QScriptValue onConnected() const                                            { return mOnConnected; }

        void setOnConnectionFailed(const QScriptValue &onConnectionFailed)          { mOnConnectionFailed = onConnectionFailed; }
        QScriptValue onConnectionFailed() const                                     { return mOnConnectionFailed; }

        void setOnEncrypted(const QScriptValue &onEncrypted)                        { mOnEncrypted = onEncrypted; }
        QScriptValue onEncrypted() const                                            { return mOnEncrypted; }

        void setOnEncryptionFailed(const QScriptValue &onEncryptionFailed)          { mOnEncryptionFailed = onEncryptionFailed; }
        QScriptValue onEncryptionFailed() const                                     { return mOnEncryptionFailed; }

        void setOnAuthenticated(const QScriptValue &onAuthenticated)                { mOnAuthenticated = onAuthenticated; }
        QScriptValue onAuthenticated() const                                        { return mOnAuthenticated; }

        void setOnAuthenticationFailed(const QScriptValue &onAuthenticationFailed)	{ mOnAuthenticationFailed = onAuthenticationFailed; }
        QScriptValue onAuthenticationFailed() const                                 { return mOnAuthenticationFailed; }

        void setOnSenderRejected(const QScriptValue &onSenderRejected)              { mOnSenderRejected = onSenderRejected; }
        QScriptValue onSenderRejected() const                                       { return mOnSenderRejected; }

        void setOnRecipientRejected(const QScriptValue &onRecipientRejected)        { mOnRecipientRejected = onRecipientRejected; }
        QScriptValue onRecipientRejected() const                                    { return mOnRecipientRejected; }

        void setOnMailFailed(const QScriptValue &onMailFailed)                      { mOnMailFailed = onMailFailed; }
        QScriptValue onMailFailed() const                                           { return mOnMailFailed; }

        void setOnMailSent(const QScriptValue &onMailSent)                          { mOnMailSent = onMailSent; }
        QScriptValue onMailSent() const                                             { return mOnMailSent; }

        void setOnFinished(const QScriptValue &onFinished)                          { mOnFinished = onFinished; }
        QScriptValue onFinished() const                                             { return mOnFinished; }

        void setOnDisconnected(const QScriptValue &onDisconnected)                  { mOnDisconnected = onDisconnected; }
        QScriptValue onDisconnected() const                                         { return mOnDisconnected; }

        int pendingMessageCount() const                                             { return mSmtp.pendingMessages(); }

        bool startTlsDisabled() const                                               { return mSmtp.startTlsDisabled(); }
        void setStartTlsDisabled(bool disable)                                      { mSmtp.setStartTlsDisabled(disable); }

    public slots:
		QString toString() const override                                                    { return QStringLiteral("Mail"); }
        bool equals(const QScriptValue &other) const override                        { return defaultEqualsImplementation<Mail>(other); }

        QScriptValue connectToServer(const QString &serverName, int port = 25);
        QScriptValue connectToSecureServer(const QString &serverName, int port = 465);
        int send(const QScriptValue &mail);
        QScriptValue disconnectFromServer()                                         { mSmtp.disconnectFromHost(); return thisObject(); }
        bool hasExtension(const QString &extension)                                 { return mSmtp.hasExtension(extension); }
        QString extensionData(const QString &extension)                             { return mSmtp.extensionData(extension); }

        QScriptValue waitForConnected(int waitTime = 30000);
        QScriptValue waitForEncrypted(int waitTime = 30000);
        QScriptValue waitForAuthenticated(int waitTime = 30000);
        QScriptValue waitForFinished(int waitTime = 30000);
        QScriptValue waitForDisconnected(int waitTime = 30000);

    private slots:
        void connected();
        void connectionFailed(const QByteArray &msg);
        void encrypted();
        void encryptionFailed(const QByteArray &msg);
        void authenticated();
        void authenticationFailed(const QByteArray &msg);
        void senderRejected(int mailID, const QString &address, const QByteArray &msg);
        void recipientRejected(int mailID, const QString &address, const QByteArray &msg);
        void mailFailed(int mailID, int errorCode, const QByteArray &msg);
        void mailSent(int mailID);
        void finished();
        void disconnected();

    private:
        QxtSmtp mSmtp;
        QScriptValue mOnConnected;
        QScriptValue mOnConnectionFailed;
        QScriptValue mOnEncrypted;
        QScriptValue mOnEncryptionFailed;
        QScriptValue mOnAuthenticated;
        QScriptValue mOnAuthenticationFailed;
        QScriptValue mOnSenderRejected;
        QScriptValue mOnRecipientRejected;
        QScriptValue mOnMailFailed;
        QScriptValue mOnMailSent;
        QScriptValue mOnFinished;
        QScriptValue mOnDisconnected;
    };
}

