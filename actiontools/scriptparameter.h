/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#ifndef SCRIPTPARAMETER_H
#define SCRIPTPARAMETER_H

#include "actiontools_global.h"

#include <QObject>
#include <QString>
#include <QSharedData>

namespace ActionTools
{
	class ScriptParameterData : public QSharedData
	{
	public:
		ScriptParameterData() : code(false)				{}
		ScriptParameterData(const ScriptParameterData &other) :
			QSharedData(other),
			name(other.name),
			value(other.value),
			code(other.code)							{}

		QString name;
		QString value;
		bool code;
	};

	class ACTIONTOOLSSHARED_EXPORT ScriptParameter
	{
	public:
		ScriptParameter(const QString &name, const QString &value, bool code)
			: d(new ScriptParameterData())
		{
			setName(name);
			setValue(value);
			setCode(code);
		}
		ScriptParameter(const ScriptParameter &other)
			: d(other.d)								{}

		void setName(const QString &name)				{ d->name = name; }
		void setValue(const QString &value)				{ d->value = value; }
		void setCode(bool code)							{ d->code = code; }

		const QString &name() const						{ return d->name; }
		const QString &value() const					{ return d->value; }
		bool isCode() const								{ return d->code; }

		bool operator == (const ScriptParameter &other) const
		{
			return (name() == other.name() && value() == other.value() && isCode() == other.isCode());
		}

	private:
		QSharedDataPointer<ScriptParameterData> d;
	};
}

#endif // SCRIPTPARAMETER_H
