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

#include "actiontools/subparameter.hpp"

#include <QDebug>
#include <QDataStream>

namespace ActionTools
{
	QDataStream &operator << (QDataStream &s, const SubParameter &subParameter)
	{
		s << subParameter.isCode();
		s << subParameter.value();

		return s;
	}

	QDataStream &operator >> (QDataStream &s, SubParameter &subParameter)
	{
		bool code;
        QString value;

		s >> code;
		s >> value;

		subParameter.setCode(code);
		subParameter.setValue(value);

		return s;
	}

	QDebug &operator << (QDebug &dbg, const SubParameter &subParameter)
	{
		dbg.space() << "Code:" << subParameter.isCode();
		dbg.space() << "Value:" << subParameter.value();

		return dbg.maybeSpace();
	}
}
