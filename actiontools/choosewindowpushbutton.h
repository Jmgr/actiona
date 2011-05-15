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

#ifndef CHOOSEWINDOWPUSHBUTTON_H
#define CHOOSEWINDOWPUSHBUTTON_H

#include "actiontools_global.h"
#include "windowhandle.h"
#include "nativeeventfilter.h"

#include <QPushButton>

class QMainWindow;

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT ChooseWindowPushButton : public QPushButton, public NativeEventFilter
	{
		Q_OBJECT

	public:
		explicit ChooseWindowPushButton(QWidget *parent = 0);
		~ChooseWindowPushButton();

	signals:
		void foundValidWindow(const WindowHandle &handle);
		void searchEnded(const WindowHandle &handle);

	private:
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);
#ifdef Q_WS_WIN
		void mouseReleaseEvent(QMouseEvent *event);

		void foundWindow(const WindowHandle &handle);
#endif
		bool isWindowValid(const WindowHandle &handle) const;

#ifdef Q_WS_WIN
		void refreshWindow(const WindowHandle &handle);
		void highlightWindow(const WindowHandle &handle);
#endif

		void startMouseCapture();
		void stopMouseCapture();

#ifdef Q_WS_X11
		WId windowAtPointer() const;
		bool x11EventFilter(XEvent *event);
#endif
#ifdef Q_WS_WIN
		bool winEventFilter(MSG *msg, long *result);
#endif

		QList<QWidget *> mWindowIgnoreList;
		QPixmap *mCrossIcon;
		WindowHandle mLastFoundWindow;
		bool mSearching;
		QMainWindow *mMainWindow;
#ifdef Q_WS_WIN
		HCURSOR mPreviousCursor;
		HPEN	mRectanglePen;
#endif

		Q_DISABLE_COPY(ChooseWindowPushButton)
	};
}

#endif // CHOOSEWINDOWPUSHBUTTON_H
