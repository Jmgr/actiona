/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#ifndef ACTIONTIMECONDITIONINSTANCE_H
#define ACTIONTIMECONDITIONINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "actioninstance.h"

#include <QDateTime>
#include <QTimer>

class ActionTimeConditionInstance : public ActionTools::ActionInstance
{
	Q_OBJECT

public:
	ActionTimeConditionInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
		: ActionTools::ActionInstance(definition, parent)										{}

	void startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
		QString date;
		QString ifBeforeAction;
		QString ifBeforeLine;
		QString ifNowAction;
		QString ifNowLine;
		QString ifAfterAction;
		QString ifAfterLine;

		if(!actionInstanceExecutionHelper.evaluateString(date, "date") ||
		   !actionInstanceExecutionHelper.evaluateString(ifBeforeAction, "ifbefore", "action") ||
		   !actionInstanceExecutionHelper.evaluateString(ifBeforeLine, "ifbefore", "line") ||
		   !actionInstanceExecutionHelper.evaluateString(ifNowAction, "ifnow", "action") ||
		   !actionInstanceExecutionHelper.evaluateString(ifNowLine, "ifnow", "line") ||
		   !actionInstanceExecutionHelper.evaluateString(ifAfterAction, "ifafter", "action") ||
		   !actionInstanceExecutionHelper.evaluateString(ifAfterLine, "ifafter", "line"))
			return;
		
		mTestedDateTime = QDateTime::fromString(date, "dd/MM/yyyy hh:mm:ss");
		if(!mTestedDateTime.isValid())
		{
			actionInstanceExecutionHelper.setCurrentParameter("date");
			emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid date"));
			return;
		}
		
		QString action;
		QString line;
		if(mTestedDateTime < QDateTime::currentDateTime())//Before
		{
			action = ifBeforeAction;
			line = ifBeforeLine;
		}
		else if(mTestedDateTime > QDateTime::currentDateTime())//After
		{
			if(ifAfterAction == "wait")
			{
				connect(&mTimer, SIGNAL(timeout()), this, SLOT(checkDateTime()));
				mTimer.setInterval(1000);
				mTimer.start();
				
				return;
			}
			
			action = ifAfterAction;
			line = ifAfterLine;
		}
		else//Now
		{
			action = ifNowAction;
			line = ifNowLine;
		}
		
		if(action == "goto")
			actionInstanceExecutionHelper.setNextLine(line);

		emit executionEnded();
	}
	
	void stopExecution()
	{
		mTimer.stop();
	}
	
private slots:
	void checkDateTime()
	{
		if(mTestedDateTime <= QDateTime::currentDateTime())
		{
			mTimer.stop();
			emit executionEnded();
		}
	}

private:
	QTimer mTimer;
	QDateTime mTestedDateTime;
	
	Q_DISABLE_COPY(ActionTimeConditionInstance)
};

#endif // ACTIONTIMECONDITIONINSTANCE_H
