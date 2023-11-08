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

#include "keyboardkeyconditioninstance.hpp"

#include <QTimer>

namespace Actions
{
    Tools::StringListPair KeyboardKeyConditionInstance::conditions =
    {
        {
            QStringLiteral("pressed"),
            QStringLiteral("notpressed")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("KeyboardKeyConditionInstance::conditions", "Is pressed")),
            QStringLiteral(QT_TRANSLATE_NOOP("KeyboardKeyConditionInstance::conditions", "Is not pressed"))
        }
    };

    KeyboardKeyConditionInstance::KeyboardKeyConditionInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
        : ActionTools::ActionInstance(definition, parent),
          mCondition(Pressed),
          mTimer(new QTimer(this))
	{
        mTimer->setTimerType(Qt::PreciseTimer);
	}

    void KeyboardKeyConditionInstance::startExecution()
	{
		bool ok = true;

        QString keysString = evaluateString(ok, QStringLiteral("keys"));
        mKeyList = ActionTools::KeyboardKey::loadKeyListFromJson(keysString);
        if(mKeyList.isEmpty())
        {
            setCurrentParameter(QStringLiteral("keys"));
            emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid key combination"));

            return;
        }

		mCondition = evaluateListElement<Condition>(ok, conditions, QStringLiteral("condition"));
		mIfTrue = evaluateIfAction(ok, QStringLiteral("ifTrue"));
		ActionTools::IfActionValue ifFalse = evaluateIfAction(ok, QStringLiteral("ifFalse"));

		if(!ok)
			return;

        auto pressed = areKeysPressed();
        if((pressed && mCondition == Pressed) ||
           (!pressed && mCondition == NotPressed))
        {
            QString line = evaluateSubParameter(ok, mIfTrue.actionParameter());

            if(!ok)
                return;

            bool stopScript = false;
            if(mIfTrue.action() == ActionTools::IfActionValue::GOTO)
                setNextLine(line);
            else if(mIfTrue.action() == ActionTools::IfActionValue::CALLPROCEDURE)
            {
                if(!callProcedure(line))
                    return;
            }
            else if(mIfTrue.action() == ActionTools::IfActionValue::STOPEXECUTION)
            {
                stopScript = true;
            }

            executionEnded(stopScript);
        }
        else
        {
            QString line = evaluateSubParameter(ok, ifFalse.actionParameter());

            if(!ok)
                return;

            if(ifFalse.action() == ActionTools::IfActionValue::GOTO)
            {
                setNextLine(line);

                executionEnded();
            }
            else if(ifFalse.action() == ActionTools::IfActionValue::CALLPROCEDURE)
            {
                if(!callProcedure(line))
                    return;

                executionEnded();
            }
            else if(ifFalse.action() == ActionTools::IfActionValue::STOPEXECUTION)
            {
                bool stopScript = true;
                executionEnded(stopScript);
            }
            else if(ifFalse.action() == ActionTools::IfActionValue::WAIT)
            {
                connect(mTimer, &QTimer::timeout, this, [this]()
                {
                    auto pressed = areKeysPressed();
                    if((pressed && mCondition == Pressed) ||
                       (!pressed && mCondition == NotPressed))
                    {
                        bool ok = true;

                        QString line = evaluateSubParameter(ok, mIfTrue.actionParameter());
                        if(!ok)
                            return;

                        bool stopScript = false;
                        if(mIfTrue.action() == ActionTools::IfActionValue::GOTO)
                            setNextLine(line);
                        else if(mIfTrue.action() == ActionTools::IfActionValue::CALLPROCEDURE)
                        {
                            if(!callProcedure(line))
                                return;
                        }
                        else if(mIfTrue.action() == ActionTools::IfActionValue::STOPEXECUTION)
                        {
                            stopScript = true;
                        }

                        mTimer->stop();
                        executionEnded(stopScript);
                    }
                });
                mTimer->setInterval(100);
                mTimer->start();
            }
            else
                executionEnded();
        }
    }

    bool KeyboardKeyConditionInstance::areKeysPressed() const
    {
        for(const auto &key: mKeyList)
        {
            if(!key.isPressed())
                return false;
        }

        return true;
    }

    void KeyboardKeyConditionInstance::stopExecution()
	{
        mTimer->stop();
        mTimer->disconnect();
	}
}
