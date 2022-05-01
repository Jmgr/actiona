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

#include <QMouseEvent>

namespace ActionTools
{
    ChooseWindowPushButton::ChooseWindowPushButton(QWidget *parent)
    : QPushButton(parent),
      mWindowChooser(Backend::Instance::windowing().createWindowChooser(this)),
      mWindowsHidingTool(Backend::Instance::windowing().createWindowsHidingTool(this))
    {
        setIcon(QIcon(QStringLiteral(":/images/cross.png")));

        connect(mWindowChooser, &Backend::WindowChooser::windowChosen, this, [this](WId windowId){
            mWindowsHidingTool->show();

            emit windowChosen(windowId);
        });
        connect(mWindowChooser, &Backend::WindowChooser::errorOccurred, this, [this](const Backend::BackendError &error){
            emit errorOccurred(error.what());
        });

        setToolTip(tr("Target a window by clicking this button, moving the cursor to the wanted window and releasing the mouse button."));
    }

    ChooseWindowPushButton::~ChooseWindowPushButton()
    {
    }

    void ChooseWindowPushButton::mousePressEvent(QMouseEvent *event)
    {
        QPushButton::mousePressEvent(event);

        mWindowsHidingTool->hide();
        mWindowChooser->mousePressEvent(event);
    }

    void ChooseWindowPushButton::mouseReleaseEvent(QMouseEvent *event)
    {
        mWindowChooser->mouseReleaseEvent(event);

        QPushButton::mouseReleaseEvent(event);
    }
}
