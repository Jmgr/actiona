/*
	Actiona
	Copyright (C) 2008-2015 Jonathan Mercier-Ganady

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

#ifndef SENDMAILDEFINITION_H
#define SENDMAILDEFINITION_H

#include "actiondefinition.h"
#include "sendmailinstance.h"
#include "textparameterdefinition.h"
#include "booleanparameterdefinition.h"
#include "fileparameterdefinition.h"
#include "listparameterdefinition.h"
#include "numberparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
    class SendMailDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
        explicit SendMailDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            translateItems("SendMailInstance::attachmentContentTypes", SendMailInstance::attachmentContentTypes);

            ActionTools::TextParameterDefinition *serverName = new ActionTools::TextParameterDefinition(ActionTools::Name("serverName", tr("Server name")), this);
            serverName->setTooltip(tr("The name or IP of the SMTP server to use\nExample: mail.example.com"));
            addElement(serverName);

            ActionTools::BooleanParameterDefinition *secureConnection  = new ActionTools::BooleanParameterDefinition(ActionTools::Name("secureConnection", tr("Secure connection")), this);
            secureConnection->setTooltip(tr("Secure (encrypted) connection to the server"));
            secureConnection->setDefaultValue(true);
            addElement(secureConnection);

            ActionTools::TextParameterDefinition *userName = new ActionTools::TextParameterDefinition(ActionTools::Name("userName", tr("User name")), this);
            userName->setTooltip(tr("The user (or login) name to use\nMany SMTP servers use the e-mail address as user name"));
            addElement(userName);

            ActionTools::TextParameterDefinition *password = new ActionTools::TextParameterDefinition(ActionTools::Name("password", tr("Password")), this);
            password->setTooltip(tr("The password to use"));
            addElement(password);

            ActionTools::TextParameterDefinition *sender = new ActionTools::TextParameterDefinition(ActionTools::Name("sender", tr("Sender")), this);
            sender->setTooltip(tr("The sender's e-mail address"));
            addElement(sender);

            ActionTools::TextParameterDefinition *receivers = new ActionTools::TextParameterDefinition(ActionTools::Name("receivers", tr("Receivers")), this);
            receivers->setTooltip(tr("The receiver's e-mail address\nCan contain multiple receivers separated with a comma"));
            addElement(receivers);

            ActionTools::TextParameterDefinition *subject = new ActionTools::TextParameterDefinition(ActionTools::Name("subject", tr("Subject")), this);
            subject->setTooltip(tr("The e-mail's subject"));
            addElement(subject);

            ActionTools::TextParameterDefinition *body = new ActionTools::TextParameterDefinition(ActionTools::Name("body", tr("Body")), this);
            body->setTooltip(tr("The e-mail's body"));
            addElement(body);

            ActionTools::TextParameterDefinition *attachmentName = new ActionTools::TextParameterDefinition(ActionTools::Name("attachmentName", tr("Attachment name")), this);
            attachmentName->setTooltip(tr("The attachment name, an empty name means that there is no attachment"));
            addElement(attachmentName, 1);

            ActionTools::TextParameterDefinition *attachmentData = new ActionTools::TextParameterDefinition(ActionTools::Name("attachmentData", tr("Attachment data")), this);
            attachmentData->setTooltip(tr("The data to attach to the e-mail"));
            addElement(attachmentData, 1);

            ActionTools::ListParameterDefinition *attachmentContentType = new ActionTools::ListParameterDefinition(ActionTools::Name("attachmentContentType", tr("Attachment content type")), this);
            attachmentContentType->setTooltip(tr("The MIME content type of the attachment\nIf left empty, Actiona will try to deduce it from the data type"));
            attachmentContentType->setItems(SendMailInstance::attachmentContentTypes);
            attachmentContentType->setDefaultValue(SendMailInstance::attachmentContentTypes.second.at(SendMailInstance::AutoDetectAttachmentContentType));
            addElement(attachmentContentType, 1);

            ActionTools::TextParameterDefinition *carbonCopy = new ActionTools::TextParameterDefinition(ActionTools::Name("carbonCopy", tr("Carbon copy (Cc)")), this);
            carbonCopy->setTooltip(tr("The carbon copy receiver's e-mail address\nCan contain multiple receivers separated with a comma"));
            addElement(carbonCopy, 2);

            ActionTools::TextParameterDefinition *blindCarbonCopy = new ActionTools::TextParameterDefinition(ActionTools::Name("blindCarbonCopy", tr("Blind carbon copy (Bcc)")), this);
            blindCarbonCopy->setTooltip(tr("The blind carbon copy receiver's e-mail address\nCan contain multiple receivers separated with a comma"));
            addElement(blindCarbonCopy, 2);

            ActionTools::NumberParameterDefinition *serverPort = new ActionTools::NumberParameterDefinition(ActionTools::Name("serverPort", tr("Server port")), this);
            serverPort->setTooltip(tr("The server port"));
            serverPort->setMinimum(0);
            serverPort->setMaximum(65535);
            serverPort->setDefaultValue(0);
            serverPort->setSpecialValueText(tr("Default port"));
            addElement(serverPort, 2);

            ActionTools::TextParameterDefinition *extraHeaders = new ActionTools::TextParameterDefinition(ActionTools::Name("extraHeaders", tr("Extra headers")), this);
            extraHeaders->setTooltip(tr("Extra headers to add to the e-mail\nShould contain one header per line"));
            addElement(extraHeaders, 2);

            addException(SendMailInstance::ConnectionErrorException, tr("Connection error"));
            addException(SendMailInstance::ErrorWhileSendingEMailException, tr("Error while sending e-mail"));
		}

        QString name() const													{ return QObject::tr("Send e-mail"); }
        QString id() const														{ return "ActionSendMail"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
        QString description() const												{ return QObject::tr("Sends an e-mail"); }
        ActionTools::ActionInstance *newActionInstance() const					{ return new SendMailInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Data; }
        QPixmap icon() const													{ return QPixmap(":/icons/sendmail.png"); }
        QStringList tabs() const												{ return QStringList() << ActionDefinition::StandardTabs.at(0) << tr("Attachment") << ActionDefinition::StandardTabs.at(1); }

	private:
        Q_DISABLE_COPY(SendMailDefinition)
	};
}

#endif // SENDMAILDEFINITION_H
