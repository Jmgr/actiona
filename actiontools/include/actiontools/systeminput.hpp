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

#include <QMetaType>

namespace ActionTools
{
	namespace SystemInput
	{
		enum Button
		{
			LeftButton,
			MiddleButton,
			RightButton,
			XButton0,
			XButton1,
			XButton2,
			XButton3,
			XButton4,
			XButton5,
			XButton6,
			XButton7,
			XButton8,
			XButton9
		};
	}
}

Q_DECLARE_METATYPE(ActionTools::SystemInput::Button)

