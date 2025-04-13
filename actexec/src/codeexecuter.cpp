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

#include "codeexecuter.hpp"
#include "execution/codeinitializer.hpp"
#include "execution/codestdio.hpp"
#include "actiontools/scriptengine.hpp"
#include "actiontools/actionfactory.hpp"
#include "actiontools/actionpack.hpp"
#include "execution/codeactiona.hpp"
#include "global.hpp"
#include "tools/languages.hpp"

#include <QFile>
#include <QMainWindow>
#include <QTimer>
#include <QTextStream>
#include <QStringList>
#include <QApplication>
#include <QSettings>

CodeExecuter::CodeExecuter(QObject *parent) :
    Executer(parent),
    mScriptEngine(std::make_unique<ActionTools::ScriptEngine>())
{
}

CodeExecuter::~CodeExecuter()
{
}

bool CodeExecuter::start(QIODevice *device, const QString &filename)
{
	if(!Executer::start(device, filename))
		return false;
	
	QString code = QString::fromUtf8(device->readAll());
    device->close();

    mScriptEngine->setContext(ActionTools::ScriptEngine::ActionInit);
    Execution::CodeInitializer::initialize(*mScriptEngine, actionFactory(), filename);

    Execution::CodeStdio::registerClass(*mScriptEngine);

    // Make Console an alias for Stdio
    mScriptEngine->evaluate(QStringLiteral("Console = Stdio"));

    Execution::CodeActiona::registerClass(*mScriptEngine);
    Execution::CodeActiona::setActExec(true);
    Execution::CodeActiona::setActionaVersion(Global::ACTIONA_VERSION);
    Execution::CodeActiona::setScriptVersion(Global::SCRIPT_VERSION);

    QString locale = Tools::Languages::locale();

	for(int actionPackIndex = 0; actionPackIndex < actionFactory()->actionPackCount(); ++actionPackIndex)
	{
		ActionTools::ActionPack *actionPack = actionFactory()->actionPack(actionPackIndex);

        Tools::Languages::installTranslator(QStringLiteral("actionpack%1").arg(actionPack->id()), locale);
	}

    mScriptEngine->setContext(ActionTools::ScriptEngine::Actions);

    QStringList exceptionStackTrace;
    QJSValue result = mScriptEngine->evaluate(code, filename, 1, &exceptionStackTrace);
	if(result.isError())
	{
		QTextStream stream(stdout);
		stream << QObject::tr("Uncaught exception: ") << result.toString() << "\n";
        stream << tr("Backtrace: ") << exceptionStackTrace.join(QStringLiteral("\n")) << "\n";
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
    mScriptEngine->setInterrupted(true);
}
