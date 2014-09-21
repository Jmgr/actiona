/*
	Actiona
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef CHOOSEPOSITIONPUSHBUTTON_H
#define CHOOSEPOSITIONPUSHBUTTON_H

#include "actiontools_global.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QAbstractNativeEventFilter>
#else
#include "nativeeventfilter.h"
#endif

#include <QPushButton>
#include <QList>

class QMainWindow;

namespace ActionTools
{
    class ACTIONTOOLSSHARED_EXPORT ChoosePositionPushButton : public QPushButton
#if (QT_VERSION >= 0x050000)//BUG: Cannot use QT_VERSION_CHECK here, or the MOC will consider the condition to be true
            , public QAbstractNativeEventFilter
#else
            , public NativeEventFilter
#endif
	{
		Q_OBJECT
	public:
		explicit ChoosePositionPushButton(QWidget *parent = 0);
		~ChoosePositionPushButton();

	signals:
		void chooseStarted();
        void positionChosen(QPointF position);
		
	private:
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);
#ifdef Q_OS_WIN
		void mouseReleaseEvent(QMouseEvent *event);
#endif
#ifdef Q_OS_LINUX
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        bool x11EventFilter(XEvent *event);
#else
        bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif
#endif
		void stopMouseCapture();

		QPixmap *mCrossIcon;
		bool mSearching;
		QPoint mResult;
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

		Q_DISABLE_COPY(ChoosePositionPushButton)
	};
}

#endif // CHOOSEPOSITIONPUSHBUTTON_H
