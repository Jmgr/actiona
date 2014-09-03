/*
	Actiona
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef DATETIMEPARAMETERDEFINITION_H
#define DATETIMEPARAMETERDEFINITION_H

#include "parameterdefinition.h"
#include "actiontools_global.h"

namespace ActionTools
{
	class CodeDateTimeEdit;

	class ACTIONTOOLSSHARED_EXPORT DateTimeParameterDefinition : public ParameterDefinition
	{
		Q_OBJECT

	public:
        DateTimeParameterDefinition(const Name &name, QObject *parent);

		void buildEditors(Script *script, QWidget *parent);
		void load(const ActionInstance *actionInstance);
		void save(ActionInstance *actionInstance);
        void setDefaultValues(ActionInstance *actionInstance);
		
	private:
		CodeDateTimeEdit *mDateTimeEdit;

		Q_DISABLE_COPY(DateTimeParameterDefinition)
	};
}

#endif // DATETIMEPARAMETERDEFINITION_H
