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

#include "version.h"

#if (QT_VERSION < 0x050600)
#include <QRegExp>
#include <QStringList>
#include <QDataStream>

namespace Tools
{
	Version::Version()
		: d(new VersionData())
	{
	}

	Version::Version(const QString &str)
		: d(new VersionData())
	{
		setFromString(str);
	}

	Version::Version(const Version &other)
		: d(other.d)
	{
	}

	Version::Version(int major, int minor, int micro, int nano)
		: d(new VersionData())
	{
		setMajor(major);
		setMinor(minor);
		setMicro(micro);
		setNano(nano);

		setNumberCount(4);
	}

	Version::Version(int major, int minor, int micro)
		: d(new VersionData())
	{
		setMajor(major);
		setMinor(minor);
		setMicro(micro);

		setNumberCount(3);
	}

	Version::Version(int major, int minor)
		: d(new VersionData())
	{
		setMajor(major);
		setMinor(minor);

		setNumberCount(2);
	}

	Version::Version(int major)
		: d(new VersionData())
	{
		setMajor(major);

		setNumberCount(1);
	}

	QString Version::toString() const
	{
		switch(numberCount())
		{
		case 1:
			return QString("%1").arg(d->major);
		case 2:
			return QString("%1.%2").arg(d->major).arg(d->minor);
		case 3:
			return QString("%1.%2.%3").arg(d->major).arg(d->minor).arg(d->micro);
		case 4:
			return QString("%1.%2.%3.%4").arg(d->major).arg(d->minor).arg(d->micro).arg(d->nano);
		default:
			return QString();
		}
	}

	bool Version::setFromString(const QString &str)
	{
		int newNumberCount = str.count('.') + 1;
		if(newNumberCount < 1 || newNumberCount > 4)
		{
			d->major = d->minor = d->micro = d->nano = 0;
			return false;
		}

		setNumberCount(newNumberCount);

		QRegExp regExp(regExpForNumberCount(newNumberCount));

		if(!regExp.exactMatch(str))
		{
			d->major = d->minor = d->micro = d->nano = 0;
			return false;
		}

		const QStringList versionNumbers = regExp.capturedTexts();

		if((versionNumbers.count() - 1) != newNumberCount)
		{
			d->major = d->minor = d->micro = d->nano = 0;
			return false;
		}

		d->major = versionNumbers.at(1).toInt();
		if(newNumberCount >= 2)
			d->minor = versionNumbers.at(2).toInt();
		if(newNumberCount >= 3)
			d->micro = versionNumbers.at(3).toInt();
		if(newNumberCount >= 4)
			d->nano = versionNumbers.at(4).toInt();

		return true;
	}
	
	void Version::swap(Version &other)
	{
		d.swap(other.d);
	}

	bool Version::isValidVersion(const QString &str)
	{
		int newNumberCount = str.count('.') + 1;
		if(newNumberCount < 1 || newNumberCount > 4)
			return false;

		QRegExp regExp(regExpForNumberCount(newNumberCount));

		return regExp.exactMatch(str);
	}

	Version &Version::operator= (Version other)
	{
		swap(other);

		return *this;
	}

	bool Version::operator== (const Version &other) const
	{
		if(d->numberCount != other.d->numberCount)
			return false;

		switch(numberCount())
		{
		case 1:
			return (d->major == other.d->major);
		case 2:
			return (d->major == other.d->major && d->minor == other.d->minor);
		case 3:
			return (d->major == other.d->major && d->minor == other.d->minor && d->micro == other.d->micro);
		case 4:
			return (d->major == other.d->major && d->minor == other.d->minor && d->micro == other.d->micro && d->nano == other.d->nano);
		default:
			return false;
		}
	}

	bool Version::operator!= (const Version &other) const
	{
		return !((*this) == other);
	}

	bool Version::operator< (const Version &other) const
	{
		if(d->numberCount != other.d->numberCount)
			return false;

		switch(numberCount())
		{
		case 1:
			if(d->major < other.d->major)
				return true;

			return false;
		case 2:
			if(d->major < other.d->major)
				return true;
			if(d->major > other.d->major)
				return false;
			if(d->minor < other.d->minor)
				return true;
			if(d->minor > other.d->minor)
				return false;

			return false;
		case 3:
			if(d->major < other.d->major)
				return true;
			if(d->major > other.d->major)
				return false;
			if(d->minor < other.d->minor)
				return true;
			if(d->minor > other.d->minor)
				return false;
			if(d->micro < other.d->micro)
				return true;
			if(d->micro > other.d->micro)
				return false;

			return false;
		case 4:
			if(d->major < other.d->major)
				return true;
			if(d->major > other.d->major)
				return false;
			if(d->minor < other.d->minor)
				return true;
			if(d->minor > other.d->minor)
				return false;
			if(d->micro < other.d->micro)
				return true;
			if(d->micro > other.d->micro)
				return false;
			if(d->nano < other.d->nano)
				return true;
			if(d->nano > other.d->nano)
				return false;

			return false;
		default:
			return false;
		}
	}

	bool Version::operator> (const Version &other) const
	{
		if(d->numberCount != other.d->numberCount)
			return false;

		switch(numberCount())
		{
		case 1:
			if(d->major > other.d->major)
				return true;

			return false;
		case 2:
			if(d->major > other.d->major)
				return true;
			if(d->major < other.d->major)
				return false;
			if(d->minor > other.d->minor)
				return true;
			if(d->minor < other.d->minor)
				return false;

			return false;
		case 3:
			if(d->major > other.d->major)
				return true;
			if(d->major < other.d->major)
				return false;
			if(d->minor > other.d->minor)
				return true;
			if(d->minor < other.d->minor)
				return false;
			if(d->micro > other.d->micro)
				return true;
			if(d->micro < other.d->micro)
				return false;

			return false;
		case 4:
			if(d->major > other.d->major)
				return true;
			if(d->major < other.d->major)
				return false;
			if(d->minor > other.d->minor)
				return true;
			if(d->minor < other.d->minor)
				return false;
			if(d->micro > other.d->micro)
				return true;
			if(d->micro < other.d->micro)
				return false;
			if(d->nano > other.d->nano)
				return true;
			if(d->nano < other.d->nano)
				return false;

			return false;
		default:
			return false;
		}
	}

	bool Version::operator<= (const Version &other) const
	{
		return ((*this) < other || (*this) == other);
	}

	bool Version::operator>= (const Version &other) const
	{
		return ((*this) > other || (*this) == other);
	}

	QString Version::regExpForNumberCount(int numberCount)
	{
		switch(numberCount)
		{
		case 1:
			return "^([0-9]+)$";
		case 2:
			return "^([0-9]+)\\.([0-9]+)$";
		case 3:
			return "^([0-9]+)\\.([0-9]+)\\.([0-9]+)$";
		case 4:
			return "^([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)$";
		default:
			return QString();
		}
	}
}

QDataStream &operator<<(QDataStream &s, const Tools::Version &version)
{
	s << version.major();
	s << version.minor();
	s << version.micro();
	s << version.nano();
	s << version.numberCount();

	return s;
}

QDataStream &operator>>(QDataStream &s, Tools::Version &version)
{
	int major, minor, micro, nano, numberCount;

	s >> major;
	s >> minor;
	s >> micro;
	s >> nano;
	s >> numberCount;

	version.setMajor(major);
	version.setMinor(minor);
	version.setMicro(micro);
	version.setNano(nano);
	version.setNumberCount(numberCount);

	return s;
}

QDebug &operator<<(QDebug &dbg, const Tools::Version &version)
{
	dbg.nospace() << version.toString();

	return dbg.maybeSpace();
}
#endif
