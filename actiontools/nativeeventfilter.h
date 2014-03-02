/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef NATIVEEVENTFILTER_H
#define NATIVEEVENTFILTER_H

#include "actiontools_global.h"
#include "nativeeventfilteringapplication.h"

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT NativeEventFilter
	{
	public:
		virtual ~NativeEventFilter()
		{
			NativeEventFilteringApplication *app = qobject_cast<NativeEventFilteringApplication *>(qApp);

			if(app)
				app->removeNativeEventFilter(this);
		}
		
#ifdef Q_WS_X11
		virtual bool x11EventFilter(XEvent*)
		{
			return false;
		}
#endif
		
#ifdef Q_WS_WIN
		virtual bool winEventFilter(MSG*, long*)
		{
			return false;
		}
#endif
	};
}

#endif // NATIVEEVENTFILTER_H
