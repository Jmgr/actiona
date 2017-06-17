/*
	Actiona
	Copyright (C) 2005-2017 Jonathan Mercier-Ganady

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

#ifndef CHOOSEWINDOWPUSHBUTTON_H
#define CHOOSEWINDOWPUSHBUTTON_H

#include "actiontools_global.h"
#include "windowhandle.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QAbstractNativeEventFilter>
#else
#include "nativeeventfilter.h"
#endif

#include <QPushButton>

class QMainWindow;

namespace ActionTools
{
    class ACTIONTOOLSSHARED_EXPORT ChooseWindowPushButton : public QPushButton
#if (QT_VERSION >= 0x050000)//BUG: Cannot use QT_VERSION_CHECK here, or the MOC will consider the condition to be true
            , public QAbstractNativeEventFilter
#else
            , public NativeEventFilter
#endif
	{
		Q_OBJECT

	public:
		explicit ChooseWindowPushButton(QWidget *parent = 0);
		~ChooseWindowPushButton();

	signals:
        void foundValidWindow(const ActionTools::WindowHandle &handle);
        void searchEnded(const ActionTools::WindowHandle &handle);

	private:
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);
#ifdef Q_OS_WIN
		void mouseReleaseEvent(QMouseEvent *event);

		void foundWindow(const WindowHandle &handle);
#endif
		bool isWindowValid(const WindowHandle &handle) const;

#ifdef Q_OS_WIN
		void refreshWindow(const WindowHandle &handle);
		void highlightWindow(const WindowHandle &handle);
#endif

		void startMouseCapture();
		void stopMouseCapture();

#ifdef Q_OS_LINUX
		WId windowAtPointer() const;
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        bool x11EventFilter(XEvent *event);
#else
        bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif
#endif

#ifdef Q_OS_WIN
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
		bool winEventFilter(MSG *msg, long *result);
#else
        bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif
#endif

		QPixmap *mCrossIcon;
		WindowHandle mLastFoundWindow;
		bool mSearching;
		QMainWindow *mMainWindow;
#ifdef Q_OS_LINUX
        QList<QWidget*> mShownWindows;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        unsigned long mCrossCursor;
#endif
#endif
#ifdef Q_OS_WIN
		HCURSOR mPreviousCursor;
		HPEN	mRectanglePen;
#endif

		Q_DISABLE_COPY(ChooseWindowPushButton)
	};
}

#endif // CHOOSEWINDOWPUSHBUTTON_H
