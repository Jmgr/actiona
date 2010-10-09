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

#include "globalshortcut/globalshortcutmanager.h"
#include "qxtcommandoptions/qxtcommandoptions.h"
#include "actioninstance.h"
#include "version.h"
#include "mainclass.h"

#include <ctime>

#include <QApplication>
#include <QtGlobal>
#include <QDir>
#include <QTextCodec>
#include <QFile>

#ifdef Q_WS_X11
#undef signals
#include <libnotify/notify.h>
#define signals
#include "keysymhelper.h"
#endif

static void cleanup()
{
	ActionTools::GlobalShortcutManager::clear();
}

int main(int argc, char **argv)
{
#if (QT_VERSION < 0x040700)
		#error("You need Qt 4.7.0 or later to compile Actionaz Executer");
#endif

	QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);

	qAddPostRoutine(cleanup);

	qsrand(std::time(NULL));

#ifdef Q_WS_X11
	notify_init("Actionaz executer");
#endif

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	app.addLibraryPath(QDir::currentPath() + "/actions");

	qRegisterMetaType<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaType<ActionTools::ActionException::Exception>("Exception");
	qRegisterMetaType<ActionTools::Parameter>("Parameter");
	qRegisterMetaType<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaType<Tools::Version>("Version");

	qRegisterMetaTypeStreamOperators<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaTypeStreamOperators<ActionTools::Parameter>("Parameter");
	qRegisterMetaTypeStreamOperators<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaTypeStreamOperators<Tools::Version>("Version");

	QxtCommandOptions options;
	options.setFlagStyle(QxtCommandOptions::DoubleDash);
	options.setScreenWidth(0);
	options.add("console", QObject::tr("show the execution console"));
	options.alias("console", "c");
	options.add("execution-window", QObject::tr("show the execution window"));
	options.alias("execution-window", "e");
	options.add("stop-hotkey", QObject::tr("hotkey to stop the execution"));
	options.alias("stop-hotkey", "s");
	options.add("pause-hotkey", QObject::tr("hotkey to pause the execution"));
	options.alias("pause-hotkey", "p");
	options.add("debug-hotkey", QObject::tr("hotkey to pause and debug the execution"));
	options.alias("debug-hotkey", "d");
	options.add("code", QObject::tr("switch to code mode, may not be used with -r"));
	options.alias("code", "o");
	options.add("script", QObject::tr("switch to script mode, may not be used with -o"));
	options.alias("script", "r");
	options.add("version", QObject::tr("show the program version"));
	options.add("verbose", QObject::tr("be more verbose"));
	options.alias("verbose", "v");
	options.add("help", QObject::tr("show this help text"));
	options.alias("help", "h");
	options.parse(QCoreApplication::arguments());
	if(options.count("help") || options.showUnrecognizedWarning() || options.positional().count() < 1 || (options.count("code") && options.count("script")))
	{
		QTextStream stream(stdout);
		stream << QObject::tr("usage: ") << QCoreApplication::arguments().at(0) << " " << QObject::tr("[parameters]") << " " << QObject::tr("filename") << "\n";
		stream << QObject::tr("Parameters are:") << "\n";
		stream << options.getUsage();
		stream.flush();
		return -1;
	}

	QString filename = options.positional().at(0);
	
#ifdef Q_WS_X11
	{
#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("Load key codes");
#endif
		ActionTools::KeySymHelper::loadKeyCodes();
	}
#endif

	MainClass::ExecutionMode executionMode = MainClass::Unknown;
	if(options.count("code"))
		executionMode = MainClass::Code;
	else if(options.count("script"))
		executionMode = MainClass::Script;
	else
	{
		if(filename.endsWith(".act"))
			executionMode = MainClass::Script;
		else if(filename.endsWith(".cod"))
			executionMode = MainClass::Code;
		else
		{
			QTextStream stream(stdout);
			stream << QObject::tr("Unknown execution mode, please specify -o (script) or -r (code)") << "\n";
			stream.flush();
			return -1;
		}
	}

	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly))
	{
		QTextStream stream(stdout);
		stream << QObject::tr("Unable to read input file") << "\n";
		stream.flush();
		return -1;
	}

	MainClass mainClass(executionMode);
	if(!mainClass.start(file))
	{
		file.close();

		return -1;
	}

	return app.exec();
}
