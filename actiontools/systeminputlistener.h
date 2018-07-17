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

#include "systeminput.h"
#include "actiontools_global.h"

namespace ActionTools
{
	namespace SystemInput
	{
		class ACTIONTOOLSSHARED_EXPORT Listener
		{
		public:
			virtual void keyboardEvent() const {}
			virtual void mouseMotion(int x, int y) { Q_UNUSED(x); Q_UNUSED(y); }
			virtual void mouseWheel(int intensity) { Q_UNUSED(intensity); }
			virtual void mouseButtonPressed(Button button) { Q_UNUSED(button); }
			virtual void mouseButtonReleased(Button button) { Q_UNUSED(button); }

		protected:
			Listener() = default;
			~Listener() = default;
		};
	}
}

