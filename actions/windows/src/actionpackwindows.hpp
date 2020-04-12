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
#include "actions/messageboxdefinition.hpp"
#include "actions/datainputdefinition.hpp"
#include "actions/windowconditiondefinition.hpp"
#include "actions/windowdefinition.hpp"
#include "actions/multidatainputdefinition.hpp"

#include "code/messagebox.hpp"
#include "code/inputdialog.hpp"
#include "code/progressdialog.hpp"
#include "code/colordialog.hpp"
#include "code/filedialog.hpp"

#include <QtCore/qplugin.h>

namespace ActionTools
{
	class ActionDefinition;
}

class ActionPackWindows : public QObject, public ActionTools::ActionPack
{
	Q_OBJECT
	Q_INTERFACES(ActionTools::ActionPack)
    Q_PLUGIN_METADATA(IID "tools.actiona.ActionPack" FILE "windows.json")

public:
	ActionPackWindows()							= default;

	void createDefinitions() override
	{
		addActionDefinition(new Actions::MessageBoxDefinition(this));
		addActionDefinition(new Actions::DataInputDefinition(this));
		addActionDefinition(new Actions::WindowConditionDefinition(this));
		addActionDefinition(new Actions::WindowDefinition(this));
		addActionDefinition(new Actions::MultiDataInputDefinition(this));
	}

	QString id() const override							{ return QStringLiteral("windows"); }
	QString name() const override						{ return tr("Actions dealing with windows"); }
	QVersionNumber version() const override				{ return QVersionNumber(0, 0, 1); }

	void codeInit(QScriptEngine *scriptEngine) const override
	{
		addCodeClass<Code::MessageBox>(QStringLiteral("MessageBox"), scriptEngine);
		addCodeClass<Code::InputDialog>(QStringLiteral("InputDialog"), scriptEngine);
		addCodeClass<Code::ProgressDialog>(QStringLiteral("ProgressDialog"), scriptEngine);
		addCodeClass<Code::ColorDialog>(QStringLiteral("ColorDialog"), scriptEngine);
		addCodeClass<Code::FileDialog>(QStringLiteral("FileDialog"), scriptEngine);
	}

private:
	Q_DISABLE_COPY(ActionPackWindows)
};

