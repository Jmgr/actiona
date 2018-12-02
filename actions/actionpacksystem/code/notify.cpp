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

#include "notify.h"

#include <QScriptValueIterator>

#ifdef Q_OS_UNIX
#undef signals
#include <libnotify/notify.h>
#define signals
#endif

#ifndef NOTIFY_CHECK_VERSION
#define NOTIFY_CHECK_VERSION(x,y,z) 0
#endif

namespace Code
{
	QScriptValue Notify::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		auto notify = new Notify;

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == QLatin1String("title"))
				notify->mTitle = it.value().toString();
			else if(it.name() == QLatin1String("text"))
				notify->mText = it.value().toString();
			else if(it.name() == QLatin1String("icon"))
				notify->mIcon = it.value().toString();
			else if(it.name() == QLatin1String("timeout"))
				notify->mTimeout = it.value().toInt32();
		}

		return CodeClass::constructor(notify, context, engine);
	}
	
	Notify::Notify()
		: CodeClass()
		
	{
	}
	
	Notify::~Notify()
	{
#ifdef Q_OS_UNIX
		if(mNotification)
		{
			notify_notification_close(mNotification, nullptr);
			g_object_unref(mNotification);
		}
#endif
	}
	
	QScriptValue Notify::show()
	{
#ifdef Q_OS_UNIX
		QScriptValueIterator it(context()->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == QLatin1String("title"))
				mTitle = it.value().toString();
			else if(it.name() == QLatin1String("text"))
				mText = it.value().toString();
			else if(it.name() == QLatin1String("icon"))
				mIcon = it.value().toString();
			else if(it.name() == QLatin1String("timeout"))
				mTimeout = it.value().toInt32();
		}
		
		if(!mNotification)
			mNotification = notify_notification_new(mTitle.toUtf8().constData(), mText.toUtf8().constData(), mIcon.toUtf8().constData()
		#if NOTIFY_CHECK_VERSION (0, 7, 0)
		);
		#else
		, 0);
		#endif
		else
			notify_notification_update(mNotification, mTitle.toUtf8().constData(), mText.toUtf8().constData(), mIcon.toUtf8().constData());

		notify_notification_set_timeout(mNotification, mTimeout);

		if(!notify_notification_show(mNotification, nullptr))
			throwError(QStringLiteral("NotificationError"), tr("Unable to show the notification"));
#endif
		return thisObject();
	}
}
