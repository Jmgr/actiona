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

#include "actiontools/choosepositionpushbutton.hpp"
#include "backend/windowing.hpp"

#include <QMouseEvent>

namespace ActionTools
{
    ChoosePositionPushButton::ChoosePositionPushButton(QWidget *parent)
    : QPushButton(parent),
      mPositionChooser(Backend::Instance::windowing().createPositionChooser(this)),
      mWindowsHidingTool(Backend::Instance::windowing().createWindowsHidingTool(this))
    {
        setIcon(QIcon(QStringLiteral(":/images/cross.png")));

        connect(mPositionChooser, &Backend::PositionChooser::positionChosen, this, [this](const QPoint &position){
            mWindowsHidingTool->show();

            emit positionChosen(position);
        });
        connect(mPositionChooser, &Backend::PositionChooser::errorOccurred, this, [this](const Backend::BackendError &error){
            emit errorOccurred(error.what());
        });

        setToolTip(tr("Target a position by clicking this button, moving the cursor to the desired position and releasing the mouse button."));
    }

    ChoosePositionPushButton::~ChoosePositionPushButton()
    {
    }

    void ChoosePositionPushButton::mousePressEvent(QMouseEvent *event)
    {
        QPushButton::mousePressEvent(event);

        mWindowsHidingTool->hide();
        mPositionChooser->mousePressEvent(event);
    }

    void ChoosePositionPushButton::mouseReleaseEvent(QMouseEvent *event)
    {
        mPositionChooser->mouseReleaseEvent(event);
        
        QPushButton::mouseReleaseEvent(event);
    }
}
