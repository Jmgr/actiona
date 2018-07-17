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

	Contact : jmgr@jmgr.info
*/

#pragma once

#include "actioninstance.h"
#include "../mousedevice.h"
#include "stringlistpair.h"

namespace Actions
{
	class ClickInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
	
	public:
		enum Action
		{
			ClickAction,
			PressAction,
			ReleaseAction
		};
		enum Exceptions
		{
			FailedToSendInputException = ActionTools::ActionException::UserException,
			InvalidActionException
		};
	
		ClickInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent)										{}
	
        static Tools::StringListPair buttons;
        static Tools::StringListPair actions;
	
		void startExecution() override;
		void stopLongTermExecution() override;
	
	private:
		MouseDevice mMouseDevice;
	
		Q_DISABLE_COPY(ClickInstance)
	};
}

