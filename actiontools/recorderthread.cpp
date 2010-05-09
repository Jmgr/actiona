#include "recorderthread.h"
#include "keyinput.h"

#include <QSettings>
#include <QLibrary>

#ifdef Q_WS_WIN
#include <windows.h>
#include <limits.h>
#endif
/*
#pragma warning(disable: 4200)

namespace
{
	HHOOK gHook;
}

LRESULT CALLBACK HookProc(int code, WPARAM wparam, LPARAM lparam)
{
	if(code != HC_ACTION)
		return CallNextHookEx(gHook, code, wparam, lparam);

	KBDLLHOOKSTRUCT *hookstruct = ((KBDLLHOOKSTRUCT*)lparam);
	//KeyInput input(hookstruct->vkCode, hookstruct->scanCode);

	HKL dwhkl = 0;
	BYTE dbKbdState[256];
	wchar_t szCharBuf[32];
	static KBDLLHOOKSTRUCT lastState = {0};

	GetKeyboardState(dbKbdState);
	dwhkl = GetKeyboardLayout(0);

	if(ToUnicodeEx(hookstruct->vkCode, hookstruct->scanCode, dbKbdState, (LPWORD)szCharBuf, 1, 0, dwhkl) == -1)
	{
		//Save the current keyboard state.
		lastState = *hookstruct;
		while(ToUnicodeEx(hookstruct->vkCode, hookstruct->scanCode, dbKbdState, (LPWORD)szCharBuf, 1, 0, dwhkl) <0);
	}
	else
	{
		qDebug() << QString::fromWCharArray(szCharBuf, 1) << " scan:" << hookstruct->scanCode;

		if(lastState.vkCode != 0)
		{
			//Safest to just clear this.
			memset(dbKbdState, 0, 256);

			//Put the old vkCode back into the locale's buffer.
			ToUnicodeEx(lastState.vkCode, lastState.scanCode, dbKbdState, (LPWORD)szCharBuf, 1, 0, dwhkl);

			lastState.vkCode = 0;
		}
	}

	//qDebug() << "scan : " << hookstruct->scanCode
//			<< " vk : " << hookstruct->vkCode
//			<< " name : " << input.keyName();
			//<< " map : " << MapVirtualKey(hookstruct->scanCode, MAPVK_VSC_TO_VK_EX);

	return CallNextHookEx(gHook, code, wparam, lparam);
}

namespace ActionTools
{
	RecorderThread::RecorderThread(QObject *parent)
		: QThread(parent)
	{
	}

	RecorderThread::~RecorderThread()
	{
	}

	void RecorderThread::run()
	{
		gHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)HookProc, GetModuleHandle(NULL), 0);
		if(!gHook)
		{
			qDebug() << "Hook failed" << GetLastError();
			return;
		}

		exec();

		UnhookWindowsHookEx(gHook);
	}
}
*/
