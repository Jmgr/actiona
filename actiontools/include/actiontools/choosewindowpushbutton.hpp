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

#pragma once

#include "actiontools_global.hpp"
#include "windowhandle.hpp"

#include <QAbstractNativeEventFilter>
#include <QPushButton>

class QMainWindow;

namespace ActionTools
{
    class ACTIONTOOLSSHARED_EXPORT ChooseWindowPushButton : public QPushButton, public QAbstractNativeEventFilter
	{
		Q_OBJECT

	public:
        explicit ChooseWindowPushButton(QWidget *parent = nullptr);
        ~ChooseWindowPushButton() override;

        void setOpacityChangeOnWindows(bool value) { mOpacityChangeOnWindows = value; }

	signals:
        void searchStarted();
        void foundValidWindow(const ActionTools::WindowHandle &handle);
        void searchEnded(const ActionTools::WindowHandle &handle);

	private:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
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

#ifdef Q_OS_UNIX
		WId windowAtPointer() const;
#endif

        bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

		QPixmap *mCrossIcon;
        bool mOpacityChangeOnWindows{true};
		WindowHandle mLastFoundWindow;
		bool mSearching{false};
		QMainWindow *mMainWindow{nullptr};
#ifdef Q_OS_UNIX
        QList<QWidget*> mShownWindows;
        unsigned long mCrossCursor;
#endif
#ifdef Q_OS_WIN
		HCURSOR mPreviousCursor;
		HPEN	mRectanglePen;
#endif

		Q_DISABLE_COPY(ChooseWindowPushButton)
	};
}

