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

#include "version.h"

#include <QRegExp>
#include <QStringList>

namespace ActionTools
{
	Version::Version()
		: mMajor(0), mMinor(0), mRelease(0)
	{
	}

	Version::Version(const QString &str)
	{
		setFromString(str);
	}

	Version::Version(const Version &other)
		: mMajor(other.mMajor), mMinor(other.mMinor), mRelease(other.mRelease)
	{
	}

	Version::Version(int major, int minor, int release)
		: mMajor(major), mMinor(minor), mRelease(release)
	{
	}

	QString Version::toString() const
	{
		return QString("%1.%2.%3").arg(mMajor).arg(mMinor).arg(mRelease);
	}

	bool Version::setFromString(const QString &str)
	{
		QRegExp regExp("^([0-9]+)\\.([0-9]+)\\.([0-9]+)$");

		if(!regExp.exactMatch(str))
		{
		mMajor = mMinor = mRelease = 0;
		return false;
		}

		const QStringList versionNumbers = regExp.capturedTexts();

		if(versionNumbers.count() != 4)
		{
		mMajor = mMinor = mRelease = 0;
		return false;
		}

		mMajor = versionNumbers.at(1).toInt();
		mMinor = versionNumbers.at(2).toInt();
		mRelease = versionNumbers.at(3).toInt();

		return true;
	}

	bool Version::isValidVersion(const QString &str)
	{
		QRegExp regExp("([\\d]+)\\.([\\d]+)\\.([\\d]+)");

		return regExp.exactMatch(str);
	}

	Version &Version::operator= (const Version &other)
	{
		if(this == &other)
		return *this;

		mMajor = other.mMajor;
		mMinor = other.mMinor;
		mRelease = other.mRelease;

		return *this;
	}

	bool Version::operator== (const Version &other) const
	{
		return (mMajor == other.mMajor && mMinor == other.mMinor && mRelease == other.mRelease);
	}

	bool Version::operator!= (const Version &other) const
	{
		return !((*this) == other);
	}

	bool Version::operator< (const Version &other) const
	{
		if(mMajor < other.mMajor)
		return true;
		if(mMinor < other.mMinor)
		return true;
		if(mRelease < other.mRelease)
		return true;

		return false;
	}

	bool Version::operator> (const Version &other) const
	{
		if(mMajor > other.mMajor)
		return true;
		if(mMinor > other.mMinor)
		return true;
		if(mRelease > other.mRelease)
		return true;

		return false;
	}

	bool Version::operator<= (const Version &other) const
	{
		return ((*this) < other || (*this) == other);
	}

	bool Version::operator>= (const Version &other) const
	{
		return ((*this) > other || (*this) == other);
	}

	QDebug &operator<<(QDebug &dbg, const Version &version)
	{
		dbg.nospace() << version.toString();

		return dbg.maybeSpace();
	}
}
