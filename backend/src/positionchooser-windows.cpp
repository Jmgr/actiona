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

#include "backend/positionchooser-windows.hpp"
#include "backend/mouse.hpp"

#include <QApplication>

#include <Windows.h>

namespace Backend
{
    PositionChooserWindows::PositionChooserWindows(QObject *parent):
        PositionChooser(parent),
        mPreviousCursor(nullptr)
	{
	}

    PositionChooserWindows::~PositionChooserWindows()
	{
        stopMouseCapture();
    }

    void PositionChooserX11::mousePressEvent(QMouseEvent *event)
    {
        mPreviousCursor = SetCursor(LoadCursor(0, IDC_CROSS));
    }

    void PositionChooserX11::mouseReleaseEvent(QMouseEvent *event)
    {
        emit positionChosen(event->globalPos());

        stopMouseCapture();
    }


    void PositionChooserWindows::stopMouseCapture()
	{
        if(mPreviousCursor)
            SetCursor(mPreviousCursor);
    }
}