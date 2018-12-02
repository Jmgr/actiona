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

	Contact : jmgr@jmgr.info
*/

#include "systeminputtask.h"

#include <QDebug>
#include <QThread>
#include <QSharedPointer>
#include <QPoint>

#ifdef Q_OS_UNIX
#include <QTimer>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include <X11/extensions/record.h>
#include <QX11Info>
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

namespace ActionTools
{
	namespace SystemInput
	{
#ifdef Q_OS_UNIX
		static XRecordContext gXRecordContext;

		static void xRecordCallback(XPointer, XRecordInterceptData *data)
		{
			QSharedPointer<XRecordInterceptData> safeData(data, XRecordFreeData);

			switch(data->category)
			{
			case XRecordFromServer:
			case XRecordFromClient:
				{
					auto recordData = reinterpret_cast<xEvent *>(safeData.data()->data);

					switch(recordData->u.u.type)
					{
					case KeyPress:
						//TODO
						break;
					case KeyRelease:
						//TODO
						break;
					case ButtonPress:
						switch(recordData->u.u.detail)
						{
						case Button1:
							Task::instance()->emitMouseButtonPressed(LeftButton);
							break;
						case Button2:
							Task::instance()->emitMouseButtonPressed(MiddleButton);
							break;
						case Button3:
							Task::instance()->emitMouseButtonPressed(RightButton);
							break;
						case Button4:
						case Button5:
							// Ignore wheel up & wheel down buttons
							break;
						default:
							Task::instance()->emitMouseButtonPressed(static_cast<Button>(XButton0 + recordData->u.u.detail - Button5 - 1));
							break;
						}
						break;
					case ButtonRelease:
						switch(recordData->u.u.detail)
						{
						case Button1:
							Task::instance()->emitMouseButtonReleased(LeftButton);
							break;
						case Button2:
							Task::instance()->emitMouseButtonReleased(MiddleButton);
							break;
						case Button3:
							Task::instance()->emitMouseButtonReleased(RightButton);
							break;
						case Button4:
							Task::instance()->emitMouseWheel(1);
							break;
						case Button5:
							Task::instance()->emitMouseWheel(-1);
							break;
						default:
							Task::instance()->emitMouseButtonReleased(static_cast<Button>(XButton0 + recordData->u.u.detail - Button5 - 1));
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

#ifdef Q_OS_WIN
		static HHOOK gMouseHook;
		static HHOOK gKeyboardHook;

		static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
		{
			if(nCode < 0)
				return CallNextHookEx(gMouseHook, nCode, wParam, lParam);

			MSLLHOOKSTRUCT *data = reinterpret_cast<MSLLHOOKSTRUCT *>(lParam);

			switch(wParam)
			{
			case WM_MOUSEMOVE:
				Task::instance()->emitMouseMotion(data->pt.x, data->pt.y);
				break;
			case WM_MOUSEWHEEL:
				if(static_cast<qint16>(HIWORD(data->mouseData)) > 0)
					Task::instance()->emitMouseWheel(1);
				else
					Task::instance()->emitMouseWheel(-1);
				break;
			case WM_LBUTTONDOWN:
				Task::instance()->emitMouseButtonPressed(LeftButton);
				break;
			case WM_LBUTTONUP:
				Task::instance()->emitMouseButtonReleased(LeftButton);
				break;
			case WM_RBUTTONDOWN:
				Task::instance()->emitMouseButtonPressed(RightButton);
				break;
			case WM_RBUTTONUP:
				Task::instance()->emitMouseButtonReleased(RightButton);
				break;
			case WM_MBUTTONDOWN:
				Task::instance()->emitMouseButtonPressed(MiddleButton);
				break;
			case WM_MBUTTONUP:
				Task::instance()->emitMouseButtonReleased(MiddleButton);
				break;
			case WM_XBUTTONDOWN:
				switch(HIWORD(data->mouseData))
				{
				case XBUTTON1:
					Task::instance()->emitMouseButtonPressed(XButton0);
					break;
				case XBUTTON2:
					Task::instance()->emitMouseButtonPressed(XButton1);
					break;
				default:
					break;
				}
				break;
			case WM_XBUTTONUP:
				switch(HIWORD(data->mouseData))
				{
				case XBUTTON1:
					Task::instance()->emitMouseButtonReleased(XButton0);
					break;
				case XBUTTON2:
					Task::instance()->emitMouseButtonReleased(XButton1);
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}

			return CallNextHookEx(gMouseHook, nCode, wParam, lParam);
		}

		static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
		{
			//TODO: send input using Task::instance()->keyboardEvent

			return CallNextHookEx(gKeyboardHook, nCode, wParam, lParam);
		}
#endif
		Task *Task::mInstance = nullptr;

		Task::Task(QObject *parent)
			: QObject(parent),
			  mThread(new QThread(this))
#ifdef Q_OS_UNIX
            ,mProcessRepliesTimer(new QTimer(this))
#endif
		{
			Q_ASSERT(mInstance == nullptr);

			mInstance = this;

#ifdef Q_OS_WIN
			moveToThread(mThread);

			mThread->start();
#endif

#ifdef Q_OS_UNIX
            connect(mProcessRepliesTimer, &QTimer::timeout, this, &Task::processReplies);

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

#ifdef Q_OS_UNIX
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

			XRecordEnableContextAsync(QX11Info::display(), gXRecordContext, &xRecordCallback, nullptr);

			mProcessRepliesTimer->setSingleShot(false);
			mProcessRepliesTimer->start(0);
#endif

#ifdef Q_OS_WIN
			gMouseHook = SetWindowsHookEx(WH_MOUSE_LL, &LowLevelMouseProc, 0, 0);
			gKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, &LowLevelKeyboardProc, 0, 0);
#endif
		}

		void Task::stop()
		{
			if(!mStarted)
				return;

			mStarted = false;

#ifdef Q_OS_UNIX
			mProcessRepliesTimer->stop();

			XRecordDisableContext(QX11Info::display(), gXRecordContext);
			XRecordFreeContext(QX11Info::display(), gXRecordContext);
#endif

#ifdef Q_OS_WIN
			UnhookWindowsHookEx(gMouseHook);
			UnhookWindowsHookEx(gKeyboardHook);
#endif
		}

#ifdef Q_OS_UNIX
		void Task::processReplies()
		{
			//XRecordProcessReplies(QX11Info::display());
		}
#endif
	}
}
