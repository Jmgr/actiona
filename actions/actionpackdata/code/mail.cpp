/*
    Actiona
    Copyright (C) 2005-2017 Jonathan Mercier-Ganady

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

            if(it.name() == "username")
                mail->setUsername(it.value().toString());
            else if(it.name() == "password")
                mail->setPassword(it.value().toString());
            else if(it.name() == "onConnected")
                mail->mOnConnected = it.value();
            else if(it.name() == "onConnectionFailed")
                mail->mOnConnectionFailed = it.value();
            else if(it.name() == "onEncrypted")
                mail->mOnEncrypted = it.value();
            else if(it.name() == "onEncryptionFailed")
                mail->mOnEncryptionFailed = it.value();
            else if(it.name() == "onAuthenticated")
                mail->mOnAuthenticated = it.value();
            else if(it.name() == "onAuthenticationFailed")
                mail->mOnAuthenticationFailed = it.value();
            else if(it.name() == "onSenderRejected")
                mail->mOnSenderRejected = it.value();
            else if(it.name() == "onRecipientRejected")
                mail->mOnRecipientRejected = it.value();
            else if(it.name() == "onMailFailed")
                mail->mOnMailFailed = it.value();
            else if(it.name() == "onMailSent")
                mail->mOnMailSent = it.value();
            else if(it.name() == "onFinished")
                mail->mOnFinished = it.value();
            else if(it.name() == "onDisconnected")
                mail->mOnDisconnected = it.value();
        }

        return CodeClass::constructor(mail, context, engine);
    }

    Mail::Mail()
        : CodeClass()
    {
        connect(&mSmtp, SIGNAL(connected()), this, SLOT(connected()));
        connect(&mSmtp, SIGNAL(connectionFailed(QByteArray)), this, SLOT(connectionFailed(QByteArray)));
        connect(&mSmtp, SIGNAL(encrypted()), this, SLOT(encrypted()));
        connect(&mSmtp, SIGNAL(encryptionFailed(QByteArray)), this, SLOT(encryptionFailed(QByteArray)));
        connect(&mSmtp, SIGNAL(authenticated()), this, SLOT(authenticated()));
        connect(&mSmtp, SIGNAL(authenticationFailed(QByteArray)), this, SLOT(authenticationFailed(QByteArray)));
        connect(&mSmtp, SIGNAL(senderRejected(int,QString,QByteArray)), this, SLOT(senderRejected(int,QString,QByteArray)));
        connect(&mSmtp, SIGNAL(recipientRejected(int,QString,QByteArray)), this, SLOT(recipientRejected(int,QString,QByteArray)));
        connect(&mSmtp, SIGNAL(mailFailed(int,int,QByteArray)), this, SLOT(mailFailed(int,int,QByteArray)));
        connect(&mSmtp, SIGNAL(mailSent(int)), this, SLOT(mailSent(int)));
        connect(&mSmtp, SIGNAL(finished()), this, SLOT(finished()));
        connect(&mSmtp, SIGNAL(disconnected()), this, SLOT(disconnected()));
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
        if(MailMessage *mailMessage = qobject_cast<MailMessage*>(mail.toQObject()))
        {
            QxtMailMessage &message = mailMessage->message();

            if(!message.hasExtraHeader("Date"))
                message.setExtraHeader("Date", QDateTime::currentDateTime().toString(Qt::RFC2822Date));

            return mSmtp.send(message);
        }
        else
        {
            throwError("ParameterTypeError", tr("Incorrect parameter type: not a MailMessage"));
            return -1;
        }
    }

    QScriptValue Mail::waitForConnected(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(connected()));
        if(!waiter.wait(waitTime))
            throwError("ConnectionError", tr("Cannot establish a connection to the server"));

        return thisObject();
    }

    QScriptValue Mail::waitForEncrypted(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(encrypted()));
        if(!waiter.wait(waitTime))
            throwError("EncryptionError", tr("Cannot encrypt the connection"));

        return thisObject();
    }

    QScriptValue Mail::waitForAuthenticated(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(authenticated()));
        if(!waiter.wait(waitTime))
            throwError("AuthenticationError", tr("Cannot authenticate to the server"));

        return thisObject();
    }

    QScriptValue Mail::waitForFinished(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(finished()));
        if(!waiter.wait(waitTime))
            throwError("WaitForFinishedError", tr("Wait for finished failed"));

        return thisObject();
    }

    QScriptValue Mail::waitForDisconnected(int waitTime)
    {
        QxtSignalWaiter waiter(&mSmtp, SIGNAL(disconnected()));
        if(!waiter.wait(waitTime))
            throwError("WaitForDisconnectedError", tr("Wait for disconnected failed"));

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
