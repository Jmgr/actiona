/*
	Actiona
	Copyright (C) 2008-2015 Jonathan Mercier-Ganady

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

#include <QObject>
#include <QVariant>

#include "actiontools_global.h"

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT SubParameter : public QObject
	{
		Q_OBJECT

	public:
		SubParameter(QObject *parent, bool code = false, const QVariant &value = QVariant());

		bool isCode() const						{ return mCode; }
		QVariant value() const					{ return mValue; }

		void setCode(bool code)					{ mCode = code; }
		void setValue(const QVariant &value)    { mValue = value; }

	private:
		bool mCode;
		QVariant mValue;

		Q_DISABLE_COPY(SubParameter)
	};
}

