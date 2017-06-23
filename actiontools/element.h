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
#include "actiontools_global.h"

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT Element : public QObject
	{
		Q_OBJECT

	public:
		enum Category
		{
			INPUT,
			OUTPUT,
			OTHER
		};

		Element(Category category, const QString &name, const QString &translatedName, QObject *parent);

		QString translatedName() const							{ return mTranslatedName; }
		QString name() const									{ return mName; }
		QString tooltip() const									{ return mTooltip; }
		Category category() const								{ return mCategory; }

		void setTranslatedName(const QString &translatedName)   { mTranslatedName = translatedName; }
		void setName(const QString &name)						{ mName = name; }
		void setTooltip(const QString &tooltip)					{ mTooltip = tooltip; }

	private:
		QString mName;
		QString mTranslatedName;
		QString mTooltip;
		Category mCategory;

		Q_DISABLE_COPY(Element)
	};
}

