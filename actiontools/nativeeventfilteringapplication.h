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

#ifndef NATIVEEVENTFILTERINGAPPLICATION_H
#define NATIVEEVENTFILTERINGAPPLICATION_H

#include "actiontools_global.h"
#include "qtsingleapplication/qtsingleapplication.h"

#define nativeEventFilteringApp (ActionTools::NativeEventFilteringApplication::instance())

namespace ActionTools
{
	class NativeEventFilter;
	
	class ACTIONTOOLSSHARED_EXPORT NativeEventFilteringApplication : public QtSingleApplication
	{
		Q_OBJECT
		
	public:
		NativeEventFilteringApplication(const QString &appId, int &argc, char **argv);
		
		void installNativeEventFilter(NativeEventFilter *filter);
		void removeNativeEventFilter(NativeEventFilter *filter);
		
		inline static NativeEventFilteringApplication *instance()
		{
			return qobject_cast<NativeEventFilteringApplication*>(QApplication::instance());
		}
		
	private:
#ifdef Q_WS_X11
		bool x11EventFilter(XEvent *event);
#endif
#ifdef Q_WS_WIN
		bool winEventFilter(MSG *msg, long *result);
#endif
		
		QList<NativeEventFilter*> mNativeFilters;
	};
}

#endif // NATIVEEVENTFILTERINGAPPLICATION_H
