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

#include "mail.h"
#include "mailmessage.h"
#include "qxtcore/qxtsignalwaiter.h"

#include <QScriptValueIterator>
#include <QDateTime>

namespace Code
{
    QScriptValue Mail::constructor(QScriptContext *context, QScriptEngine *engine)
    {
        Mail *mail = new Mail;

        QScriptValueIterator it(context->argument(0));

        while(it.hasNext())
        {
            it.next();

			if(it.name() == QLatin1String("username"))
                mail->setUsername(it.value().toString());
			else if(it.name() == QLatin1String("password"))
                mail->setPassword(it.value().toString());
			else if(it.name() == QLatin1String("onConnected"))
                mail->mOnConnected = it.value();
			else if(it.name() == QLatin1String("onConnectionFailed"))
                mail->mOnConnectionFailed = it.value();
			else if(it.name() == QLatin1String("onEncrypted"))
                mail->mOnEncrypted = it.value();
			else if(it.name() == QLatin1String("onEncryptionFailed"))
                mail->mOnEncryptionFailed = it.value();
			else if(it.name() == QLatin1String("onAuthenticated"))
                mail->mOnAuthenticated = it.value();
			else if(it.name() == QLatin1String("onAuthenticationFailed"))
                mail->mOnAuthenticationFailed = it.value();
			else if(it.name() == QLatin1String("onSenderRejected"))
                mail->mOnSenderRejected = it.value();
			else if(it.name() == QLatin1String("onRecipientRejected"))
                mail->mOnRecipientRejected = it.value();
			else if(it.name() == QLatin1String("onMailFailed"))
                mail->mOnMailFailed = it.value();
			else if(it.name() == QLatin1String("onMailSent"))
                mail->mOnMailSent = it.value();
			else if(it.name() == QLatin1String("onFinished"))
                mail->mOnFinished = it.value();
			else if(it.name() == QLatin1String("onDisconnected"))
                mail->mOnDisconnected = it.value();
        }

        return CodeClass::constructor(mail, context, engine);
    }

    Mail::Mail()
        : CodeClass()
    {
        connect(&mSmtp, &QxtSmtp::connected, this, &Mail::connected);
        connect(&mSmtp, static_cast<void (QxtSmtp::*)(const QByteArray &)>(&QxtSmtp::connectionFailed), this, &Mail::connectionFailed);
        connect(&mSmtp, &QxtSmtp::encrypted, this, &Mail::encrypted);
        connect(&mSmtp, static_cast<void (QxtSmtp::*)(const QByteArray &)>(&QxtSmtp::encryptionFailed), this, &Mail::encryptionFailed);
        connect(&mSmtp, &QxtSmtp::authenticated, this, &Mail::authenticated);
        connect(&mSmtp, static_cast<void (QxtSmtp::*)(const QByteArray &)>(&QxtSmtp::authenticationFailed), this, &Mail::authenticationFailed);
        connect(&mSmtp, static_cast<void (QxtSmtp::*)(int, const QString &, const QByteArray &)>(&QxtSmtp::senderRejected), this, &Mail::senderRejected);
        connect(&mSmtp, static_cast<void (QxtSmtp::*)(int, const QString &, const QByteArray &)>(&QxtSmtp::recipientRejected), this, &Mail::recipientRejected);
        connect(&mSmtp, static_cast<void (QxtSmtp::*)(int, int, const QByteArray &)>(&QxtSmtp::mailFailed), this, &Mail::mailFailed);
        connect(&mSmtp, static_cast<void (QxtSmtp::*)(int)>(&QxtSmtp::mailSent), this, &Mail::mailSent);
        connect(&mSmtp, &QxtSmtp::finished, this, &Mail::finished);
        connect(&mSmtp, &QxtSmtp::disconnected, this, &Mail::disconnected);
    }

    QScriptValue Mail::connectToServer(const QString &serverName, int port)
    {
        mSmtp.connectToHost(serverName, port);

        return thisObject();
    }

    QScriptValue Mail::connectToSecureServer(const QString &serverName, int port)
    {
        mSmtp.connectToSecureHost(serverName, port);

        return thisObject();
    }

