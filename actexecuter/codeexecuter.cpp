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

#include "codeexecuter.h"
#include "executer/codeinitializer.h"
#include "executer/scriptagent.h"
#include "actionfactory.h"
#include "actionpack.h"
#include "executer/codestdio.h"
#include "code/codetools.h"
#include "code/prettyprinting.h"
#include "executer/codeactiona.h"
#include "global.h"
#include "languages.h"

#include <QFile>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QMainWindow>
#include <QTimer>
#include <QTextStream>
#include <QStringList>
#include <QApplication>
#include <QSettings>

CodeExecuter::CodeExecuter(QObject *parent) :
    Executer(parent),
	mScriptEngine(new QScriptEngine(this)),
	mScriptAgent(new LibExecuter::ScriptAgent(mScriptEngine)),
	mScriptEngineDebugger(new QScriptEngineDebugger(this)),
	mDebuggerWindow(mScriptEngineDebugger->standardWindow())
{
    connect(mScriptEngineDebugger, &QScriptEngineDebugger::evaluationResumed, this, &CodeExecuter::onEvaluationResumed);
    connect(mScriptEngineDebugger, &QScriptEngineDebugger::evaluationSuspended, this, &CodeExecuter::onEvaluationPaused);
    connect(mScriptAgent, &LibExecuter::ScriptAgent::executionStopped, this, &CodeExecuter::stopExecution);

    for(QString extension: mScriptEngine->availableExtensions())
		mScriptEngine->importExtension(extension);

    Code::setupPrettyPrinting(*mScriptEngine);
	
	mScriptEngineDebugger->setAutoShowStandardWindow(false);
	mScriptEngineDebugger->attachTo(mScriptEngine);
	QScriptEngineAgent *debuggerAgent = mScriptEngine->agent();
	mScriptEngine->setAgent(mScriptAgent);
	mScriptAgent->setDebuggerAgent(debuggerAgent);
}

bool CodeExecuter::start(QIODevice *device, const QString &filename)
{
	if(!Executer::start(device, filename))
		return false;
	
	QString code = QString::fromUtf8(device->readAll());
	device->close();
	
	mScriptAgent->setContext(LibExecuter::ScriptAgent::ActionInit);
    LibExecuter::CodeInitializer::initialize(mScriptEngine, mScriptAgent, actionFactory(), filename);

	Code::CodeTools::addClassToScriptEngine<LibExecuter::CodeStdio>(QStringLiteral("Console"), mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine(QStringLiteral("Console"), &LibExecuter::CodeStdio::print, QStringLiteral("print"), mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine(QStringLiteral("Console"), &LibExecuter::CodeStdio::println, QStringLiteral("println"), mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine(QStringLiteral("Console"), &LibExecuter::CodeStdio::printWarning, QStringLiteral("printWarning"), mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine(QStringLiteral("Console"), &LibExecuter::CodeStdio::printlnWarning, QStringLiteral("printlnWarning"), mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine(QStringLiteral("Console"), &LibExecuter::CodeStdio::printError, QStringLiteral("printError"), mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine(QStringLiteral("Console"), &LibExecuter::CodeStdio::printlnError, QStringLiteral("printlnError"), mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine(QStringLiteral("Console"), &LibExecuter::CodeStdio::clear, QStringLiteral("clear"), mScriptEngine);

	Code::CodeTools::addClassToScriptEngine<LibExecuter::CodeActiona>(QStringLiteral("Actiona"), mScriptEngine);
    LibExecuter::CodeActiona::setActExec(true);
    LibExecuter::CodeActiona::setActionaVersion(Global::ACTIONA_VERSION);
    LibExecuter::CodeActiona::setScriptVersion(Global::SCRIPT_VERSION);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine(QStringLiteral("Actiona"), &LibExecuter::CodeActiona::version, QStringLiteral("version"), mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine(QStringLiteral("Actiona"), &LibExecuter::CodeActiona::scriptVersion, QStringLiteral("scriptVersion"), mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine(QStringLiteral("Actiona"), &LibExecuter::CodeActiona::isActExec, QStringLiteral("isActExec"), mScriptEngine);
	Code::CodeTools::addClassGlobalFunctionToScriptEngine(QStringLiteral("Actiona"), &LibExecuter::CodeActiona::isActiona, QStringLiteral("isActiona"), mScriptEngine);

    QString locale = Tools::Languages::locale();

	for(int actionPackIndex = 0; actionPackIndex < actionFactory()->actionPackCount(); ++actionPackIndex)
	{
		ActionTools::ActionPack *actionPack = actionFactory()->actionPack(actionPackIndex);

        Tools::Languages::installTranslator(QStringLiteral("actionpack%1").arg(actionPack->id()), locale);
	}

	mScriptAgent->setContext(LibExecuter::ScriptAgent::Parameters);
	
	QScriptValue result = mScriptEngine->evaluate(code, filename);
	if(result.isError())
	{
		QTextStream stream(stdout);
		stream << QObject::tr("Uncaught exception: ") << result.toString() << "\n";
		stream << tr("Backtrace: ") << mScriptEngine->uncaughtExceptionBacktrace().join(QStringLiteral("\n")) << "\n";
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
	mDebuggerWindow->show();
}

void CodeExecuter::stopExecution()
{
	mScriptEngine->abortEvaluation();
}
