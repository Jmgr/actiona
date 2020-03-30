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

#include <QString>

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT Name
	{
	public:
        Name() = default;
		Name(const QString &original, const QString &translated)
			: mOriginal(original),
			  mTranslated(translated)
		{
		}
        Name(const Name &other) = default;

		void setOriginal(const QString &original) { mOriginal = original; }
		void setTranslated(const QString &translated) { mTranslated = translated; }

		const QString &original() const { return mOriginal; }
		const QString &translated() const { return mTranslated; }

		Name &operator=(Name other)
		{
			swap(other);

			return *this;
		}

		void swap(Name &other)
		{
			std::swap(mOriginal, other.mOriginal);
			std::swap(mTranslated, other.mTranslated);
		}

	private:
		QString mOriginal;
		QString mTranslated;
	};
}

