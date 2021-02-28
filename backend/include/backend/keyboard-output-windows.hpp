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

#include "backend/mouse-output.hpp"

namespace Backend
{
    class BACKENDSHARED_EXPORT MouseOutputWindows : public MouseOutput
    {
        Q_OBJECT

    public:
        void setCursorPosition(const QPoint &position) override;
        bool buttonClick(Button button) override;
        bool pressButton(Button button) override;
        bool releaseButton(Button button) override;
        bool wheel(int intensity = 1) override;
    };
}
