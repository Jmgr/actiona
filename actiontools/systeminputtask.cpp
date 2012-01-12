/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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
#include <QThread>
#include <QSharedPointer>
#include <QPoint>

#ifdef Q_WS_X11
#include <QTimer>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include <X11/extensions/record.h>
#include <QX11Info>
#endif

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

namespace ActionTools
{
	namespace SystemInput
	{
#ifdef Q_WS_X11
		static XRecordContext gXRecordContext;

		static void xRecordCallback(XPointer, XRecordInterceptData *data)
		{
			QSharedPointer<XRecordInterceptData> safeData(data, XRecordFreeData);

			switch(data->category)
			{
			case XRecordFromServer:
			case XRecordFromClient:
				{
					xEvent *recordData = reinterpret_cast<xEvent *>(safeData.data()->data);

					switch(recordData->u.u.type)
					{
					case KeyPress:
						//TODO
						break;
					case KeyRelease:
						//TODO
						break;
					case ButtonPress:
						Task::instance()->emitMouseButtonPressed(static_cast<Button>(recordData->u.u.detail - 1));
						break;
					case ButtonRelease:
						Task::instance()->emitMouseButtonReleased(static_cast<Button>(recordData->u.u.detail - 1));

						switch(recordData->u.u.detail)
						{
						case Button4:
							Task::instance()->emitMouseWheel(1);
							break;
						case Button5:
							Task::instance()->emitMouseWheel(-1);
							break;
						}
						break;
					case MotionNotify:
						Task::instance()->emitMouseMotion(recordData->u.keyButtonPointer.rootX,
										 recordData->u.keyButtonPointer.rootY);
						break;
					default:
						break;
					}
				}
				break;
			default:
				return;
			}
		}
#endif

#ifdef Q_WS_WIN
		static HHOOK gMouseHook;
		static HHOOK gKeyboardHook;

		static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
		{
			if(nCode < 0)
				return CallNextHookEx(gMouseHook, nCode, wParam, lParam);

			qDebug() << "Thread?" << QThread::currentThreadId();

			switch(wParam)
			{
			case WM_MOUSEMOVE:
				{
//					const QPoint &position = QCursor::pos();

//					Task::instance()->emitMouseMotion(position.x(), position.y());
				}
				break;
			case WM_MOUSEWHEEL:
				{
					//Task::instance()->emitMouseWheel();
				}
				break;
			default:
				break;
			}

			//TODO: send input using Task::instance()->mouseEvent

			return CallNextHookEx(gMouseHook, nCode, wParam, lParam);
		}

		static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
		{
			//TODO: send input using Task::instance()->keyboardEvent

			return CallNextHookEx(gKeyboardHook, nCode, wParam, lParam);
		}
#endif
		Task *Task::mInstance = 0;

		Task::Task(QObject *parent)
			: QObject(parent),
			  mThread(new QThread(this))
			, mStarted(false)
	#ifdef Q_WS_X11
			, mProcessRepliesTimer(new QTimer(this))
	#endif
		{
			Q_ASSERT(mInstance == 0);

			mInstance = this;

#ifdef Q_WS_WIN
			moveToThread(mThread);
#endif

#ifdef Q_WS_X11
			connect(mProcessRepliesTimer, SIGNAL(timeout()), this, SLOT(processReplies()));
#endif

			qDebug() << "Main thread" << QThread::currentThreadId();

#ifdef Q_WS_WIN
			mThread->start();
#else
			start();
#endif
		}

		Task::~Task()
		{
			mThread->quit();
			mThread->wait();

			delete mThread;
		}

		void Task::start()
		{
			if(mStarted)
				return;

			mStarted = true;

			qDebug() << "Thread" << QThread::currentThreadId();

#ifdef Q_WS_X11
			XRecordClientSpec clients = XRecordAllClients;
			XRecordRange *range = XRecordAllocRange();

			if(!range)
			{
				qWarning() << "Failed to allocate XRecord range";

				return;
			}

			range->device_events.first = KeyPress;
			range->device_events.last = MotionNotify;

			XRecordContext gXRecordContext = XRecordCreateContext(QX11Info::display(), 0, &clients, 1, &range, 1);

			XFree(range);

			if(!gXRecordContext)
			{
				qWarning() << "Failed to create XRecord context";

				return;
			}

			XRecordEnableContextAsync(QX11Info::display(), gXRecordContext, &xRecordCallback, 0);

			mProcessRepliesTimer->setSingleShot(false);
			mProcessRepliesTimer->start(0);
#endif

#ifdef Q_WS_WIN
			gMouseHook = SetWindowsHookEx(WH_MOUSE_LL, &LowLevelMouseProc, 0, 0);
			gKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, &LowLevelKeyboardProc, 0, 0);
#endif
		}

		void Task::stop()
		{
			if(!mStarted)
				return;

			mStarted = false;

	#ifdef Q_WS_X11
			mProcessRepliesTimer->stop();

			XRecordDisableContext(QX11Info::display(), gXRecordContext);
			XRecordFreeContext(QX11Info::display(), gXRecordContext);
	#endif

	#ifdef Q_WS_WIN
			UnhookWindowsHookEx(gMouseHook);
			UnhookWindowsHookEx(gKeyboardHook);
	#endif
		}

#ifdef Q_WS_X11
		void Task::processReplies()
		{
			//XRecordProcessReplies(QX11Info::display());
		}
#endif
	}
}
