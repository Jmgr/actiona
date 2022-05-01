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

#include "backend_global.hpp"
#include "backend/windowing.hpp"

#include <QList>

class QWidget;

namespace Backend
{
    class BACKENDSHARED_EXPORT WindowsHidingToolX11 : public WindowsHidingTool
	{
		Q_OBJECT
        Q_DISABLE_COPY(WindowsHidingToolX11)

	public:
        explicit WindowsHidingToolX11(QObject *parent);
        ~WindowsHidingToolX11() override;

        void hide() override;
        void show() override;

	private:
        struct VisibleWindow
        {
            QWidget *window;
            QPoint position;
        };

        QList<VisibleWindow> mShownWindows;
	};
}
