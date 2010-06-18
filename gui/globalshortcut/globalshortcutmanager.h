/*
 * globalshortcutmanager.h - Class managing global shortcuts
 * Copyright (C) 2006  Maciej Niedzielski
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef GLOBALSHORTCUTMANAGER_H
#define GLOBALSHORTCUTMANAGER_H

#include <QObject>
#include <QKeySequence>
#include <QMap>

class QObject;
class KeyTrigger;

class GlobalShortcutManager : public QObject
{
public:
	static GlobalShortcutManager* instance();
	static void connect(const QKeySequence& key, QObject* receiver, const char* slot);
	static void disconnect(const QKeySequence& key, QObject* receiver, const char* slot);
	static void clear();

private:
	GlobalShortcutManager();
	~GlobalShortcutManager();
	static GlobalShortcutManager* instance_;
	class KeyTrigger;
	QMap<QKeySequence, KeyTrigger*> triggers_;
};

#endif
