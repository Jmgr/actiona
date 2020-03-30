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

    Contact : jmgr@jmgr.info
*/

#pragma once

#include "actiontools_global.hpp"

#include <QString>

namespace ActionTools
{
    class ACTIONTOOLSSHARED_EXPORT NumberFormat
    {
    public:
        static QString sizeString(quint64 size);
        static QString labelIndexString(int rowIndex);

    private:
        static QString numberFormat(const QStringList &units, double value);

        NumberFormat() = default;
    };
}

