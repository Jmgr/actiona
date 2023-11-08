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

	Contact: jmgr@jmgr.info
*/

#pragma once

#include "actiontools/actiondefinition.hpp"
#include "sendmailinstance.hpp"
#include "actiontools/textparameterdefinition.hpp"
#include "actiontools/booleanparameterdefinition.hpp"
#include "actiontools/fileparameterdefinition.hpp"
#include "actiontools/listparameterdefinition.hpp"
#include "actiontools/numberparameterdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
    class SendMailDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
        explicit SendMailDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            translateItems("SendMailInstance::attachmentContentTypes", SendMailInstance::attachmentContentTypes);

			auto &serverName = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("serverName"), tr("Server name")});
            serverName.setTooltip(tr("The name or IP of the SMTP server to use\nExample: mail.example.com"));

            auto &secureConnection  = addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("secureConnection"), tr("Secure connection")});
            secureConnection.setTooltip(tr("Secure (encrypted) connection to the server"));
            secureConnection.setDefaultValue(QStringLiteral("true"));

			auto &userName = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("userName"), tr("User name")});
            userName.setTooltip(tr("The user (or login) name to use\nMany SMTP servers use the e-mail address as user name"));

			auto &password = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("password"), tr("Password")});
            password.setTooltip(tr("The password to use"));

			auto &sender = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("sender"), tr("Sender")});
            sender.setTooltip(tr("The sender's e-mail address"));

			auto &receivers = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("receivers"), tr("Receivers")});
            receivers.setTooltip(tr("The receiver's e-mail address\nCan contain multiple receivers separated with a comma"));

			auto &subject = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("subject"), tr("Subject")});
            subject.setTooltip(tr("The e-mail's subject"));

			auto &body = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("body"), tr("Body")});
            body.setTooltip(tr("The e-mail's body"));

            auto &attachmentName = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("attachmentName"), tr("Attachment name")}, 1);
            attachmentName.setTooltip(tr("The attachment name, an empty name means that there is no attachment"));

            auto &attachmentData = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("attachmentData"), tr("Attachment data")}, 1);
            attachmentData.setTooltip(tr("The data to attach to the e-mail"));

            auto &attachmentContentType = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("attachmentContentType"), tr("Attachment content type")}, 1);
            attachmentContentType.setTooltip(tr("The MIME content type of the attachment\nIf left empty, Actiona will try to deduce it from the data type"));
            attachmentContentType.setItems(SendMailInstance::attachmentContentTypes);
            attachmentContentType.setDefaultValue(SendMailInstance::attachmentContentTypes.second.at(SendMailInstance::AutoDetectAttachmentContentType));

            auto &carbonCopy = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("carbonCopy"), tr("Carbon copy (Cc)")}, 2);
            carbonCopy.setTooltip(tr("The carbon copy receiver's e-mail address\nCan contain multiple receivers separated with a comma"));

            auto &blindCarbonCopy = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("blindCarbonCopy"), tr("Blind carbon copy (Bcc)")}, 2);
            blindCarbonCopy.setTooltip(tr("The blind carbon copy receiver's e-mail address\nCan contain multiple receivers separated with a comma"));

            auto &serverPort = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("serverPort"), tr("Server port")}, 2);
            serverPort.setTooltip(tr("The server port"));
            serverPort.setMinimum(0);
            serverPort.setMaximum(65535);
            serverPort.setDefaultValue(QStringLiteral("0"));
            serverPort.setSpecialValueText(tr("Default port"));

            auto &extraHeaders = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("extraHeaders"), tr("Extra headers")}, 2);
            extraHeaders.setTooltip(tr("Extra headers to add to the e-mail\nShould contain one header per line"));

            addException(SendMailInstance::ConnectionErrorException, tr("Connection error"));
            addException(SendMailInstance::ErrorWhileSendingEMailException, tr("Error while sending e-mail"));
		}

        QString name() const override													{ return QObject::tr("Send e-mail"); }
		QString id() const override														{ return QStringLiteral("ActionSendMail"); }
        QString description() const override											{ return QObject::tr("Sends an e-mail"); }
        ActionTools::ActionInstance *newActionInstance() const override					{ return new SendMailInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Data; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/sendmail.png")); }
        QStringList tabs() const override												{ return {ActionDefinition::StandardTabs.at(0), tr("Attachment"), ActionDefinition::StandardTabs.at(1)}; }

	private:
        Q_DISABLE_COPY(SendMailDefinition)
	};
}

