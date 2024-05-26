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

#include "actiontools/actionpack.hpp"
#include "data_global.hpp"
#include "actions/writetextfiledefinition.hpp"
#include "actions/readtextfiledefinition.hpp"
#include "actions/writeinifiledefinition.hpp"
#include "actions/readinifiledefinition.hpp"
#include "actions/readregistrydefinition.hpp"
#include "actions/writeregistrydefinition.hpp"
#include "actions/readclipboarddefinition.hpp"
#include "actions/writeclipboarddefinition.hpp"
#include "actions/readbinaryfiledefinition.hpp"
#include "actions/writebinaryfiledefinition.hpp"
#include "actions/readenvironmentdefinition.hpp"
#include "actions/copyfiledefinition.hpp"
#include "actions/webdownloaddefinition.hpp"
#include "actions/sendmaildefinition.hpp"

#include "code/file.hpp"
#include "code/clipboard.hpp"
#include "code/registry.hpp"
#include "code/inifile.hpp"
#include "code/udp.hpp"
#include "code/tcp.hpp"
#include "code/sql.hpp"
#include "code/tcpserver.hpp"
#include "code/web.hpp"
#include "code/mailattachment.hpp"
#include "code/mailmessage.hpp"
#include "code/mail.hpp"

namespace ActionTools
{
	class ActionDefinition;
}

class ACTIONPACKDATA_EXPORT ActionPackData : public QObject, public ActionTools::ActionPack
{
	Q_OBJECT

public:
	ActionPackData()								= default;

	void createDefinitions() override
	{
		addActionDefinition(new Actions::WriteTextFileDefinition(this));
		addActionDefinition(new Actions::ReadTextFileDefinition(this));
		addActionDefinition(new Actions::WriteIniFileDefinition(this));
		addActionDefinition(new Actions::ReadIniFileDefinition(this));
		addActionDefinition(new Actions::ReadRegistryDefinition(this));
		addActionDefinition(new Actions::WriteRegistryDefinition(this));
		addActionDefinition(new Actions::ReadClipboardDefinition(this));
		addActionDefinition(new Actions::WriteClipboardDefinition(this));
		addActionDefinition(new Actions::ReadBinaryFileDefinition(this));
		addActionDefinition(new Actions::WriteBinaryFileDefinition(this));
		addActionDefinition(new Actions::ReadEnvironmentVariableDefinition(this));
		addActionDefinition(new Actions::CopyFileDefinition(this));
		addActionDefinition(new Actions::WebDownloadDefinition(this));
        addActionDefinition(new Actions::SendMailDefinition(this));
	}

	QString id() const override								{ return QStringLiteral("data"); }
	QString name() const override							{ return tr("Data related actions"); }

    void codeInit(ActionTools::ScriptEngine &scriptEngine) const override
	{
		Code::File::registerClass(scriptEngine);
        Code::Clipboard::registerClass(scriptEngine);
        Code::Registry::registerClass(scriptEngine);
        Code::IniFile::registerClass(scriptEngine);
        Code::Udp::registerClass(scriptEngine);
        Code::Tcp::registerClass(scriptEngine);
        Code::Sql::registerClass(scriptEngine);
		Code::Sql::registerClass(scriptEngine);
        Code::TcpServer::registerClass(scriptEngine);
        Code::Web::registerClass(scriptEngine);
        Code::MailAttachment::registerClass(scriptEngine);
        Code::MailMessage::registerClass(scriptEngine);
        Code::Mail::registerClass(scriptEngine);
	}

private:
	Q_DISABLE_COPY(ActionPackData)
};

