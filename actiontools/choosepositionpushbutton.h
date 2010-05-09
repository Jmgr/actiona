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

#ifndef CHOOSEPOSITIONPUSHBUTTON_H
#define CHOOSEPOSITIONPUSHBUTTON_H

#include "actiontools_global.h"

#include <QPushButton>
#include <QxtNativeEventFilter>

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT ChoosePositionPushButton : public QPushButton, public QxtNativeEventFilter
	{
		Q_OBJECT
	public:
		explicit ChoosePositionPushButton(QWidget *parent = 0);
		~ChoosePositionPushButton();

	signals:
		void positionChosen(QPoint position);

	private:
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);
#ifdef Q_WS_WIN
		void mouseReleaseEvent(QMouseEvent *event);
#endif
#ifdef Q_WS_X11
		bool x11EventFilter(XEvent *event);
#endif
		void stopMouseCapture();

		QPixmap *mCrossIcon;
		bool mSearching;
		QPoint mResult;
#ifdef Q_WS_WIN
		HCURSOR mPreviousCursor;
		HPEN	mRectanglePen;
#endif

		Q_DISABLE_COPY(ChoosePositionPushButton)
	};
}

#endif // CHOOSEPOSITIONPUSHBUTTON_H
