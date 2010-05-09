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

#ifndef ACTIONINFO_H
#define ACTIONINFO_H

#include "tools_global.h"
#include "version.h"

#include <QSharedData>
#include <QList>

namespace Tools
{
	class ActionInfoData : public QSharedData
	{
	public:
		ActionInfoData()									{}
		ActionInfoData(const ActionInfoData &other)
			: QSharedData(other),
			version(other.version),
			name(other.name),
			description(other.description)					{}

		Version version;
		QString name;
		QString description;
	};

	class TOOLSSHARED_EXPORT ActionInfo
	{
	public:
		ActionInfo() : d(new ActionInfoData())				{}
		ActionInfo(const ActionInfo &other) : d(other.d)	{}

		void setVersion(const Tools::Version &version)		{ d->version = version; }
		void setName(const QString &name)					{ d->name = name; }
		void setDescription(const QString &description)		{ d->description = description; }

		Version version() const								{ return d->version; }
		QString name() const								{ return d->name; }
		QString description() const							{ return d->description; }

	private:
		QSharedDataPointer<ActionInfoData> d;
	};

	typedef QList<ActionInfo> ActionInfoList;

	class ActionPackInfoData : public QSharedData
	{
	public:
		ActionPackInfoData()								{}
		ActionPackInfoData(const ActionPackInfoData &other)
			: QSharedData(other),
			version(other.version),
			name(other.name),
			description(other.description),
			actions(other.actions),
			releaseDate(other.releaseDate),
			type(other.type),
			changelog(other.changelog),
			filename(other.filename),
			size(other.size),
			hash(other.hash)								{}

		Version version;
		QString name;
		QString description;
		ActionInfoList actions;
		QString releaseDate;
		QString type;
		QString changelog;
		QString filename;
		int size;
		QString hash;
	};

	class TOOLSSHARED_EXPORT ActionPackInfo
	{
	public:
		ActionPackInfo() : d(new ActionPackInfoData())		{}
		ActionPackInfo(const ActionPackInfo &other)
			: d(other.d)									{}

		void setVersion(const Tools::Version &version)		{ d->version = version; }
		void setName(const QString &name)					{ d->name = name; }
		void setDescription(const QString &description)		{ d->description = description; }
		void setActions(const ActionInfoList &actions)		{ d->actions = actions; }
		void setReleaseDate(const QString &releaseDate)		{ d->releaseDate = releaseDate; }
		void setType(const QString &type)					{ d->type = type; }
		void setChangelog(const QString &changelog)			{ d->changelog = changelog; }
		void setFilename(const QString &filename)			{ d->filename = filename; }
		void setSize(int size)								{ d->size = size; }
		void setHash(const QString &hash)					{ d->hash = hash; }

		Version version() const								{ return d->version; }
		QString name() const								{ return d->name; }
		QString description() const							{ return d->description; }
		ActionInfoList actions() const						{ return d->actions; }
		QString releaseDate() const							{ return d->releaseDate; }
		QString type() const								{ return d->type; }
		QString changelog() const							{ return d->changelog; }
		QString filename() const							{ return d->filename; }
		int size() const									{ return d->size; }
		QString hash() const								{ return d->hash; }

	private:
		QSharedDataPointer<ActionPackInfoData> d;
	};

	typedef QList<ActionPackInfo> ActionPackInfoList;
}

TOOLSSHARED_EXPORT QDataStream &operator<<(QDataStream &s, const Tools::ActionInfo &actionInfo);
TOOLSSHARED_EXPORT QDataStream &operator>>(QDataStream &s, Tools::ActionInfo &actionInfo);
QDebug TOOLSSHARED_EXPORT &operator<<(QDebug &dbg, const Tools::ActionInfo &actionInfo);

TOOLSSHARED_EXPORT QDataStream &operator<<(QDataStream &s, const Tools::ActionPackInfo &actionPackInfo);
TOOLSSHARED_EXPORT QDataStream &operator>>(QDataStream &s, Tools::ActionPackInfo &actionPackInfo);
QDebug TOOLSSHARED_EXPORT &operator<<(QDebug &dbg, const Tools::ActionPackInfo &actionPackInfo);

Q_DECLARE_METATYPE(Tools::ActionInfo)
Q_DECLARE_METATYPE(Tools::ActionPackInfo)
Q_DECLARE_METATYPE(Tools::ActionPackInfoList);

#endif // ACTIONINFO_H
