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

#ifdef Q_OS_WIN
#include <Windows.h>
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

		mKeyboardDevice.setType(static_cast<KeyboardDevice::Type>(type));

		bool result = true;

		switch(action)
		{
		case PressAction:
			pressOrReleaseModifiers(true);

			result &= mKeyboardDevice.pressKey(mKey);
			break;
		case ReleaseAction:
			pressOrReleaseModifiers(false);

			result &= mKeyboardDevice.releaseKey(mKey);
			break;
		case PressReleaseAction:
			pressOrReleaseModifiers(true);

			result &= mKeyboardDevice.pressKey(mKey);

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

	void KeyInstance::stopLongTermExecution()
	{
		mKeyboardDevice.reset();
	}

	void KeyInstance::sendRelease()
	{
		pressOrReleaseModifiers(false);
		mKeyboardDevice.releaseKey(mKey);

		--mAmount;
		if (mAmount > 0)
			emit sendPressKey();
		else
			executionEnded();
	}

	void KeyInstance::sendPressKey()
	{
		bool result = true;

		pressOrReleaseModifiers(true);

		result &= mKeyboardDevice.pressKey(mKey);

		if(!result)
		{
			emit executionException(FailedToSendInputException, tr("Unable to emulate key: failed to send input"));
			return;
		}

		mTimer->start(mPause);
	}

	void KeyInstance::pressOrReleaseModifiers(bool press)
	{
		if(press)
		{
			if(mCtrl)
				mKeyboardDevice.pressKey(QStringLiteral("controlLeft"));
			if(mAlt)
				mKeyboardDevice.pressKey(QStringLiteral("altLeft"));
			if(mShift)
				mKeyboardDevice.pressKey(QStringLiteral("shiftLeft"));
			if(mMeta)
				mKeyboardDevice.pressKey(QStringLiteral("metaLeft"));
		}
		else
		{
			if(mCtrl)
				mKeyboardDevice.releaseKey(QStringLiteral("controlLeft"));
			if(mAlt)
				mKeyboardDevice.releaseKey(QStringLiteral("altLeft"));
			if(mShift)
				mKeyboardDevice.releaseKey(QStringLiteral("shiftLeft"));
			if(mMeta)
				mKeyboardDevice.releaseKey(QStringLiteral("metaLeft"));
		}
	}
}
