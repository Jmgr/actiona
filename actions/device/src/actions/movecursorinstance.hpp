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

#include "actiontools/actioninstance.hpp"
#include "../mousedevice.hpp"

namespace Actions
{
	class MoveCursorInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
	
	public:
		MoveCursorInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent)										{}
	
		void startExecution() override
		{
			bool ok = true;
		
			QPoint position        = evaluatePoint(ok, QStringLiteral("position"));
			QPoint positionOffset = evaluatePoint(ok, QStringLiteral("positionOffset"));

			if(!ok)
				return;
			
			position += positionOffset;
			mMouseDevice.setCursorPosition(position);
		
			executionEnded();
		}

		void stopLongTermExecution() override
		{
			mMouseDevice.reset();
		}
	
	private:
		MouseDevice mMouseDevice;
		
		Q_DISABLE_COPY(MoveCursorInstance)
	};
}

