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

#include <Qt>

#ifdef Q_OS_UNIX
#include <QSet>
#endif

namespace Backend
{
    class Capabilities final
    {
        Q_DISABLE_COPY(Capabilities)

    public:
        Capabilities();

        bool hasX11() const { return mHasX11; }
        bool hasXTest() const { return mHasXTest; }
        bool hasDBusService(const QString &service) const { return mServices.contains(service); }

    private:
        void detectX11();
        void detectXTest();
        void detectDBus();

        bool mHasX11{};
        bool mHasXTest{};
        bool mHasDBus{};

#ifdef Q_OS_UNIX
        QSet<QString> mServices;
#endif
    };
}