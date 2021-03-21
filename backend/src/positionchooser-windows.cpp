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
        PositionChooser(parent)
	{
	}

    PositionChooserWindows::~PositionChooserWindows()
	{
        stopMouseCapture();
    }

    void PositionChooserWindows::choose()
    {
        QCoreApplication::instance()->installNativeEventFilter(this);

        mPreviousCursor = SetCursor(LoadCursor(0, IDC_CROSS));
    }

    bool PositionChooserWindows::nativeEventFilter(const QByteArray &eventType, void *message, long *)
    {
        if(eventType != "windows_generic_MSG")
            return false;

        auto msg = static_cast<MSG*>(message);
        switch(msg->message)
        {
        case WM_KEYDOWN:
            switch(msg->wParam)
            {
            case VK_ESCAPE:
                stopMouseCapture();

                emit canceled();
                break;
            }
            break;
        case WM_LBUTTONUP:
            stopMouseCapture();

            try
            {
                emit done(Instance::mouse().cursorPosition());
            }
            catch(const BackendError &e)
            {
                emit canceled(); // TODO: Error
            }

            break;
        }

        return false;
    }

    void PositionChooserWindows::stopMouseCapture()
	{
        if(mPreviousCursor)
            SetCursor(mPreviousCursor);

        QCoreApplication::instance()->removeNativeEventFilter(this);
    }
}
