/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#ifndef ACTIONPACKDATA_H
#define ACTIONPACKDATA_H

#include "actionpack.h"
#include "actions/writetextfiledefinition.h"
#include "actions/readtextfiledefinition.h"
#include "actions/writeinifiledefinition.h"
#include "actions/readinifiledefinition.h"
#include "actions/readregistrydefinition.h"
#include "actions/writeregistrydefinition.h"
#include "actions/readclipboarddefinition.h"
#include "actions/writeclipboarddefinition.h"
#include "actions/readbinaryfiledefinition.h"
#include "actions/writebinaryfiledefinition.h"
#include "actions/readenvironmentdefinition.h"
#include "actions/copyfiledefinition.h"
#include "actions/webdownloaddefinition.h"

#include "code/file.h"
#include "code/clipboard.h"
#include "code/registry.h"
#include "code/inifile.h"
#include "code/udp.h"
#include "code/tcp.h"
#include "code/sql.h"
#include "code/tcpserver.h"
#include "code/web.h"

#include <QtCore/qplugin.h>

namespace ActionTools
{
	class ActionDefinition;
}

class ActionPackData : public QObject, public ActionTools::ActionPack
{
	Q_OBJECT
	Q_INTERFACES(ActionTools::ActionPack)

public:
	ActionPackData()								{}

	void createDefinitions()
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
	}

	QString id() const								{ return "data"; }
	QString name() const							{ return tr("Data related actions"); }
	Tools::Version version() const					{ return Tools::Version(0, 0, 1); }
	
	void codeInit(QScriptEngine *scriptEngine) const
	{
		addCodeClass<Code::File>("File", scriptEngine);
		Code::File::registerClass(scriptEngine);
		addCodeClass<Code::Clipboard>("Clipboard", scriptEngine);
		addCodeClass<Code::Registry>("Registry", scriptEngine);
		addCodeClass<Code::IniFile>("IniFile", scriptEngine);
		addCodeClass<Code::Udp>("Udp", scriptEngine);
		addCodeClass<Code::Tcp>("Tcp", scriptEngine);
		addCodeClass<Code::Sql>("Sql", scriptEngine);
		Code::Sql::registerClass(scriptEngine);
		addCodeClass<Code::TcpServer>("TcpServer", scriptEngine);
		addCodeClass<Code::Web>("Web", scriptEngine);
	}

private:
	Q_DISABLE_COPY(ActionPackData)
};

Q_EXPORT_PLUGIN2(ActionPackData, ActionPackData)

#endif // ACTIONPACKDATA_H
