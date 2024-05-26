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

#include "actiontools/actiontools_global.hpp"
#include "actiontools/code/codeclass.hpp"

class QJSValue;

namespace Code
{
    class ACTIONTOOLSSHARED_EXPORT Algorithms : public CodeClass
    {
        Q_OBJECT

    public:
        Algorithms(QObject *parent): CodeClass(parent) {}

        Q_INVOKABLE QString md4(const QString &input);
        Q_INVOKABLE QString md5(const QString &input);
        Q_INVOKABLE QString sha1(const QString &input);
        Q_INVOKABLE void setRandomSeed(qint32 seed);
        Q_INVOKABLE qint32 randomMax();
        Q_INVOKABLE qint32 randomInteger();
        Q_INVOKABLE qint32 randomInteger(int min, int max);
        Q_INVOKABLE double randomFloat(double min, double max);
        Q_INVOKABLE QString randomString(const QJSValue &parameters);
        Q_INVOKABLE QString toString() const override { return QStringLiteral("Algorithms"); }

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

    private:
        static int randomInteger_(int min, int max);
    };
}
