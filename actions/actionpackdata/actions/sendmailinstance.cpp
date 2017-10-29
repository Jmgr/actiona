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

#include "sendmailinstance.h"
#include "code/rawdata.h"
#include "code/image.h"

#include "qxtsmtp/qxtsmtp.h"
#include "qxtsmtp/qxtmailmessage.h"
#include "qxtsmtp/qxtmailattachment.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProgressDialog>
#include <QRegularExpression>
#include <QScriptValue>
#include <QBuffer>

namespace Actions
{
    ActionTools::StringListPair SendMailInstance::attachmentContentTypes = qMakePair(
            QStringList() << "autoDetect" << "application/octet-stream" << "text/plain" << "image/jpeg" << "image/png",
            QStringList() << QT_TRANSLATE_NOOP("SendMailInstance::attachmentContentTypes", "Auto detect")
                          << QT_TRANSLATE_NOOP("SendMailInstance::attachmentContentTypes", "Binary")
                          << QT_TRANSLATE_NOOP("SendMailInstance::attachmentContentTypes", "Text")
                          << QT_TRANSLATE_NOOP("SendMailInstance::attachmentContentTypes", "Jpg image")
                          << QT_TRANSLATE_NOOP("SendMailInstance::attachmentContentTypes", "Png image"));

    SendMailInstance::SendMailInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
          mSmtp(nullptr),
		  mProgressDialog(new QProgressDialog)
	{
	}

    SendMailInstance::~SendMailInstance()
	{
		delete mProgressDialog;
	}

    void SendMailInstance::startExecution()
	{
		bool ok = true;

        QString serverName = evaluateString(ok, "serverName");
        bool secureConnection = evaluateBoolean(ok, "secureConnection");
        QString userName = evaluateString(ok, "userName");
        QString password = evaluateString(ok, "password");
        QString sender = evaluateString(ok, "sender");
        QString receivers = evaluateString(ok, "receivers");
        QString subject = evaluateString(ok, "subject");
        QString body = evaluateString(ok, "body");
        QString attachmentName = evaluateString(ok, "attachmentName");
        QScriptValue attachmentData = evaluateValue(ok, "attachmentData");
        QString attachmentContentType = evaluateEditableListElement(ok, attachmentContentTypes, "attachmentContentType");
        QString carbonCopy = evaluateString(ok, "carbonCopy");
        QString blindCarbonCopy = evaluateString(ok, "blindCarbonCopy");
        int serverPort = evaluateInteger(ok, "serverPort");
        QString extraHeaders = evaluateString(ok, "extraHeaders");

        if(serverName.isEmpty() || sender.isEmpty() || receivers.isEmpty())
        {
            emit executionEnded();

            return;
        }

        if(attachmentContentType == "autoDetect" && !attachmentName.isEmpty() && attachmentData.isValid())
        {
            bool foundAttachmentContentType = true;

            if(QObject *scriptObject = attachmentData.toQObject())
            {
                if(qobject_cast<Code::RawData*>(scriptObject))
                    attachmentContentType = "application/octet-stream";
                else if(qobject_cast<Code::Image*>(scriptObject))
                {
                    QString lowerAttachmentName = attachmentName.toLower();

                    if(lowerAttachmentName.endsWith(".jpg") || lowerAttachmentName.endsWith(".jpeg"))
                        attachmentContentType = "image/jpeg";
                    else
                        attachmentContentType = "image/png";
                }
                else
                    foundAttachmentContentType = false;
            }
            else if(attachmentData.isString())
                attachmentContentType = "text/plain";
            else
                foundAttachmentContentType = false;

            if(!foundAttachmentContentType)
            {
                setCurrentParameter("attachmentContentType");
                emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Failed to auto detect the attachment content type"));
                return;
            }
        }

        validateParameterRange(ok, serverPort, "serverPort", tr("server port"), 0, 65535);

		if(!ok)
			return;

        mSmtp = new QxtSmtp(this);

        connect(mSmtp, SIGNAL(connectionFailed(QByteArray)), this, SLOT(connectionFailed(QByteArray)));
        connect(mSmtp, SIGNAL(encryptionFailed(QByteArray)), this, SLOT(encryptionFailed(QByteArray)));
        connect(mSmtp, SIGNAL(authenticationFailed(QByteArray)), this, SLOT(authenticationFailed(QByteArray)));
        connect(mSmtp, SIGNAL(authenticated()), this, SLOT(authenticated()));
        connect(mSmtp, SIGNAL(senderRejected(int,QString,QByteArray)), this, SLOT(senderRejected(int,QString,QByteArray)));
        connect(mSmtp, SIGNAL(recipientRejected(int,QString,QByteArray)), this, SLOT(recipientRejected(int,QString,QByteArray)));
        connect(mSmtp, SIGNAL(mailFailed(int,int,QByteArray)), this, SLOT(mailFailed(int,int,QByteArray)));
        connect(mSmtp, SIGNAL(mailSent(int)), this, SLOT(mailSent(int)));
        connect(mSmtp, SIGNAL(disconnected()), this, SLOT(disconnected()));

        connect(mProgressDialog, SIGNAL(canceled()), this, SLOT(canceled()));

        mSmtp->setUsername(userName.toUtf8());
        mSmtp->setPassword(password.toUtf8());

        QxtMailMessage mailMessage;

        mailMessage.setSender(sender);
        mailMessage.setSubject(subject);
        mailMessage.setBody(body);
        mailMessage.setExtraHeader("Date", QDateTime::currentDateTime().toString(Qt::RFC2822Date));

        for(QString receiver: receivers.split(QChar(','), QString::SkipEmptyParts))
            mailMessage.addRecipient(receiver.trimmed());

        for(QString carbonCopyReceiver: carbonCopy.split(QChar(','), QString::SkipEmptyParts))
            mailMessage.addRecipient(carbonCopyReceiver.trimmed(), QxtMailMessage::Cc);

        for(QString blindCarbonCopyReceiver: blindCarbonCopy.split(QChar(','), QString::SkipEmptyParts))
            mailMessage.addRecipient(blindCarbonCopyReceiver.trimmed(), QxtMailMessage::Bcc);

        for(QString extraHeader: extraHeaders.split(QChar('\n'), QString::SkipEmptyParts))
        {
            QStringList extraHeaderParts = extraHeader.split(QChar(':'), QString::SkipEmptyParts);

            if(extraHeaderParts.size() != 2)
            {
                setCurrentParameter("extraHeaders");
                emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Failed to parse an extra header: \"%1\" has to be in the form \"name: value\"").arg(extraHeader));
                return;
            }

