/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#ifndef ACTIONPACKOTHER_H
#define ACTIONPACKOTHER_H

#include "actionpack.h"
#include "actionreadtextfiledefinition.h"
#include "actionwriteinifiledefinition.h"
#include "actionreadinifiledefinition.h"
#include "actionreadregistrydefinition.h"
#include "actionwriteregistrydefinition.h"
#include "actionreadclipboarddefinition.h"
#include "actionwriteclipboarddefinition.h"
#include "actionreadbinaryfiledefinition.h"
#include "actionwritebinaryfiledefinition.h"
#include "actioncopyfiledefinition.h"

#include "code/file.h"
#include "code/clipboard.h"
#include "code/registry.h"
#include "code/inifile.h"
#include "code/udp.h"
#include "code/tcp.h"
#include "code/sql.h"

#include <QtCore/qplugin.h>

namespace ActionTools
{
	class ActionDefinition;
}

class ActionPackOther : public QObject, public ActionTools::ActionPack
{
	Q_OBJECT
	Q_INTERFACES(ActionTools::ActionPack)

public:
	ActionPackOther()
	{
		addActionDefinition(new ActionReadTextFileDefinition(this));
		addActionDefinition(new ActionWriteIniFileDefinition(this));
		addActionDefinition(new ActionReadIniFileDefinition(this));
		addActionDefinition(new ActionReadRegistryDefinition(this));
		addActionDefinition(new ActionWriteRegistryDefinition(this));
		addActionDefinition(new ActionReadClipboardDefinition(this));
		addActionDefinition(new ActionWriteClipboardDefinition(this));
		addActionDefinition(new ActionReadBinaryFileDefinition(this));
		addActionDefinition(new ActionWriteBinaryFileDefinition(this));
		addActionDefinition(new ActionCopyFileDefinition(this));
	}

	QString id() const								{ return tr("other"); }
	QString name() const							{ return tr("Other actions"); }
	Tools::Version version() const					{ return Tools::Version(0, 0, 1); }
	
	void codeInit(QScriptEngine *scriptEngine) const
	{
		addCodeClass<Code::File>("File", scriptEngine);
		addCodeClass<Code::Clipboard>("Clipboard", scriptEngine);
		addCodeClass<Code::Registry>("Registry", scriptEngine);
		addCodeClass<Code::IniFile>("IniFile", scriptEngine);
		addCodeClass<Code::Udp>("Udp", scriptEngine);
		addCodeClass<Code::Tcp>("Tcp", scriptEngine);
		addCodeClass<Code::Sql>("Sql", scriptEngine);
	}

private:
	Q_DISABLE_COPY(ActionPackOther)
};

Q_EXPORT_PLUGIN2(ActionPackOther, ActionPackOther)

#endif // ACTIONPACKINTERNAL_H
