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

#ifndef READCLIPBOARDDEFINITION_H
#define READCLIPBOARDDEFINITION_H

#include "actiondefinition.h"
#include "readclipboardinstance.h"
#include "textparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ReadClipboardDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit ReadClipboardDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::VariableParameterDefinition *output = new ActionTools::VariableParameterDefinition("variable", tr("Variable"), this);
			output->setTooltip(tr("The variable where to save the current clipboard value"));
			addElement(output);
		}

		QString name() const													{ return QObject::tr("Read clipboard"); }
		QString id() const														{ return "ActionReadClipboard"; }
		Flag flags() const														{ return ActionDefinition::flags() | Official; }
		QString description() const												{ return QObject::tr("Read the clipboard contents"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new ReadClipboardInstance(this); }
		Category category() const												{ return Data; }
		QPixmap icon() const													{ return QPixmap(":/icons/clipboard.png"); }

	private:
		Q_DISABLE_COPY(ReadClipboardDefinition)
	};
}

#endif // READCLIPBOARDDEFINITION_H
