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

#include "actiontools_global.hpp"

#include <QObject>
#include <QString>
#include <QSharedData>

namespace ActionTools
{
	class ScriptParameterData : public QSharedData
	{
	public:
		ScriptParameterData()  	= default;
		ScriptParameterData(const ScriptParameterData &other) = default;

		QString name;
		QString value;
		bool code{false};
		int type{0};
	};

	class ACTIONTOOLSSHARED_EXPORT ScriptParameter
	{
	public:
		enum ParameterType
		{
			Text,
			Number,
			Window,
			File,
			Line
		};

		ScriptParameter(const QString &name, const QString &value, bool code, ParameterType type)
			: d(new ScriptParameterData())
		{
			setName(name);
			setValue(value);
			setCode(code);
			setType(type);
		}
        ScriptParameter(const ScriptParameter &other)            = default;
        ScriptParameter &operator=(const ScriptParameter &other) = default;

        void setName(const QString &name)				{ d->name = name; }
		void setValue(const QString &value)				{ d->value = value; }
		void setCode(bool code)							{ d->code = code; }
		void setType(ParameterType type)				{ d->type = type; }

		const QString &name() const						{ return d->name; }
		const QString &value() const					{ return d->value; }
		bool isCode() const								{ return d->code; }
		ParameterType type() const						{ return static_cast<ParameterType>(d->type); }

		bool operator == (const ScriptParameter &other) const
		{
			return (name() == other.name() && value() == other.value() && isCode() == other.isCode() && type() == other.type());
		}

	private:
		QSharedDataPointer<ScriptParameterData> d;
	};
}

