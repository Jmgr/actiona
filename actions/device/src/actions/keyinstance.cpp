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

#include "keyinstance.hpp"
#include "actiontools/keyinput.hpp"
#include "backend/keyboard-output.hpp"

#ifdef Q_OS_WIN
#include "backend/keyboard-output-windows.hpp"
#endif

#include <QTimer>

namespace Actions
{
    Tools::StringListPair KeyInstance::actions =
    {
        {
            QStringLiteral("pressRelease"),
            QStringLiteral("press"),
            QStringLiteral("release")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("KeyInstance::actions", "Press and release")),
            QStringLiteral(QT_TRANSLATE_NOOP("KeyInstance::actions", "Press")),
            QStringLiteral(QT_TRANSLATE_NOOP("KeyInstance::actions", "Release"))
        }
    };

    Tools::StringListPair KeyInstance::types =
    {
        {
            QStringLiteral("win32"),
            QStringLiteral("directx")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("KeyInstance::actions", "Win32")),
            QStringLiteral(QT_TRANSLATE_NOOP("KeyInstance::actions", "DirectX"))
        }
    };

	KeyInstance::KeyInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
          mAutoreleaser(Backend::Backend::instance().keyboardOutput()),
		  mCtrl(false),
		  mAlt(false),
		  mShift(false),
		  mMeta(false),
		  mTimer(new QTimer(this))
	{
        mTimer->setTimerType(Qt::PreciseTimer);

        connect(mTimer, &QTimer::timeout, this, &KeyInstance::sendRelease);
	}

	void KeyInstance::startExecution()
	{
        auto &keyboardOutput = Backend::Backend::instance().keyboardOutput();

		bool ok = true;

		mKey = evaluateString(ok, QStringLiteral("key"), QStringLiteral("key"));
		auto action = evaluateListElement<Action>(ok, actions, QStringLiteral("action"));
		mAmount = evaluateInteger(ok, QStringLiteral("amount"));
		mCtrl = evaluateBoolean(ok, QStringLiteral("ctrl"));
		mAlt = evaluateBoolean(ok, QStringLiteral("alt"));
		mShift = evaluateBoolean(ok, QStringLiteral("shift"));
		mMeta = evaluateBoolean(ok, QStringLiteral("meta"));
		Type type = evaluateListElement<Type>(ok, types, QStringLiteral("type"));
		mPause  = evaluateInteger(ok, QStringLiteral("pause"));

		if(mPause < 0)
			mPause = 0;
		
		if (!ok)
			return;
		
		if(action != PressReleaseAction)
			mAmount = 1;

		if(mAmount <= 0)
		{
			setCurrentParameter(QStringLiteral("amount"));
			emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid key presses amount"));
			return;
		}

#ifdef Q_OS_WIN
        auto &windowsKeyboardOutput = qobject_cast<Backend::KeyboardOutputWindows&>();
        windowsKeyboardOutput.setType(static_cast<Backend::KeyboardOutputWindows::Type>(type));
#endif

		bool result = true;

		switch(action)
		{
		case PressAction:
			pressOrReleaseModifiers(true);

            result &= keyboardOutput.pressKey(mKey);
			break;
		case ReleaseAction:
			pressOrReleaseModifiers(false);

            result &= keyboardOutput.releaseKey(mKey);
			break;
		case PressReleaseAction:
			pressOrReleaseModifiers(true);

            result &= keyboardOutput.pressKey(mKey);

			mTimer->setSingleShot(true);
			mTimer->start(mPause);
			break;
		}

		if(!result)
		{
			emit executionException(FailedToSendInputException, tr("Unable to emulate key: failed to send input"));
			return;
		}

		if(action != PressReleaseAction)
			executionEnded();
	}

	void KeyInstance::stopExecution()
	{
		mTimer->stop();
	}

	void KeyInstance::sendRelease()
	{
        auto &keyboardOutput = Backend::Backend::instance().keyboardOutput();

		pressOrReleaseModifiers(false);
        keyboardOutput.releaseKey(mKey);

		--mAmount;
		if (mAmount > 0)
			emit sendPressKey();
		else
			executionEnded();
	}

	void KeyInstance::sendPressKey()
	{
        auto &keyboardOutput = Backend::Backend::instance().keyboardOutput();
		bool result = true;

		pressOrReleaseModifiers(true);

        result &= keyboardOutput.pressKey(mKey);

		if(!result)
		{
			emit executionException(FailedToSendInputException, tr("Unable to emulate key: failed to send input"));
			return;
		}

		mTimer->start(mPause);
	}

	void KeyInstance::pressOrReleaseModifiers(bool press)
	{
        auto &keyboardOutput = Backend::Backend::instance().keyboardOutput();

		if(press)
		{
			if(mCtrl)
                keyboardOutput.pressKey(QStringLiteral("controlLeft"));
			if(mAlt)
                keyboardOutput.pressKey(QStringLiteral("altLeft"));
			if(mShift)
                keyboardOutput.pressKey(QStringLiteral("shiftLeft"));
			if(mMeta)
                keyboardOutput.pressKey(QStringLiteral("metaLeft"));
		}
		else
		{
			if(mCtrl)
                keyboardOutput.releaseKey(QStringLiteral("controlLeft"));
			if(mAlt)
                keyboardOutput.releaseKey(QStringLiteral("altLeft"));
			if(mShift)
                keyboardOutput.releaseKey(QStringLiteral("shiftLeft"));
			if(mMeta)
                keyboardOutput.releaseKey(QStringLiteral("metaLeft"));
		}
	}
}
