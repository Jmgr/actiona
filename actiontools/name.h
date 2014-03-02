/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef NAME_H
#define NAME_H

#include "actiontools_global.h"

#include <QString>

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT Name
	{
	public:
		Name()
		{
		}
		Name(const QString &original, const QString &translated)
			: mOriginal(original),
			  mTranslated(translated)
		{
		}
		Name(const Name &other)
			: mOriginal(other.mOriginal),
			  mTranslated(other.mTranslated)
		{
		}

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

#endif // NAME_H
