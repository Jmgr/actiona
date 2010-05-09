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

#ifndef ACTIONPACKINTERFACE_H
#define ACTIONPACKINTERFACE_H

#include <QList>
#include "version.h"

namespace ActionTools
{
	class ActionInterface;

	typedef QList<ActionInterface *> ActionInterfaceList;

	class ActionPackInterface
	{
	public:
		ActionPackInterface()			{}
		virtual ~ActionPackInterface()	{}

		//Returns the id of the pack
		virtual QString id() const = 0;

		//Returns the name of the pack
		virtual QString name() const = 0;

		//Returns the pack version
		virtual Tools::Version version() const = 0;

		//Returns an instance of each plugin interface
		const ActionInterfaceList &actionsInterfaces() const { return mActionInterfaces; }

	protected:
		void addActionInterface(ActionInterface *actionInterface)
		{
			mActionInterfaces.append(actionInterface);
		}

	private:
		ActionInterfaceList mActionInterfaces;

		Q_DISABLE_COPY(ActionPackInterface)
	};
}

Q_DECLARE_INTERFACE(ActionTools::ActionPackInterface,
					"info.jmgr.actionaz.ActionPackInterface/1.0")

#endif // ACTIONPACKINTERFACE_H
