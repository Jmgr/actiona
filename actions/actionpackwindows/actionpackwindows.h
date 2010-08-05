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

#ifndef ACTIONPACKWINDOWS_H
#define ACTIONPACKWINDOWS_H

#include "actionpack.h"
#include "actionmessageboxdefinition.h"
#include "actiontextinputdefinition.h"
#include "actionwindowconditiondefinition.h"
#include "actionwindowdefinition.h"

#include <QtCore/qplugin.h>

namespace ActionTools
{
	class ActionDefinition;
}

class ActionPackWindows : public QObject, public ActionTools::ActionPack
{
	Q_OBJECT
	Q_INTERFACES(ActionTools::ActionPack)

public:
	ActionPackWindows()
	{
		addActionDefinition(new ActionMessageBoxDefinition(this));
		addActionDefinition(new ActionTextInputDefinition(this));
		addActionDefinition(new ActionWindowConditionDefinition(this));
		addActionDefinition(new ActionWindowDefinition(this));
	}

	QString id() const							{ return tr("windows"); }
	QString name() const						{ return tr("Actions dealing with windows"); }
	Tools::Version version() const				{ return Tools::Version(0, 0, 1); }

private:
	Q_DISABLE_COPY(ActionPackWindows)
};

Q_EXPORT_PLUGIN2(ActionPackWindows, ActionPackWindows)

#endif // ACTIONPACKWINDOWS_H
