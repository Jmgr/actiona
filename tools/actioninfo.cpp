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

#include "actioninfo.h"

QDataStream &operator<<(QDataStream &s, const Tools::ActionInfo &actionInfo)
{
	s << actionInfo.version();
	s << actionInfo.name();
	s << actionInfo.description();

	return s;
}

QDataStream &operator>>(QDataStream &s, Tools::ActionInfo &actionInfo)
{
	Tools::Version version;
	QString name;
	QString description;

	s >> version;
	s >> name;
	s >> description;

	actionInfo.setVersion(version);
	actionInfo.setName(name);
	actionInfo.setDescription(description);

	return s;
}

QDebug &operator<<(QDebug &dbg, const Tools::ActionInfo &actionInfo)
{
	dbg.space() << "Version:" << actionInfo.version();
	dbg.space() << "Name:" << actionInfo.name();
	dbg.space() << "Description:" << actionInfo.description();

	return dbg.maybeSpace();
}

QDataStream &operator<<(QDataStream &s, const Tools::ActionPackInfo &actionPackInfo)
{
	s << actionPackInfo.version();
	s << actionPackInfo.name();
	s << actionPackInfo.description();
	s << actionPackInfo.actions();
	s << actionPackInfo.releaseDate();
	s << actionPackInfo.type();
	s << actionPackInfo.filename();
	s << actionPackInfo.size();
	s << actionPackInfo.hash();

	return s;
}

QDataStream &operator>>(QDataStream &s, Tools::ActionPackInfo &actionPackInfo)
{
	Tools::Version version;
	QString name;
	QString description;
	Tools::ActionInfoList actions;
	QString releaseDate;
	QString type;
	QString filename;
	int size;
	QString hash;

	s >> version;
	s >> name;
	s >> description;
	s >> actions;
	s >> releaseDate;
	s >> type;
	s >> filename;
	s >> size;
	s >> hash;

	actionPackInfo.setVersion(version);
	actionPackInfo.setName(name);
	actionPackInfo.setDescription(description);
	actionPackInfo.setActions(actions);
	actionPackInfo.setReleaseDate(releaseDate);
	actionPackInfo.setType(type);
	actionPackInfo.setFilename(filename);
	actionPackInfo.setSize(size);
	actionPackInfo.setHash(hash);

	return s;
}

QDebug &operator<<(QDebug &dbg, const Tools::ActionPackInfo &actionPackInfo)
{
	dbg.space() << "Version:" << actionPackInfo.version();
	dbg.space() << "Name:" << actionPackInfo.name();
	dbg.space() << "Description" << actionPackInfo.description();
	dbg.space() << "Actions:" << actionPackInfo.actions();
	dbg.space() << "ReleaseDate:" << actionPackInfo.releaseDate();
	dbg.space() << "Type:" << actionPackInfo.type();
	dbg.space() << "Filename:" << actionPackInfo.filename();
	dbg.space() << "Size:" << actionPackInfo.size();
	dbg.space() << "Hash:" << actionPackInfo.hash();

	return dbg.maybeSpace();
}

