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

#ifndef ACTIONPACK_H
#define ACTIONPACK_H

#include <QList>
#include "version.h"

namespace ActionTools
{
	class ActionDefinition;

	typedef QList<ActionDefinition *> ActionDefinitionList;

	class ActionPack
	{
	public:
		ActionPack()			{}
		virtual ~ActionPack()	{}

		//Returns the id of the pack
		virtual QString id() const = 0;

		//Returns the name of the pack
		virtual QString name() const = 0;

		//Returns the pack version
		virtual Tools::Version version() const = 0;

		//Returns an instance of each plugin definition
		const ActionDefinitionList &actionsDefinitions() const	{ return mActionDefinitions; }

		void setFilename(const QString &filename)				{ mFilename = filename; }
		const QString &filename() const							{ return mFilename; }

	protected:
		void addActionDefinition(ActionDefinition *actionDefinition)
		{
			mActionDefinitions.append(actionDefinition);
		}

	private:
		ActionDefinitionList mActionDefinitions;
		QString mFilename;

		Q_DISABLE_COPY(ActionPack)
	};
}

Q_DECLARE_INTERFACE(ActionTools::ActionPack,
					"info.jmgr.actionaz.ActionPack/1.0")

#endif // ACTIONPACK_H
