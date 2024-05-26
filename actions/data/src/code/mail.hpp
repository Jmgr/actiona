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
#include "qxtsmtp/qxtsmtp.h"

#include <QJSValue>

namespace Code
{
    class Mail : public CodeClass
    {
        Q_OBJECT
        Q_PROPERTY(QString username READ username WRITE setUsername)
        Q_PROPERTY(QString password READ password WRITE setPassword)
        Q_PROPERTY(QJSValue onConnected READ onConnected WRITE setOnConnected)
        Q_PROPERTY(QJSValue onConnectionFailed READ onConnectionFailed WRITE setOnConnectionFailed)
        Q_PROPERTY(QJSValue onEncrypted READ onEncrypted WRITE setOnEncrypted)
        Q_PROPERTY(QJSValue onEncryptionFailed READ onEncryptionFailed WRITE setOnEncryptionFailed)
        Q_PROPERTY(QJSValue onAuthenticated READ onAuthenticated WRITE setOnAuthenticated)
        Q_PROPERTY(QJSValue onAuthenticationFailed READ onAuthenticationFailed WRITE setOnAuthenticationFailed)
        Q_PROPERTY(QJSValue onSenderRejected READ onSenderRejected WRITE setOnSenderRejected)
        Q_PROPERTY(QJSValue onRecipientRejected READ onRecipientRejected WRITE setOnRecipientRejected)
        Q_PROPERTY(QJSValue onMailFailed READ onMailFailed WRITE setOnMailFailed)
        Q_PROPERTY(QJSValue onMailSent READ onMailSent WRITE setOnMailSent)
        Q_PROPERTY(QJSValue onFinished READ onFinished WRITE setOnFinished)
        Q_PROPERTY(QJSValue onDisconnected READ onDisconnected WRITE setOnDisconnected)
        Q_PROPERTY(int pendingMessageCount READ pendingMessageCount)
        Q_PROPERTY(bool startTlsDisabled READ startTlsDisabled WRITE setStartTlsDisabled)

    public:
        Q_INVOKABLE Mail();
        Q_INVOKABLE Mail(const QJSValue &parameters);

		QString username() const                                                    { return QString::fromUtf8(mSmtp.username()); }
        void setUsername(const QString &username)                                   { mSmtp.setUsername(username.toUtf8()); }

		QString password() const                                                    { return QString::fromUtf8(mSmtp.password()); }
        void setPassword(const QString &password)                                   { mSmtp.setPassword(password.toUtf8()); }

        void setOnConnected(const QJSValue &onConnected)                        { mOnConnected = onConnected; }
        QJSValue onConnected() const                                            { return mOnConnected; }

        void setOnConnectionFailed(const QJSValue &onConnectionFailed)          { mOnConnectionFailed = onConnectionFailed; }
        QJSValue onConnectionFailed() const                                     { return mOnConnectionFailed; }

        void setOnEncrypted(const QJSValue &onEncrypted)                        { mOnEncrypted = onEncrypted; }
        QJSValue onEncrypted() const                                            { return mOnEncrypted; }

        void setOnEncryptionFailed(const QJSValue &onEncryptionFailed)          { mOnEncryptionFailed = onEncryptionFailed; }
        QJSValue onEncryptionFailed() const                                     { return mOnEncryptionFailed; }

        void setOnAuthenticated(const QJSValue &onAuthenticated)                { mOnAuthenticated = onAuthenticated; }
        QJSValue onAuthenticated() const                                        { return mOnAuthenticated; }

        void setOnAuthenticationFailed(const QJSValue &onAuthenticationFailed)	{ mOnAuthenticationFailed = onAuthenticationFailed; }
        QJSValue onAuthenticationFailed() const                                 { return mOnAuthenticationFailed; }

        void setOnSenderRejected(const QJSValue &onSenderRejected)              { mOnSenderRejected = onSenderRejected; }
        QJSValue onSenderRejected() const                                       { return mOnSenderRejected; }

        void setOnRecipientRejected(const QJSValue &onRecipientRejected)        { mOnRecipientRejected = onRecipientRejected; }
        QJSValue onRecipientRejected() const                                    { return mOnRecipientRejected; }

        void setOnMailFailed(const QJSValue &onMailFailed)                      { mOnMailFailed = onMailFailed; }
        QJSValue onMailFailed() const                                           { return mOnMailFailed; }

        void setOnMailSent(const QJSValue &onMailSent)                          { mOnMailSent = onMailSent; }
        QJSValue onMailSent() const                                             { return mOnMailSent; }

        void setOnFinished(const QJSValue &onFinished)                          { mOnFinished = onFinished; }
        QJSValue onFinished() const                                             { return mOnFinished; }

        void setOnDisconnected(const QJSValue &onDisconnected)                  { mOnDisconnected = onDisconnected; }
        QJSValue onDisconnected() const                                         { return mOnDisconnected; }

        int pendingMessageCount() const                                             { return mSmtp.pendingMessages(); }

        bool startTlsDisabled() const                                               { return mSmtp.startTlsDisabled(); }
        void setStartTlsDisabled(bool disable)                                      { mSmtp.setStartTlsDisabled(disable); }

        Q_INVOKABLE QString toString() const override                                                    { return QStringLiteral("Mail"); }
        Q_INVOKABLE Mail *connectToServer(const QString &serverName, int port = 25);
        Q_INVOKABLE Mail *connectToSecureServer(const QString &serverName, int port = 465);
        Q_INVOKABLE int send(const QJSValue &mail);
        Q_INVOKABLE Mail *disconnectFromServer()                                         { mSmtp.disconnectFromHost(); return this; }
        Q_INVOKABLE bool hasExtension(const QString &extension)                                 { return mSmtp.hasExtension(extension); }
        Q_INVOKABLE QString extensionData(const QString &extension)                             { return mSmtp.extensionData(extension); }
        Q_INVOKABLE Mail *waitForConnected(int waitTime = 30000);
        Q_INVOKABLE Mail *waitForEncrypted(int waitTime = 30000);
        Q_INVOKABLE Mail *waitForAuthenticated(int waitTime = 30000);
        Q_INVOKABLE Mail *waitForFinished(int waitTime = 30000);
        Q_INVOKABLE Mail *waitForDisconnected(int waitTime = 30000);

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

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
        QJSValue mOnConnected;
        QJSValue mOnConnectionFailed;
        QJSValue mOnEncrypted;
        QJSValue mOnEncryptionFailed;
        QJSValue mOnAuthenticated;
        QJSValue mOnAuthenticationFailed;
        QJSValue mOnSenderRejected;
        QJSValue mOnRecipientRejected;
        QJSValue mOnMailFailed;
        QJSValue mOnMailSent;
        QJSValue mOnFinished;
        QJSValue mOnDisconnected;
    };
}

