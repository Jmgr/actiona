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
#include "readclipboardinstance.hpp"
#include "actiontools/textparameterdefinition.hpp"
#include "actiontools/variableparameterdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ReadClipboardDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit ReadClipboardDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            auto &output = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("variable"), tr("Variable")});
            output.setTooltip(tr("The variable where to save the current clipboard value"));
		}

		QString name() const override													{ return QObject::tr("Read clipboard"); }
		QString id() const override														{ return QStringLiteral("ActionReadClipboard"); }
		QString description() const override												{ return QObject::tr("Read the clipboard contents"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new ReadClipboardInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Data; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/readclipboard.png")); }

	private:
		Q_DISABLE_COPY(ReadClipboardDefinition)
	};
}

