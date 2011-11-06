/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#include <QtGlobal>

#include "notifyinstance.h"

#ifdef Q_WS_X11
#undef signals
#include <libnotify/notify.h>
#define signals
#endif

#ifndef NOTIFY_CHECK_VERSION
#define NOTIFY_CHECK_VERSION(x,y,z) 0
#endif

namespace Actions
{
	NotifyInstance::NotifyInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		mNotification(0)
	{
	}

	NotifyInstance::~NotifyInstance()
	{
	#ifdef Q_WS_X11
		if(mNotification)
		{
			notify_notification_close(mNotification, 0);
			g_object_unref(mNotification);
		}
	#endif
	}

	void NotifyInstance::startExecution()
	{
	#ifdef Q_WS_X11
		bool ok = true;

		QString title = evaluateString(ok, "title");
		QString text = evaluateString(ok, "text");
		int timeout = evaluateInteger(ok, "timeout");
		QString icon = evaluateString(ok, "icon");

		if(!ok)
			return;

		if(!mNotification)
			mNotification = notify_notification_new(title.toUtf8(), text.toUtf8(), icon.toUtf8()
	#if NOTIFY_CHECK_VERSION (0, 7, 0)
	);
	#else
	, 0);
	#endif
		else
			notify_notification_update(mNotification, title.toUtf8(), text.toUtf8(), icon.toUtf8());

		notify_notification_set_timeout(mNotification, timeout);

		if(!notify_notification_show(mNotification, 0))
		{
			emit executionException(UnableToShowNotificationException, tr("Unable to show the notification"));
			return;
		}
	#endif

		emit executionEnded();
	}
}
