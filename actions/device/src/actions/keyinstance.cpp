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
#include "backend/keyinput.hpp"
#include "backend/keyboard.hpp"

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
        auto &keyboard = Backend::Instance::keyboard();

		bool ok = true;

		mKey = evaluateString(ok, QStringLiteral("key"), QStringLiteral("key"));
		auto action = evaluateListElement<Action>(ok, actions, QStringLiteral("action"));
		mAmount = evaluateInteger(ok, QStringLiteral("amount"));
		mCtrl = evaluateBoolean(ok, QStringLiteral("ctrl"));
		mAlt = evaluateBoolean(ok, QStringLiteral("alt"));
		mShift = evaluateBoolean(ok, QStringLiteral("shift"));
		mMeta = evaluateBoolean(ok, QStringLiteral("meta"));
		Type type = evaluateListElement<Type>(ok, types, QStringLiteral("type"));
        mDirectX = (type == DirectXType);
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

        try
        {
            switch(action)
            {
            case PressAction:
                pressOrReleaseModifiers(true);

                keyboard.pressKey(mKey, true, mDirectX);
                break;
            case ReleaseAction:
                pressOrReleaseModifiers(false);

                keyboard.pressKey(mKey, false, mDirectX);
                break;
            case PressReleaseAction:
                pressOrReleaseModifiers(true);

                keyboard.pressKey(mKey, true, mDirectX);

                mTimer->setSingleShot(true);
                mTimer->start(mPause);
                break;
            }
        }
        catch(const Backend::BackendError &e)
        {
            emit executionException(FailedToSendInputException, e.what());
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
        auto &keyboard = Backend::Instance::keyboard();

		pressOrReleaseModifiers(false);

        try
        {
            keyboard.pressKey(mKey, false, mDirectX);
        }
        catch(const Backend::BackendError &e)
        {
            emit executionException(FailedToSendInputException, e.what());
            return;
        }

		--mAmount;
		if (mAmount > 0)
            sendPressKey();
		else
			executionEnded();
	}

	void KeyInstance::sendPressKey()
	{
        auto &keyboard = Backend::Instance::keyboard();

		pressOrReleaseModifiers(true);

        try
        {
            keyboard.pressKey(mKey, true, mDirectX);
        }
        catch(const Backend::BackendError &e)
        {
            emit executionException(FailedToSendInputException, e.what());
            return;
        }

		mTimer->start(mPause);
	}

	void KeyInstance::pressOrReleaseModifiers(bool press)
	{
        auto &keyboard = Backend::Instance::keyboard();

        try
        {
            if(press)
            {
                if(mCtrl)
                    keyboard.pressKey(QStringLiteral("controlLeft"), true, mDirectX);
                if(mAlt)
                    keyboard.pressKey(QStringLiteral("altLeft"), true, mDirectX);
                if(mShift)
                    keyboard.pressKey(QStringLiteral("shiftLeft"), true, mDirectX);
                if(mMeta)
                    keyboard.pressKey(QStringLiteral("metaLeft"), true, mDirectX);
            }
            else
            {
                if(mCtrl)
                    keyboard.pressKey(QStringLiteral("controlLeft"), false, mDirectX);
                if(mAlt)
                    keyboard.pressKey(QStringLiteral("altLeft"), false, mDirectX);
                if(mShift)
                    keyboard.pressKey(QStringLiteral("shiftLeft"), false, mDirectX);
                if(mMeta)
                    keyboard.pressKey(QStringLiteral("metaLeft"), false, mDirectX);
            }
        }
        catch(const Backend::BackendError &e)
        {
            emit executionException(FailedToSendInputException, e.what());
            return;
        }
	}
}
