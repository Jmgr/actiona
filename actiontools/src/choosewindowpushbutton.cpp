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
#include "actiontools/choosewindowpushbutton.hpp"
#include "actiontools/windowhandle.hpp"
#include "backend/windowing.hpp"

namespace ActionTools
{
    ChooseWindowPushButton::ChooseWindowPushButton(QWidget *parent):
        QPushButton(parent),
        mChooser(Backend::Instance::windowing().createWindowChooser(this))
	{
        setIcon(QIcon(QStringLiteral(":/images/cross.png")));

#ifdef Q_OS_WIN
        setToolTip(tr("Target a window by clicking this button, moving the cursor to the wanted window and releasing the mouse button."));
#endif

#ifdef Q_OS_WIN
        connect(this, &QPushButton::pressed, mChooser, &Backend::WindowChooser::choose);
#else
        connect(this, &QPushButton::clicked, mChooser, &Backend::WindowChooser::choose);
#endif

        connect(mChooser, &Backend::WindowChooser::done, this, [this](const WId &window){ emit searchEnded(WindowHandle(window)); });
        connect(mChooser, &Backend::WindowChooser::canceled, this, &ChooseWindowPushButton::canceled);
        connect(mChooser, &Backend::WindowChooser::errorOccurred, this, [this](const Backend::BackendError &e){ emit errorOccurred(e.what()); });
	}

	ChooseWindowPushButton::~ChooseWindowPushButton()
	{
	}
}
