/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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

#ifndef ELEMENTDEFINITION_H
#define ELEMENTDEFINITION_H

#include "actiontools_global.h"
#include "name.h"

#include <QObject>

namespace ActionTools
{
	class ActionInstance;
	
	class ACTIONTOOLSSHARED_EXPORT ElementDefinition : public QObject
	{
		Q_OBJECT

	public:
		enum Category
		{
			INPUT,
			OUTPUT
		};

		ElementDefinition(const Name &name, QObject *parent);
		virtual ~ElementDefinition()							{}

		const Name &name() const								{ return mName; }
		const QString &tooltip() const							{ return mTooltip; }
		Category category() const								{ return mCategory; }
		const QString &description() const						{ return mDescription; }
		int tab() const											{ return mTab; }

		void setName(const Name &name)							{ mName = name; }
		void setTooltip(const QString &tooltip)					{ mTooltip = tooltip; }
		void setCategory(Category category)						{ mCategory = category; }
		void setDescription(const QString &description)			{ mDescription = description; }
		void setTab(int tab)									{ mTab = tab; }
		
		virtual void setDefaultValues(ActionInstance *actionInstance) = 0;

	private:
		Name mName;
		QString mTooltip;
		Category mCategory;
		QString mDescription;
		int mTab;

		Q_DISABLE_COPY(ElementDefinition)
	};
}

#endif // ELEMENTDEFINITION_H
