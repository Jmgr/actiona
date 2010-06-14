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

#include <QtGlobal>

#ifdef Q_WS_X11
#include <libnotify/notify.h>
#endif

#include "actionnotifyinstance.h"

ActionNotifyInstance::ActionNotifyInstance(ActionTools::ActionInterface *interface, QObject *parent)
	: ActionTools::Action(interface, parent),
	mNotification(0)
{
}

ActionNotifyInstance::ActionNotifyInstance(QObject *parent)
	: ActionTools::Action(0, parent),
	mNotification(0)
{
}

ActionNotifyInstance::~ActionNotifyInstance()
{
#ifdef Q_WS_X11
	if(mNotification)
	{
		notify_notification_close(mNotification, 0);
		g_object_unref(mNotification);
	}
#endif
}

void ActionNotifyInstance::startExecution()
{
	ActionTools::ActionExecution actionExecution(this, script(), scriptEngine());
	QString title;
	QString text;
	int timeout;
	QString icon;

	if(!actionExecution.evaluateString(title, "title") ||
	   !actionExecution.evaluateString(text, "text") ||
	   !actionExecution.evaluateInteger(timeout, "timeout") ||
	   !actionExecution.evaluateString(icon, "icon"))
		return;

#ifdef Q_WS_X11
	if(!mNotification)
		mNotification = notify_notification_new(title.toUtf8(), text.toUtf8(), icon.toUtf8(), 0);
	else
		notify_notification_update(mNotification, title.toUtf8(), text.toUtf8(), icon.toUtf8());
	
	notify_notification_set_timeout(mNotification, timeout);
	
	if(!notify_notification_show(mNotification, 0))
		emit executionException(ActionTools::Action::Error, tr("Unable to show the notification"));
#endif

	emit executionEnded();
}
