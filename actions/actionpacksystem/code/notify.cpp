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

#include "notify.h"

#include <QScriptValueIterator>

#ifdef Q_WS_X11
#undef signals
#include <libnotify/notify.h>
#define signals
#endif

namespace Code
{
	QScriptValue Notify::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Notify *notify = new Notify;

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == "title")
				notify->mTitle = it.value().toString();
			else if(it.name() == "text")
				notify->mText = it.value().toString();
			else if(it.name() == "icon")
				notify->mIcon = it.value().toString();
			else if(it.name() == "timeout")
				notify->mTimeout = it.value().toInt32();
		}

		return engine->newQObject(notify, QScriptEngine::ScriptOwnership);
	}
	
	Notify::Notify()
		: QObject(),
		QScriptable(),
		mNotification(0),
		mTimeout(5000)
	{
	}
	
	Notify::~Notify()
	{
#ifdef Q_WS_X11
		if(mNotification)
		{
			notify_notification_close(mNotification, 0);
			g_object_unref(mNotification);
		}
#endif
	}
	
	QScriptValue Notify::show()
	{
#ifdef Q_WS_X11
		QScriptValueIterator it(context()->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == "title")
				mTitle = it.value().toString();
			else if(it.name() == "text")
				mText = it.value().toString();
			else if(it.name() == "icon")
				mIcon = it.value().toString();
			else if(it.name() == "timeout")
				mTimeout = it.value().toInt32();
		}
		
		if(!mNotification)
			mNotification = notify_notification_new(mTitle.toUtf8(), mText.toUtf8(), mIcon.toUtf8(), 0);
		else
			notify_notification_update(mNotification, mTitle.toUtf8(), mText.toUtf8(), mIcon.toUtf8());

		notify_notification_set_timeout(mNotification, mTimeout);

		if(!notify_notification_show(mNotification, 0))
			context()->throwError(tr("Unable to show the notification"));
#endif
		return context()->thisObject();
	}
}
