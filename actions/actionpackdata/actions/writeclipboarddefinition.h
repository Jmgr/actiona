/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef WRITECLIPBOARDDEFINITION_H
#define WRITECLIPBOARDDEFINITION_H

#include "actiondefinition.h"
#include "writeclipboardinstance.h"
#include "textparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class WriteClipboardDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit WriteClipboardDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::TextParameterDefinition *input = new ActionTools::TextParameterDefinition(ActionTools::Name("value", tr("Value")), this);
			input->setTooltip(tr("The new clipboard value"));
			addElement(input);
		}

		QString name() const													{ return QObject::tr("Write clipboard"); }
		QString id() const														{ return "ActionWriteClipboard"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Set the clipboard contents"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new WriteClipboardInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Data; }
		QPixmap icon() const													{ return QPixmap(":/icons/writeclipboard.png"); }

	private:
		Q_DISABLE_COPY(WriteClipboardDefinition)
	};
}

#endif // WRITECLIPBOARDDEFINITION_H
