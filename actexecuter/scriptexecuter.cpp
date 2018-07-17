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

	Contact : jmgr@jmgr.info
*/

#include "scriptexecuter.h"
#include "script.h"
#include "executer/executer.h"
#include "mainclass.h"
#include "global.h"

#include <QFile>
#include <QApplication>
#include <QTextStream>

ScriptExecuter::ScriptExecuter(QObject *parent) :
    Executer(parent),
	mScript(new ActionTools::Script(actionFactory(), this)),
	mExecuter(new LibExecuter::Executer(this))
{
    connect(mExecuter, &LibExecuter::Executer::executionStopped, this, &ScriptExecuter::executionStopped);
    connect(mExecuter, &LibExecuter::Executer::scriptError, this, &ScriptExecuter::scriptError);
}

bool ScriptExecuter::start(QIODevice *device, const QString &filename)
{
	if(!Executer::start(device, filename))
		return false;
	
	ActionTools::Script::ReadResult result = mScript->read(device, Global::SCRIPT_VERSION);
	switch(result)
	{
	case ActionTools::Script::ReadInternal:
		{
			QTextStream stream(stdout);
			stream << QObject::tr("Reading script file failed due to an internal error") << "\n";
			stream.flush();
		}
		return false;
    case ActionTools::Script::ReadInvalidSchema:
		{
			QTextStream stream(stdout);
            stream << QObject::tr("Input script file has an invalid script schema") << "\n";
			stream.flush();
		}
		return false;
    case ActionTools::Script::ReadInvalidScriptVersion:
		{
			QTextStream stream(stdout);
			stream << QObject::tr("Input script file is too recent") << "\n";
			stream.flush();
		}
		return false;
	default:
		break;
	}
	
	device->close();
	
    mExecuter->setup(mScript, actionFactory(), false, 0, 0, false, 0, 0, mScript->pauseBefore(), mScript->pauseAfter(), Global::ACTIONA_VERSION, Global::SCRIPT_VERSION, true, nullptr);
    if(!mExecuter->startExecution(false, filename))
	{
		QTextStream stream(stdout);
		stream << QObject::tr("Start execution failed") << "\n";
		stream.flush();
		return false;
	}
	
	return true;
}

void ScriptExecuter::executionStopped()
{
	QApplication::quit();
}

void ScriptExecuter::scriptError(int actionIndex, const QString &parameter, const QString &error)
{
	Q_UNUSED(actionIndex)
	Q_UNUSED(parameter)
	
	QTextStream stream(stdout);
	stream << QObject::tr("Execution error: ") << error << "\n";
	stream.flush();
	
	QApplication::quit();
}
