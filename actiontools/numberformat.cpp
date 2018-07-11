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

#include "numberformat.h"

#include <QStringList>
#include <QObject>

namespace ActionTools
{
    QString NumberFormat::sizeString(quint64 size)
    {
        if(size < 1000)
            return QObject::tr("%n byte(s)", "", static_cast<int>(size));

        return numberFormat({QObject::tr("KB"), QObject::tr("MB"), QObject::tr("GB"), QObject::tr("TB")}, static_cast<double>(size));
    }

    QString NumberFormat::labelIndexString(int rowIndex)
    {
        static QStringList lineNumbers;

        int linesToAdd = rowIndex - lineNumbers.size() + 1;
        if(linesToAdd > 0)
        {
            lineNumbers.reserve(lineNumbers.size() + linesToAdd);

            for(int lineIndex = 0; lineIndex < linesToAdd; ++lineIndex)
                lineNumbers.append(QStringLiteral("%1").arg(lineNumbers.size() + 1, 3, 10, QLatin1Char('0')));
        }

        return lineNumbers[rowIndex];
    }

    QString NumberFormat::numberFormat(const QStringList &units, double value)
    {
        Q_ASSERT(!units.isEmpty());

        QStringListIterator it(units);
        QString unit = units.first();

        while(value >= 1000.0 && it.hasNext())
        {
            unit = it.next();
            value /= 1000.0;
        }

		return QString().setNum(value, 'f', 2) + QStringLiteral(" ") + unit;
    }
}