            mailMessage.setExtraHeader(extraHeaderParts[0].trimmed(), extraHeaderParts[1]);
        }

        if(!attachmentName.isEmpty())
        {
            if(!attachmentData.isValid())
            {
                setCurrentParameter("attachmentData");
                emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid attachment data"));
                return;
            }

            QByteArray attachmentDataByteArray;

            if(QObject *scriptObject = attachmentData.toQObject())
            {
                if(Code::RawData *codeRawData = qobject_cast<Code::RawData*>(scriptObject))
                    attachmentDataByteArray = codeRawData->byteArray();
                else if(Code::Image *codeImage = qobject_cast<Code::Image*>(scriptObject))
                {
                    QImage image = codeImage->image();
                    QString format;

                    if(attachmentContentType == "image/png")
                        format = "PNG";
                    else if(attachmentContentType == "image/jpeg")
                        format = "JPG";
                    else
                    {
                        setCurrentParameter("attachmentData");
                        emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Unknown image type; supported types are PNG and JPG"));
                        return;
                    }

                    QBuffer buffer;
                    buffer.open(QIODevice::WriteOnly);

                    if(!image.save(&buffer, format.toLatin1()))
                    {
                        setCurrentParameter("attachmentData");
                        emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid image data"));
                        return;
                    }

                    attachmentDataByteArray = buffer.buffer();
                }
                else
                    attachmentDataByteArray = attachmentData.toString().toUtf8();
            }
            else
            {
                if(attachmentContentType == "text/plain")
                    attachmentDataByteArray = attachmentData.toString().toUtf8();
                else
                    attachmentDataByteArray = attachmentData.toVariant().toByteArray();
            }

            mailMessage.addAttachment(attachmentName, QxtMailAttachment(attachmentDataByteArray, attachmentContentType));
        }

        if(serverPort == 0)
        {
            if(secureConnection)
                mSmtp->connectToSecureHost(serverName);
            else
                mSmtp->connectToHost(serverName);
        }
        else
        {
            if(secureConnection)
                mSmtp->connectToSecureHost(serverName, serverPort);
            else
                mSmtp->connectToHost(serverName, serverPort);
        }

        mSmtp->send(mailMessage);

		mProgressDialog->setModal(false);
        mProgressDialog->setWindowTitle(tr("Sending e-mail"));
        mProgressDialog->setLabelText(tr("Connecting..."));
        mProgressDialog->setMaximum(2);
		mProgressDialog->show();
	}

    void SendMailInstance::stopExecution()
	{
        cleanup();
	}

    void SendMailInstance::connectionFailed(const QByteArray &msg)
    {
        cleanup();

        emit executionException(ConnectionErrorException, tr("Connection failed: %1").arg(QString::fromUtf8(msg)));
    }

    void SendMailInstance::encryptionFailed(const QByteArray &msg)
    {
        cleanup();

        emit executionException(ConnectionErrorException, tr("Encryption failed: %1").arg(QString::fromUtf8(msg)));
    }

    void SendMailInstance::authenticationFailed(const QByteArray &msg)
    {
        cleanup();

        emit executionException(ConnectionErrorException, tr("Authentication failed: %1").arg(QString::fromUtf8(msg)));
    }

    void SendMailInstance::authenticated()
    {
        mProgressDialog->setValue(1);
        mProgressDialog->setLabelText(tr("Sending e-mail..."));
    }

    void SendMailInstance::senderRejected(int mailID, const QString &address, const QByteArray &msg)
    {
        Q_UNUSED(mailID)

        cleanup();

        emit executionException(ErrorWhileSendingEMailException, tr("Sender %1 has been rejected: %2").arg(address).arg(QString::fromUtf8(msg)));
    }

    void SendMailInstance::recipientRejected(int mailID, const QString &address, const QByteArray &msg)
    {
        Q_UNUSED(mailID)

        cleanup();

        emit executionException(ErrorWhileSendingEMailException, tr("Recipient %1 has been rejected: %2").arg(address).arg(QString::fromUtf8(msg)));
    }

    void SendMailInstance::mailFailed(int mailID, int errorCode, const QByteArray &msg)
    {
        Q_UNUSED(mailID)

        cleanup();

        QString errorString;

        switch(errorCode)
        {
        case QxtSmtp::NoRecipients:
            errorString = tr("No recipients");
            break;
        case QxtSmtp::CommandUnrecognized:
            errorString = tr("Command unrecognized");
            break;
        case QxtSmtp::SyntaxError:
            errorString = tr("Syntax error");
            break;
        case QxtSmtp::CommandNotImplemented:
            errorString = tr("Command not implemented");
            break;
        case QxtSmtp::BadSequence:
            errorString = tr("Bad sequence");
            break;
        case QxtSmtp::ParameterNotImplemented:
            errorString = tr("Parameter not implemented");
            break;
        case QxtSmtp::MailboxUnavailable:
            errorString = tr("Mailbox unavailable");
            break;
        case QxtSmtp::UserNotLocal:
            errorString = tr("User not local");
            break;
        case QxtSmtp::MessageTooLarge:
            errorString = tr("Message is too large");
            break;
        case QxtSmtp::InvalidMailboxName:
            errorString = tr("Invalid mailbox name");
            break;
        case QxtSmtp::TransactionFailed:
            errorString = tr("Transaction failed");
            break;
        default:
            errorString = tr("Unknown error");
            break;
        }

        emit executionException(ErrorWhileSendingEMailException, tr("Failed sending e-mail: %1 (%2)").arg(errorString).arg(QString::fromUtf8(msg)));
    }

    void SendMailInstance::mailSent(int mailID)
    {
        Q_UNUSED(mailID)

        cleanup();

        emit executionEnded();
    }

    void SendMailInstance::disconnected()
    {
        mSmtp->deleteLater();
    }

    void SendMailInstance::canceled()
    {
        cleanup();

        emit executionEnded();
    }

    void SendMailInstance::cleanup()
    {
        mSmtp->disconnect();
        mSmtp->disconnectFromHost();

        mProgressDialog->disconnect();
        mProgressDialog->close();
    }
}
