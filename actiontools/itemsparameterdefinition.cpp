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

#include "itemsparameterdefinition.h"

namespace ActionTools
{
	QString ItemsParameterDefinition::originalNameFromTranslatedName(const QString &translation) const
	{
		for(int i=0;i<mItems.second.size();++i)
		{
			if(translation == mItems.second.at(i))
				return mItems.first.at(i);
		}

		return translation;
	}

	QString ItemsParameterDefinition::translatedNameFromOriginalName(const QString &originalName) const
	{
		for(int i=0;i<mItems.first.size();++i)
		{
			if(originalName == mItems.first.at(i))
				return mItems.second.at(i);
		}

		return originalName;
	}
}
