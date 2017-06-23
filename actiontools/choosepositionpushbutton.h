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

#pragma once

#include "actiontools_global.h"

#ifdef Q_OS_LINUX
#include <QAbstractNativeEventFilter>
#endif

#include <QPushButton>
#include <QList>

class QMainWindow;

namespace ActionTools
{
    class ACTIONTOOLSSHARED_EXPORT ChoosePositionPushButton : public QPushButton
#ifdef Q_OS_LINUX
            , public QAbstractNativeEventFilter
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
        bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif
		void stopMouseCapture();

		QPixmap *mCrossIcon;
		bool mSearching;
		QPoint mResult;
		QMainWindow *mMainWindow;
#ifdef Q_OS_LINUX
        QList<QWidget*> mShownWindows;
        unsigned long mCrossCursor;
#endif
#ifdef Q_OS_WIN
		HCURSOR mPreviousCursor;
		HPEN	mRectanglePen;
#endif

		Q_DISABLE_COPY(ChoosePositionPushButton)
	};
}

