/*
 * globalshortcutmanager.cpp - Class managing global shortcuts
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

#include "globalshortcutmanager.h"

#include <QCoreApplication>

#include "globalshortcuttrigger.h"

/**
 * \brief Constructs new GlobalShortcutManager.
 */
GlobalShortcutManager::GlobalShortcutManager()
	: QObject(QCoreApplication::instance())
{
}

GlobalShortcutManager::~GlobalShortcutManager()
{
	clear();
}

GlobalShortcutManager* GlobalShortcutManager::instance_ = 0;

/**
 * \brief Returns the instance of GlobalShortcutManager.
 */
GlobalShortcutManager* GlobalShortcutManager::instance()
{
	if (!instance_)
		instance_ = new GlobalShortcutManager();
	return instance_;
}

/**
 * \brief Connects a key sequence with a slot.
 * \param key, global shortcut to be connected
 * \param receiver, object which should receive the notification
 * \param slot, the SLOT() of the \a receiver which should be triggerd if the \a key is activated
 */
void GlobalShortcutManager::connect(const QKeySequence& key, QObject* receiver, const char* slot)
{
	KeyTrigger* t = instance()->triggers_[key];
	if (!t) {
		t = new KeyTrigger(key);
		instance()->triggers_.insert(key, t);
	}

	QObject::connect(t, SIGNAL(triggered()), receiver, slot);
}

/**
 * \brief Disonnects a key sequence from a slot.
 * \param key, global shortcut to be disconnected
 * \param receiver, object which \a slot is about to be disconnected
 * \param slot, the SLOT() of the \a receiver which should no longer be triggerd if the \a key is activated
 */
void GlobalShortcutManager::disconnect(const QKeySequence& key, QObject* receiver, const char* slot)
{
	KeyTrigger* t = instance()->triggers_[key];
	if (!t) {
		return;
	}

	QObject::disconnect(t, SIGNAL(triggered()), receiver, slot);

	if (!t->isUsed()) {
		delete instance()->triggers_.take(key);
	}
}

void GlobalShortcutManager::clear()
{
	foreach (KeyTrigger* t, instance()->triggers_)
		delete t;
	instance()->triggers_.clear();
}
