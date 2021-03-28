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

namespace ActionTools
{
	ChoosePositionPushButton::ChoosePositionPushButton(QWidget *parent)
    : QPushButton(parent),
      mChooser(Backend::Instance::windowing().createPositionChooser(this))
	{
        setIcon(QIcon(QStringLiteral(":/images/cross.png")));

#ifdef Q_OS_WIN
		setToolTip(tr("Target a position by clicking this button, moving the cursor to the desired position and releasing the mouse button."));
#endif

#ifdef Q_OS_WIN
        connect(this, &QPushButton::pressed, mChooser, &Backend::PositionChooser::choose);
#else
        connect(this, &QPushButton::clicked, mChooser, &Backend::PositionChooser::choose);
#endif

        connect(mChooser, &Backend::PositionChooser::done, this, &ChoosePositionPushButton::positionChosen);
        connect(mChooser, &Backend::PositionChooser::canceled, this, &ChoosePositionPushButton::canceled);
        connect(mChooser, &Backend::PositionChooser::errorOccurred, this, [this](const Backend::BackendError &e){ emit errorOccurred(e.what()); });
	}

	ChoosePositionPushButton::~ChoosePositionPushButton()
	{
    }
}
