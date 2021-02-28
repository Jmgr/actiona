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

#include "backend/keyboard-output.hpp"

namespace Backend
{
    class BACKENDSHARED_EXPORT KeyboardOutputWindows : public KeyboardOutput
    {
        Q_OBJECT
        Q_DISABLE_COPY(KeyboardOutputWindows)

    public:
        enum Type
        {
            Win32,
            DirectX
        };

        explicit KeyboardOutputWindows(QObject *parent = nullptr);
        void setType(Type type);
        bool pressKey(const QString &key) override;
        bool releaseKey(const QString &key) override;
        bool triggerKey(const QString &key) override;
        bool writeText(const QString &text, int delay = 0, bool noUnicodeCharacters = false) override;

    private:
        Type mType{Win32};
    };
}
