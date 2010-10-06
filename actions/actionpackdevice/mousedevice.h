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

#ifndef MOUSEDEVICE_H
#define MOUSEDEVICE_H

#include <QPoint>

class MouseDevice
{
public:
	enum Button
	{
		LeftButton,
		RightButton,
		MiddleButton,

		ButtonCount
	};

	MouseDevice();
	~MouseDevice();

	bool isButtonPressed(Button button) const;
	QPoint cursorPosition() const;
	void setCursorPosition(const QPoint &position) const;

	void pressButton(Button button);
	void releaseButton(Button button);

private:
	bool mPressedButtons[ButtonCount];
};

#endif // MOUSEDEVICE_H
