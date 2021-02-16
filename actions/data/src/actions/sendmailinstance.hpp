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

#include "actiontools/actioninstance.hpp"
#include "actiontools/script.hpp"
#include "tools/stringlistpair.hpp"

class QxtSmtp;
class QProgressDialog;

namespace Actions
{
    class SendMailInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
        enum AttachmentContentType
		{
            AutoDetectAttachmentContentType,
            BinaryAttachmentContentType,
            TextAttachmentContentType,
            JpgImageAttachmentContentType,
            PngImageAttachmentContentType
		};
        Q_ENUM(AttachmentContentType)
		enum Exceptions
		{
            ConnectionErrorException = ActionTools::ActionException::UserException,
            ErrorWhileSendingEMailException
		};

        SendMailInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr);
        ~SendMailInstance() override;

        static Tools::StringListPair attachmentContentTypes;

		void startExecution() override;
		void stopExecution() override;

	private slots:
        void connectionFailed(const QByteArray &msg);
        void encryptionFailed(const QByteArray &msg);
        void authenticationFailed(const QByteArray &msg);
        void authenticated();
        void senderRejected(int mailID, const QString &address, const QByteArray &msg);
        void recipientRejected(int mailID, const QString &address, const QByteArray &msg);
        void mailFailed(int mailID, int errorCode, const QByteArray &msg);
        void mailSent(int mailID);
        void disconnected();
        void canceled();

	private:
        Q_DISABLE_COPY(SendMailInstance)

        void cleanup();

        QxtSmtp *mSmtp;
		QProgressDialog *mProgressDialog;
	};
}

