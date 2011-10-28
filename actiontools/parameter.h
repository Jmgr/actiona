/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#ifndef PARAMETER_H
#define PARAMETER_H

#include "subparameter.h"
#include "actiontools_global.h"

#include <QSharedData>

namespace ActionTools
{
	typedef QHash<QString, SubParameter> SubParameterHash;

	class ParameterData : public QSharedData
	{
	public:
		ParameterData()																{}
		ParameterData(const ParameterData &other)
			: QSharedData(other), subParameters(other.subParameters)				{}

		SubParameterHash subParameters;
	};

	class ACTIONTOOLSSHARED_EXPORT Parameter
	{
	public:
		Parameter()																	{ d = new ParameterData(); }
		Parameter(const Parameter &other) : d(other.d)								{}

		const SubParameterHash &subParameters() const								{ return d->subParameters; }
		SubParameterHash &subParameters()											{ return d->subParameters; }
		void setSubParameter(const QString &name, const SubParameter &subParameter)	{ d->subParameters.insert(name, subParameter); }
		void setSubParameter(const QString &name, bool code, const QVariant &value)	{ setSubParameter(name, SubParameter(code, value)); }
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

#endif // PARAMETER_H
