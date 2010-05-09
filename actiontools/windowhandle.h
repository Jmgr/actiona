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

#ifndef WINDOWHANDLE_H
#define WINDOWHANDLE_H

#include <QWidget>
#include <QString>
#include <QRect>
#include <QMetaType>

#include "actiontools_global.h"

namespace ActionTools
{
	class WindowHandle
	{
	public:
		WindowHandle() : mValue(0)								{}

		bool isValid() const									{ return (mValue != 0); }
		void setInvalid()										{ mValue = 0; }

		operator bool() const									{ return isValid(); }
		bool operator ==(const WindowHandle &other) const		{ return (other.mValue == mValue); }
		bool operator !=(const WindowHandle &other) const		{ return (other.mValue != mValue); }

		QString title() const;
		QString classname() const;
		QRect rect() const;
		int processId() const;
		void kill() const;
		void killCreator() const;

		WindowHandle(WId handle)								{ mValue = handle; }
		WId value() const										{ return mValue; }

		static WindowHandle foregroundWindow();

	private:
		WId mValue;
	};
}

Q_DECLARE_METATYPE(ActionTools::WindowHandle)

#endif // WINDOWHANDLE_H
