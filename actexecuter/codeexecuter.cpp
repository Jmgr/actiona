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

#include "codeexecuter.h"
#include "executer/codeinitializer.h"
#include "executer/scriptagent.h"

#include <QFile>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QMainWindow>
#include <QTimer>
#include <QTextStream>
#include <QStringList>

CodeExecuter::CodeExecuter(QObject *parent) :
    Executer(parent),
	mScriptEngine(new QScriptEngine(this)),
	mScriptAgent(new LibExecuter::ScriptAgent(mScriptEngine)),
	mScriptEngineDebugger(new QScriptEngineDebugger(this)),
	mDebuggerWindow(mScriptEngineDebugger->standardWindow())
{
	connect(mScriptEngineDebugger, SIGNAL(evaluationResumed()), this, SLOT(onEvaluationResumed()));
	connect(mScriptEngineDebugger, SIGNAL(evaluationSuspended()), this, SLOT(onEvaluationPaused()));
	connect(mScriptAgent, SIGNAL(stopExecution()), this, SLOT(stopExecution()));
	
	mScriptEngineDebugger->attachTo(mScriptEngine);
	QScriptEngineAgent *debuggerAgent = mScriptEngine->agent();
	mScriptEngine->setAgent(mScriptAgent);
	mScriptAgent->setDebuggerAgent(debuggerAgent);
}

bool CodeExecuter::start(QFile &file)
{
	if(!Executer::start(file))
		return false;
	
	QString code = file.readAll();
	file.close();
	
	LibExecuter::CodeInitializer::initialize(mScriptEngine, mScriptAgent, actionFactory());
	
	QScriptValue result = mScriptEngine->evaluate(code, file.fileName());
	if(result.isError())
	{
		QTextStream stream(stdout);
		stream << QObject::tr("Uncaught exception: ") << result.toString() << "\n";
		stream << tr("Backtrace: ") << mScriptEngine->uncaughtExceptionBacktrace().join("\n") << "\n";
		stream.flush();
	}
	
	QTimer::singleShot(0, qApp, SLOT(quit()));
	
	return true;
}

void CodeExecuter::onEvaluationResumed()
{
	mDebuggerWindow->hide();
}

void CodeExecuter::onEvaluationPaused()
{
	
}

void CodeExecuter::stopExecution()
{
	mScriptEngine->abortEvaluation();
}
