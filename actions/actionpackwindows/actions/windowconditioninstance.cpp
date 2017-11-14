/*
	Actiona
	Copyright (C) 2005-2017 Jonathan Mercier-Ganady

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

#include "windowconditioninstance.h"
#include "code/point.h"
#include "code/size.h"

#include <QRegExp>

namespace Actions
{
	ActionTools::StringListPair WindowConditionInstance::conditions = qMakePair(
			QStringList() << QStringLiteral("exists") << QStringLiteral("dontexists"),
			QStringList()
			<< QStringLiteral(QT_TRANSLATE_NOOP("WindowConditionInstance::conditions", "Exists"))
			<< QStringLiteral(QT_TRANSLATE_NOOP("WindowConditionInstance::conditions", "Don't exists")));

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

		mTitleRegExp = QRegExp(title, Qt::CaseSensitive, QRegExp::WildcardUnix);

		ActionTools::WindowHandle foundWindow = findWindow();
		if((foundWindow.isValid() && mCondition == Exists) ||
		   (!foundWindow.isValid() && mCondition == DontExists))
		{
			QString line = evaluateSubParameter(ok, mIfTrue.actionParameter());

			if(!ok)
				return;

			if(mIfTrue.action() == ActionTools::IfActionValue::GOTO)
				setNextLine(line);
			else if(mIfTrue.action() == ActionTools::IfActionValue::CALLPROCEDURE)
			{
				if(!callProcedure(line))
					return;
			}

			emit executionEnded();
		}
		else
		{
			QString line = evaluateSubParameter(ok, ifFalse.actionParameter());

			if(!ok)
				return;

			if(ifFalse.action() == ActionTools::IfActionValue::GOTO)
			{
				setNextLine(line);

				emit executionEnded();
			}
			else if(ifFalse.action() == ActionTools::IfActionValue::CALLPROCEDURE)
			{
				if(!callProcedure(line))
					return;

				emit executionEnded();
			}
			else if(ifFalse.action() == ActionTools::IfActionValue::WAIT)
			{
				connect(&mTimer, SIGNAL(timeout()), this, SLOT(checkWindow()));
				mTimer.setInterval(100);
				mTimer.start();
			}
			else
				emit executionEnded();
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

			if(mIfTrue.action() == ActionTools::IfActionValue::GOTO)
				setNextLine(line);
			else if(mIfTrue.action() == ActionTools::IfActionValue::CALLPROCEDURE)
			{
				if(!callProcedure(line))
					return;
			}

			mTimer.stop();
			emit executionEnded();
		}
	}

	ActionTools::WindowHandle WindowConditionInstance::findWindow()
	{
		ActionTools::WindowHandle foundWindow = ActionTools::WindowHandle::findWindow(mTitleRegExp);
		if(foundWindow.isValid())
		{
			QRect windowRect = foundWindow.rect();

            setVariable(mPosition, Code::Point::constructor(windowRect.topLeft(), scriptEngine()));
            setVariable(mSize, Code::Size::constructor(windowRect.size(), scriptEngine()));
            setVariable(mXCoordinate, windowRect.x());
            setVariable(mYCoordinate, windowRect.y());
            setVariable(mWidth, windowRect.width());
            setVariable(mHeight, windowRect.height());
            setVariable(mProcessId, foundWindow.processId());

			return foundWindow;
		}

		return ActionTools::WindowHandle();
	}
}
