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

#ifndef ITEMSPARAMETERDEFINITION_H
#define ITEMSPARAMETERDEFINITION_H

#include "parameterdefinition.h"
#include "stringlistpair.h"

namespace ActionTools
{
	class ItemsParameterDefinition : public ParameterDefinition
	{
		Q_OBJECT

	public:
		ItemsParameterDefinition(const QString &name, const QString &translatedName, QObject *parent)
			: ParameterDefinition(name, translatedName, parent)						{}

		StringListPair items()																	{ return mItems; }

		void setItems(const StringListPair &items)												{ mItems = items; }

		QString originalNameFromTranslatedName(const QString &translation) const;
		QString translatedNameFromOriginalName(const QString &originalName) const;

	protected:
		StringListPair mItems;
	};
}

#endif // ITEMSPARAMETERDEFINITION_H
