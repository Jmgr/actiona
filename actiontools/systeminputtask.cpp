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

#include "systeminputtask.h"

#include <QDebug>

#ifdef Q_WS_WIN
#include <QThread>
#endif

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#include <QX11Info>
#endif

namespace ActionTools
{
	namespace SystemInput
	{
		Task *Task::mInstance = 0;

		Task::Task(QObject *parent)
			: QObject(parent),
	#ifdef Q_WS_WIN
			  mThread(new QThread(this)),
	#endif
			  mStarted(false)
		{
			Q_ASSERT(mInstance == 0);

			mInstance = this;

	#ifdef Q_WS_WIN
			moveToThread(mThread);

			mThread->start();
	#endif
		}

		Task::~Task()
		{
	#ifdef Q_WS_WIN
			mThread->quit();
			mThread->wait();
	#endif
		}

		void Task::start()
		{
			if(mStarted)
				return;

			mStarted = true;

	#ifdef Q_WS_X11
			nativeEventFilteringApp->installNativeEventFilter(this);

			if(XGrabPointer(QX11Info::display(), DefaultRootWindow(QX11Info::display()), True, ButtonReleaseMask | ButtonPressMask | PointerMotionMask, GrabModeAsync, GrabModeAsync,
							None, None, CurrentTime) != GrabSuccess)
			{
				qWarning() << "SystemInputReceiver: Unable to grab the pointer";

				return;
			}
			if(XGrabKeyboard(QX11Info::display(), DefaultRootWindow(QX11Info::display()), True, GrabModeAsync, GrabModeAsync, CurrentTime) != GrabSuccess)
			{
				qWarning() << "SystemInputReceiver: Unable to grab the keyboard";

				return;
			}
	#endif

	#ifdef Q_WS_WIN
			mMouseHook = SetWindowsHookEx(WH_MOUSE_LL, &SystemInputTask::LowLevelMouseProc, 0, 0);
			mKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, &SystemInputTask::LowLevelKeyboardProc, 0, 0);
	#endif
		}

		void Task::stop()
		{
			if(!mStarted)
				return;

			mStarted = false;

	#ifdef Q_WS_X11
			nativeEventFilteringApp->removeNativeEventFilter(this);

			XUngrabPointer(QX11Info::display(), CurrentTime);
			XUngrabKeyboard(QX11Info::display(), CurrentTime);
	#endif

	#ifdef Q_WS_WIN
			UnhookWindowsHookEx(mMouseHook);
			UnhookWindowsHookEx(mKeyboardHook);
	#endif
		}

	#ifdef Q_WS_WIN
		LRESULT CALLBACK Task::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
		{
			if(nCode < 0)
				return CallNextHookEx(mMouseHook, nCode, wParam, lParam);

			switch(wParam)
			{
			case WM_MOUSEMOVE:
				{
					const QPoint &position = QCursor::pos();

					mInstance->emitMouseMotion(position.x(), position.y());
				}
				break;
			case WM_MOUSEWHEEL:
				{
					//mInstance->emitMouseWheel();
				}
				break;
			default:
				break;
			}

			//TODO: send input using mInstance->mouseEvent

			return CallNextHookEx(mMouseHook, nCode, wParam, lParam);
		}

		LRESULT CALLBACK Task::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
		{
			//TODO: send input using mInstance->keyboardEvent

			return CallNextHookEx(mKeyboardHook, nCode, wParam, lParam);
		}
	#endif

	#ifdef Q_WS_X11
		bool Task::x11EventFilter(XEvent *event)
		{
			switch(event->type)
			{
			case MotionNotify:
				{
					const QPoint &position = QCursor::pos();

					emit mouseMotion(position.x(), position.y());
				}
				break;
			case ButtonPress:
				{
					XButtonEvent *buttonEvent = reinterpret_cast<XButtonEvent *>(event);

					emit mouseButtonPressed(static_cast<Button>(buttonEvent->button - 1));
				}
				break;
			case ButtonRelease:
				{
					XButtonEvent *buttonEvent = reinterpret_cast<XButtonEvent *>(event);

					emit mouseButtonReleased(static_cast<Button>(buttonEvent->button - 1));

					switch(buttonEvent->button)
					{
					case Button4:
						emit mouseWheel(1);
						break;
					case Button5:
						emit mouseWheel(-1);
						break;
					}
				}
			default:
				break;
			}

			/*
			else if(event->type == KeyPress)
				qDebug() << "key press";
			else if(event->type == KeyRelease)
				qDebug() << "key release";
	*/
			return false;
		}
	#endif
	}
}
