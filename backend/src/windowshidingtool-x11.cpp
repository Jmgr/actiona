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

#include "backend/windowshidingtool-x11.hpp"

#include <QApplication>
#include <QWidget>

namespace Backend
{
    WindowsHidingToolX11::WindowsHidingToolX11(QObject *parent):
        WindowsHidingTool(parent)
	{
	}

    WindowsHidingToolX11::~WindowsHidingToolX11()
	{
	}

    void WindowsHidingToolX11::hide()
    {
        mShownWindows.clear();

        const auto widgets = QApplication::topLevelWidgets();
        for(QWidget *widget: widgets)
        {
            if(widget->isVisible())
            {
                mShownWindows.append(VisibleWindow{widget, widget->pos()});

                widget->hide();
            }
        }
    }

    void WindowsHidingToolX11::show()
    {
        for(auto shownWindow: qAsConst(mShownWindows))
        {
            shownWindow.window->show();
            shownWindow.window->move(shownWindow.position);
        }
    }
}
