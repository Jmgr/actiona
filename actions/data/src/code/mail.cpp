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

#include "mail.hpp"
#include "mailmessage.hpp"
#include "qxtcore/qxtsignalwaiter.h"

#include <QJSValueIterator>
#include <QDateTime>

namespace Code
{
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

    Mail::Mail(const QJSValue &parameters)
        : Mail()
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

            if(it.name() == QLatin1String("username"))
                setUsername(it.value().toString());
            else if(it.name() == QLatin1String("password"))
                setPassword(it.value().toString());
            else if(it.name() == QLatin1String("onConnected"))
                mOnConnected = it.value();
            else if(it.name() == QLatin1String("onConnectionFailed"))
                mOnConnectionFailed = it.value();
            else if(it.name() == QLatin1String("onEncrypted"))
                mOnEncrypted = it.value();
            else if(it.name() == QLatin1String("onEncryptionFailed"))
                mOnEncryptionFailed = it.value();
            else if(it.name() == QLatin1String("onAuthenticated"))
                mOnAuthenticated = it.value();
            else if(it.name() == QLatin1String("onAuthenticationFailed"))
                mOnAuthenticationFailed = it.value();
            else if(it.name() == QLatin1String("onSenderRejected"))
                mOnSenderRejected = it.value();
            else if(it.name() == QLatin1String("onRecipientRejected"))
                mOnRecipientRejected = it.value();
            else if(it.name() == QLatin1String("onMailFailed"))
                mOnMailFailed = it.value();
            else if(it.name() == QLatin1String("onMailSent"))
                mOnMailSent = it.value();
            else if(it.name() == QLatin1String("onFinished"))
                mOnFinished = it.value();
            else if(it.name() == QLatin1String("onDisconnected"))
                mOnDisconnected = it.value();
        }
    }

    Mail *Mail::connectToServer(const QString &serverName, int port)
    {
        mSmtp.connectToHost(serverName, port);

        return this;
    }

    Mail *Mail::connectToSecureServer(const QString &serverName, int port)
    {
        mSmtp.connectToSecureHost(serverName, port);

        return this;
    }

    int Mail::send(const QJSValue &mail)
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

    Mail *Mail::waitForConnected(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(connected()));
        if(!waiter.wait(waitTime))
			throwError(QStringLiteral("ConnectionError"), tr("Cannot establish a connection to the server"));

        return this;
    }

    Mail *Mail::waitForEncrypted(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(encrypted()));
        if(!waiter.wait(waitTime))
			throwError(QStringLiteral("EncryptionError"), tr("Cannot encrypt the connection"));

        return this;
    }

    Mail *Mail::waitForAuthenticated(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(authenticated()));
        if(!waiter.wait(waitTime))
			throwError(QStringLiteral("AuthenticationError"), tr("Cannot authenticate to the server"));

        return this;
    }

    Mail *Mail::waitForFinished(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(finished()));
        if(!waiter.wait(waitTime))
			throwError(QStringLiteral("WaitForFinishedError"), tr("Wait for finished failed"));

        return this;
    }

    Mail *Mail::waitForDisconnected(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(disconnected()));
        if(!waiter.wait(waitTime))
			throwError(QStringLiteral("WaitForDisconnectedError"), tr("Wait for disconnected failed"));

        return this;
    }

    void Mail::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Mail*>("const Mail *");

        CodeClass::registerClass<Mail>(QStringLiteral("Mail"), scriptEngine);
    }

    void Mail::connected()
    {
        if(!mOnConnected.isUndefined())
            mOnConnected.call();
    }

    void Mail::connectionFailed(const QByteArray &msg)
    {
        if(!mOnConnectionFailed.isUndefined())
            mOnConnectionFailed.call(QJSValueList() << QString::fromUtf8(msg));
    }

    void Mail::encrypted()
    {
        if(!mOnEncrypted.isUndefined())
            mOnEncrypted.call();
    }

    void Mail::encryptionFailed(const QByteArray &msg)
    {
        if(!mOnEncryptionFailed.isUndefined())
            mOnEncryptionFailed.call(QJSValueList() << QString::fromUtf8(msg));
    }

    void Mail::authenticated()
    {
        if(!mOnAuthenticated.isUndefined())
            mOnAuthenticated.call();
    }

    void Mail::authenticationFailed(const QByteArray &msg)
    {
        if(!mOnAuthenticationFailed.isUndefined())
            mOnAuthenticationFailed.call(QJSValueList() << QString::fromUtf8(msg));
    }

    void Mail::senderRejected(int mailID, const QString &address, const QByteArray &msg)
    {
        if(!mOnSenderRejected.isUndefined())
            mOnSenderRejected.call(QJSValueList() << mailID << address << QString::fromUtf8(msg));
    }

    void Mail::recipientRejected(int mailID, const QString &address, const QByteArray &msg)
    {
        if(!mOnRecipientRejected.isUndefined())
            mOnRecipientRejected.call(QJSValueList() << mailID << address << QString::fromUtf8(msg));
    }

    void Mail::mailFailed(int mailID, int errorCode, const QByteArray &msg)
    {
        if(!mOnMailFailed.isUndefined())
            mOnMailFailed.call(QJSValueList() << mailID << errorCode << QString::fromUtf8(msg));
    }

    void Mail::mailSent(int mailID)
    {
        if(!mOnMailSent.isUndefined())
            mOnMailSent.call(QJSValueList() << mailID);
    }

    void Mail::finished()
    {
        if(!mOnFinished.isUndefined())
            mOnFinished.call();
    }

    void Mail::disconnected()
    {
        if(!mOnDisconnected.isUndefined())
            mOnDisconnected.call();
    }
}
