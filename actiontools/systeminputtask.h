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

#ifndef SYSTEMINPUTTASK_H
#define SYSTEMINPUTTASK_H

#include <QObject>

#include "nativeeventfilter.h"
#include "systeminput.h"

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef Q_WS_WIN
class QThread;
#endif

namespace ActionTools
{
	namespace SystemInput
	{
		class Task : public QObject, public NativeEventFilter
		{
			Q_OBJECT

		public:
			explicit Task(QObject *parent = 0);
			~Task();

		signals:
			void mouseMotion(int x, int y);
			void mouseWheel(int intensity);
			void mouseButtonPressed(Button button);
			void mouseButtonReleased(Button button);
			void keyPressed(int key);
			void keyReleased(int key);

		public slots:
			void start();
			void stop();

		private:
#ifdef Q_WS_WIN
			LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
			LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
#endif

#ifdef Q_WS_X11
			bool x11EventFilter(XEvent *event);
#endif

#ifdef Q_WS_WIN
			void emitMouseMotion(int x, int y) { emit mouseMotion(x, y); }
			void emitMouseWheel(int intensity) { emit mouseWheel(intensity); }
			void emitMouseButtonPressed(Button button) { emit mouseButtonPressed(button); }
			void emitMouseButtonReleased(Button button) { emit mouseButtonReleased(button); }
#endif

			static Task *mInstance;

#ifdef Q_WS_WIN
			QThread *mThread;
#endif
			bool mStarted;

#ifdef Q_WS_WIN
			HHOOK mMouseHook;
			HHOOK mKeyboardHook;
#endif
		};
	}
}

#endif // SYSTEMINPUTTASK_H
