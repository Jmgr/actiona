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

	Contact: jmgr@jmgr.info
*/

#include "notify.hpp"

#include <QJSValueIterator>

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
	Notify::Notify()
		: CodeClass()
		
	{
	}

    Notify::Notify(const QJSValue &parameters)
        : Notify()
    {
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return;
        }

        QJSValueIterator it(parameters);

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
                mTimeout = it.value().toInt();
        }
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
	
    Notify *Notify::show(const QJSValue &parameters)
	{
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return this;
        }

#ifdef Q_OS_UNIX
        QJSValueIterator it(parameters);

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
                mTimeout = it.value().toInt();
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
        return this;
	}

    void Notify::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Notify*>("const Notify *");

        CodeClass::registerClass<Notify>(QStringLiteral("Notify"), scriptEngine);
    }
}
