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

#include <QPoint>
#include <QObject>

class MouseDevice : public QObject
{
	Q_OBJECT

public:
	enum Button
	{
		LeftButton,
		MiddleButton,
		RightButton,

		ButtonCount
	};
	Q_ENUM(Button)

	MouseDevice();
    ~MouseDevice() override;

	void reset();

	bool isButtonPressed(Button button) const;
	QPoint cursorPosition() const;
	void setCursorPosition(const QPoint &position) const;

	bool buttonClick(Button button);
	bool pressButton(Button button);
	bool releaseButton(Button button);
	
	bool wheel(int intensity = 1) const;

private:
#ifdef Q_OS_UNIX
	int toX11Button(Button button) const;
#endif
#ifdef Q_OS_WIN
	int toWinButton(Button button, bool press) const;
#endif

	bool mPressedButtons[ButtonCount];
};

