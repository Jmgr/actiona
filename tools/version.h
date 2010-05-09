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

#ifndef VERSION_H
#define VERSION_H

#include "tools_global.h"

#include <QString>
#include <QDebug>
#include <QSharedData>
#include <QMetaType>

#ifdef major
#undef major
#endif

#ifdef minor
#undef minor
#endif

namespace Tools
{
	class VersionData : public QSharedData
	{
	public:
		VersionData()
			: major(0),
			minor(0),
			micro(0),
			nano(0),
			numberCount(3)					{}
		VersionData(const VersionData &other)
			: QSharedData(other),
			major(other.major),
			minor(other.minor),
			micro(other.micro),
			nano(other.nano),
			numberCount(other.numberCount)	{}

		int major;
		int minor;
		int micro;
		int nano;
		int numberCount;
	};

	class TOOLSSHARED_EXPORT Version
	{
	public:
		Version();
		explicit Version(const QString &str);
		Version(const Version &other);
		Version(int major, int minor, int micro, int nano);
		Version(int major, int minor, int micro);
		Version(int major, int minor);
		Version(int major);
		~Version()							{}

		int major() const					{ return d->major; }
		int minor() const					{ return d->minor; }
		int micro() const					{ return d->micro; }
		int nano() const					{ return d->nano; }
		int numberCount() const				{ return d->numberCount; }

		void setMajor(int major)			{ d->major = major; if(numberCount() < 1) setNumberCount(1); }
		void setMinor(int minor)			{ d->minor = minor; if(numberCount() < 2) setNumberCount(2); }
		void setMicro(int micro)			{ d->micro = micro; if(numberCount() < 3) setNumberCount(3); }
		void setNano(int nano)				{ d->nano = nano; if(numberCount() < 4) setNumberCount(4); }
		void setNumberCount(int numberCount){ d->numberCount = numberCount; }

		QString toString() const;
		bool setFromString(const QString &str);

		Version& operator= (const Version &other);
		bool operator== (const Version &other) const;
		bool operator!= (const Version &other) const;
		bool operator< (const Version &other) const;
		bool operator> (const Version &other) const;
		bool operator<= (const Version &other) const;
		bool operator>= (const Version &other) const;

		static bool isValidVersion(const QString &str);

	private:
		static QString regExpForNumberCount(int numberCount);

		QSharedDataPointer<VersionData> d;
	};
}

TOOLSSHARED_EXPORT QDataStream &operator<<(QDataStream &s, const Tools::Version &version);
TOOLSSHARED_EXPORT QDataStream &operator>>(QDataStream &s, Tools::Version &version);
QDebug TOOLSSHARED_EXPORT &operator<<(QDebug &dbg, const Tools::Version &version);

Q_DECLARE_METATYPE(Tools::Version)

#endif // VERSION_H