    int Mail::send(const QScriptValue &mail)
    {
        if(auto mailMessage = qobject_cast<MailMessage*>(mail.toQObject()))
        {
            QxtMailMessage &message = mailMessage->message();

			if(!message.hasExtraHeader(QStringLiteral("Date")))
				message.setExtraHeader(QStringLiteral("Date"), QDateTime::currentDateTime().toString(Qt::RFC2822Date));

            return mSmtp.send(message);
        }
        else
        {
			throwError(QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type: not a MailMessage"));
            return -1;
        }
    }

    QScriptValue Mail::waitForConnected(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(connected()));
        if(!waiter.wait(waitTime))
			throwError(QStringLiteral("ConnectionError"), tr("Cannot establish a connection to the server"));

        return thisObject();
    }

    QScriptValue Mail::waitForEncrypted(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(encrypted()));
        if(!waiter.wait(waitTime))
			throwError(QStringLiteral("EncryptionError"), tr("Cannot encrypt the connection"));

        return thisObject();
    }

    QScriptValue Mail::waitForAuthenticated(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(authenticated()));
        if(!waiter.wait(waitTime))
			throwError(QStringLiteral("AuthenticationError"), tr("Cannot authenticate to the server"));

        return thisObject();
    }

    QScriptValue Mail::waitForFinished(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(finished()));
        if(!waiter.wait(waitTime))
			throwError(QStringLiteral("WaitForFinishedError"), tr("Wait for finished failed"));

        return thisObject();
    }

    QScriptValue Mail::waitForDisconnected(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(disconnected()));
        if(!waiter.wait(waitTime))
			throwError(QStringLiteral("WaitForDisconnectedError"), tr("Wait for disconnected failed"));

        return thisObject();
    }

    void Mail::connected()
    {
        if(mOnConnected.isValid())
            mOnConnected.call(thisObject());
    }

    void Mail::connectionFailed(const QByteArray &msg)
    {
        if(mOnConnectionFailed.isValid())
            mOnConnectionFailed.call(thisObject(), QScriptValueList() << QString::fromUtf8(msg));
    }

    void Mail::encrypted()
    {
        if(mOnEncrypted.isValid())
            mOnEncrypted.call(thisObject());
    }

    void Mail::encryptionFailed(const QByteArray &msg)
    {
        if(mOnEncryptionFailed.isValid())
            mOnEncryptionFailed.call(thisObject(), QScriptValueList() << QString::fromUtf8(msg));
    }

    void Mail::authenticated()
    {
        if(mOnAuthenticated.isValid())
            mOnAuthenticated.call(thisObject());
    }

    void Mail::authenticationFailed(const QByteArray &msg)
    {
        if(mOnAuthenticationFailed.isValid())
            mOnAuthenticationFailed.call(thisObject(), QScriptValueList() << QString::fromUtf8(msg));
    }

    void Mail::senderRejected(int mailID, const QString &address, const QByteArray &msg)
    {
        if(mOnSenderRejected.isValid())
            mOnSenderRejected.call(thisObject(), QScriptValueList() << mailID << address << QString::fromUtf8(msg));
    }

    void Mail::recipientRejected(int mailID, const QString &address, const QByteArray &msg)
    {
        if(mOnRecipientRejected.isValid())
            mOnRecipientRejected.call(thisObject(), QScriptValueList() << mailID << address << QString::fromUtf8(msg));
    }

    void Mail::mailFailed(int mailID, int errorCode, const QByteArray &msg)
    {
        if(mOnMailFailed.isValid())
            mOnMailFailed.call(thisObject(), QScriptValueList() << mailID << errorCode << QString::fromUtf8(msg));
    }

    void Mail::mailSent(int mailID)
    {
        if(mOnMailSent.isValid())
            mOnMailSent.call(thisObject(), QScriptValueList() << mailID);
    }

    void Mail::finished()
    {
        if(mOnFinished.isValid())
            mOnFinished.call(thisObject());
    }

    void Mail::disconnected()
    {
        if(mOnDisconnected.isValid())
            mOnDisconnected.call(thisObject());
    }
}
