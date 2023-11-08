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

#include "windowconditioninstance.hpp"
#include "actiontools/code/point.hpp"
#include "actiontools/code/size.hpp"

#include <QRegularExpression>

namespace Actions
{
    Tools::StringListPair WindowConditionInstance::conditions =
    {
        {
            QStringLiteral("exists"),
            QStringLiteral("dontexists")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("WindowConditionInstance::conditions", "Exists")),
            QStringLiteral(QT_TRANSLATE_NOOP("WindowConditionInstance::conditions", "Do not exist"))
        }
    };

	WindowConditionInstance::WindowConditionInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent), mCondition(Exists)
	{
	}

	void WindowConditionInstance::startExecution()
	{
		bool ok = true;

		QString title = evaluateString(ok, QStringLiteral("title"));
		mCondition = evaluateListElement<Condition>(ok, conditions, QStringLiteral("condition"));
		mIfTrue = evaluateIfAction(ok, QStringLiteral("ifTrue"));
		ActionTools::IfActionValue ifFalse = evaluateIfAction(ok, QStringLiteral("ifFalse"));
		mPosition = evaluateVariable(ok, QStringLiteral("position"));
		mSize = evaluateVariable(ok, QStringLiteral("size"));
		mXCoordinate = evaluateVariable(ok, QStringLiteral("xCoordinate"));
		mYCoordinate = evaluateVariable(ok, QStringLiteral("yCoordinate"));
		mWidth = evaluateVariable(ok, QStringLiteral("width"));
		mHeight = evaluateVariable(ok, QStringLiteral("height"));
		mProcessId = evaluateVariable(ok, QStringLiteral("processId"));

		if(!ok)
			return;

        mTitleRegExp = QRegularExpression::fromWildcard(title, Qt::CaseSensitive);

		ActionTools::WindowHandle foundWindow = findWindow();
		if((foundWindow.isValid() && mCondition == Exists) ||
		   (!foundWindow.isValid() && mCondition == DontExists))
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

            bool stopScript = false;
			if(ifFalse.action() == ActionTools::IfActionValue::GOTO)
			{
				setNextLine(line);
			}
			else if(ifFalse.action() == ActionTools::IfActionValue::CALLPROCEDURE)
			{
				if(!callProcedure(line))
					return;
			}
            else if(ifFalse.action() == ActionTools::IfActionValue::STOPEXECUTION)
            {
                stopScript = true;
            }
			else if(ifFalse.action() == ActionTools::IfActionValue::WAIT)
			{
                connect(&mTimer, &QTimer::timeout, this, &WindowConditionInstance::checkWindow);
				mTimer.setInterval(100);
				mTimer.start();
                return;
			}

            executionEnded(stopScript);
		}
	}

	void WindowConditionInstance::stopExecution()
	{
		mTimer.stop();
	}

	void WindowConditionInstance::checkWindow()
	{
		ActionTools::WindowHandle foundWindow = findWindow();
		if((foundWindow.isValid() && mCondition == Exists) ||
		   (!foundWindow.isValid() && mCondition == DontExists))
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

			mTimer.stop();
            executionEnded(stopScript);
		}
	}

	ActionTools::WindowHandle WindowConditionInstance::findWindow()
	{
		ActionTools::WindowHandle foundWindow = ActionTools::WindowHandle::findWindow(mTitleRegExp);
		if(foundWindow.isValid())
		{
			QRect windowRect = foundWindow.rect();

            setVariable(mPosition, Code::CodeClass::construct<Code::Point>(windowRect.topLeft(), *scriptEngine()));
            setVariable(mSize, Code::CodeClass::construct<Code::Size>(windowRect.size(), *scriptEngine()));
            setVariable(mXCoordinate, windowRect.x());
            setVariable(mYCoordinate, windowRect.y());
            setVariable(mWidth, windowRect.width());
            setVariable(mHeight, windowRect.height());
            setVariable(mProcessId, foundWindow.processId());

			return foundWindow;
		}

		return {};
	}
}
