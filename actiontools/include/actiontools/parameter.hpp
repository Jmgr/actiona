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

#include "subparameter.hpp"
#include "actiontools_global.hpp"

#include <QSharedData>

namespace ActionTools
{
	using SubParameterHash = QMap<QString, SubParameter>;

	class ParameterData : public QSharedData
	{
	public:
		ParameterData()																= default;
        ParameterData(const ParameterData &other)                                   = default;
        ParameterData &operator=(const ParameterData &other)                        = default;

		SubParameterHash subParameters;
	};

	class ACTIONTOOLSSHARED_EXPORT Parameter
	{
	public:
		Parameter()																	{ d = new ParameterData(); }
        Parameter(const Parameter &other)                                           = default;
        Parameter &operator=(const Parameter &other)                                = default;

		const SubParameterHash &subParameters() const								{ return d->subParameters; }
		SubParameterHash &subParameters()											{ return d->subParameters; }
		void setSubParameter(const QString &name, const SubParameter &subParameter)	{ d->subParameters.insert(name, subParameter); }
        void setSubParameter(const QString &name, bool code, const QString &value)	{ setSubParameter(name, SubParameter(code, value)); }
		void setSubParameters(const SubParameterHash &subParameters)				{ d->subParameters = subParameters; }
		SubParameter subParameter(const QString &name) const						{ return d->subParameters.value(name); }

		bool operator == (const Parameter &other) const								{ return (subParameters() == other.subParameters()); }
		bool operator != (const Parameter &other) const								{ return (subParameters() != other.subParameters()); }

	private:
		QSharedDataPointer<ParameterData> d;
	};

    ACTIONTOOLSSHARED_EXPORT QDataStream &operator << (QDataStream &s, const Parameter &parameter);
    ACTIONTOOLSSHARED_EXPORT QDataStream &operator >> (QDataStream &s, Parameter &parameter);
	ACTIONTOOLSSHARED_EXPORT QDebug &operator << (QDebug &dbg, const Parameter &parameter);
}

Q_DECLARE_METATYPE(ActionTools::Parameter)

