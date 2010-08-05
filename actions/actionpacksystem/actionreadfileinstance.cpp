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

#include "actionreadfileinstance.h"
#include "actioninstanceexecutionhelper.h"

#include <QFile>
#include <QTextStream>

ActionTools::StringListPair ActionReadFileInstance::modes = qMakePair(
		QStringList() << "full" << "selection",
		QStringList() << tr("Read the entire file") << tr("Read only a selection"));

void ActionReadFileInstance::startExecution()
{
	ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
	QString filepath;
	QString variable;
	Mode mode;
	int firstline;
	int lastline;

	if(!actionInstanceExecutionHelper.evaluateString(filepath, "file") ||
	   !actionInstanceExecutionHelper.evaluateVariable(variable, "variable") ||
	   !actionInstanceExecutionHelper.evaluateListElement(mode, "mode", "value", modes) ||
	   !actionInstanceExecutionHelper.evaluateInteger(firstline, "firstline") ||
	   !actionInstanceExecutionHelper.evaluateInteger(lastline, "lastline"))
		return;

	if(mode == Selection)
	{
		if(firstline < 1)
		{
			actionInstanceExecutionHelper.setCurrentParameter("firstline");
			emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid first line value : %1").arg(firstline));
			return;
		}

		if(lastline < 1)
		{
			actionInstanceExecutionHelper.setCurrentParameter("lastline");
			emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid last line value : %1").arg(lastline));
			return;
		}

		if(lastline < firstline)
		{
			actionInstanceExecutionHelper.setCurrentParameter("firstline");
			emit executionException(ActionTools::ActionException::BadParameterException, tr("The first line has to be smaller than the last line"));
			return;
		}
	}

	QFile file(filepath);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		actionInstanceExecutionHelper.setCurrentParameter("file");
		emit executionException(CannotOpenFileException, tr("Cannot open file"));
		return;
	}

	//Line counting starts at 1
	--firstline;
	--lastline;

	QString result;

	if(mode == Full)
		result = file.readAll();
	else
	{
		QTextStream stream(&file);

		for(int line = 0; !stream.atEnd(); ++line)
		{
			QString readLine = stream.readLine();

			if(line >= firstline && line <= lastline)
			{
				if(!result.isEmpty())
					result += '\n';

				result += readLine;
			}

			if(line > lastline)
				break;
		}
	}

	actionInstanceExecutionHelper.setVariable(variable, result);

	emit executionEnded();
}
