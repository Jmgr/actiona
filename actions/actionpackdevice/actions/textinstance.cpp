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

#include "textinstance.h"
#include "actioninstanceexecutionhelper.h"

#include <QTimer>

namespace Actions
{
	void TextInstance::startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
	
		QString text;
	
		if(!actionInstanceExecutionHelper.evaluateString(text, "text"))
			return;
		
		if(!mKeyboardDevice.writeText(text))
		{
			emit executionException(FailedToSendInputException, tr("Unable to write the text"));
			return;
		}
	
		QTimer::singleShot(1, this, SIGNAL(executionEnded()));
	}

	void TextInstance::stopLongTermExecution()
	{
		mKeyboardDevice.reset();
	}
}
