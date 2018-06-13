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

#include "actiontools_global.h"

#include <QSharedData>
#include <QVariant>

namespace ActionTools
{
	class SubParameterData : public QSharedData
	{
	public:
        SubParameterData()                                              = default;
        SubParameterData(const SubParameterData &other)                 = default;

		bool code{false};
        QString value;
	};

	class ACTIONTOOLSSHARED_EXPORT SubParameter
	{
	public:
        SubParameter(bool code = false, const QString &value = QString())
			: d(new SubParameterData())
		{
			setCode(code);
			setValue(value);
		}
        SubParameter(const SubParameter &other)                         = default;

		bool isCode() const												{ return d->code; }
        QString value() const											{ return d->value; }

		void setCode(bool code)											{ d->code = code; }
        void setValue(const QString &value)                             { d->value = value; }

		bool operator == (const SubParameter &other) const				{ return (isCode() == other.isCode() && value() == other.value()); }
		bool operator != (const SubParameter &other) const				{ return (isCode() != other.isCode() || value() != other.value()); }

	private:
		QSharedDataPointer<SubParameterData> d;
	};

    ACTIONTOOLSSHARED_EXPORT QDataStream &operator << (QDataStream &s, const SubParameter &subParameter);
    ACTIONTOOLSSHARED_EXPORT QDataStream &operator >> (QDataStream &s, SubParameter &subParameter);
	ACTIONTOOLSSHARED_EXPORT QDebug &operator << (QDebug &dbg, const SubParameter &subParameter);
}

Q_DECLARE_METATYPE(ActionTools::SubParameter)

