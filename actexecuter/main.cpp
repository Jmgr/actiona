/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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
#include "nativeeventfilteringapplication.h"
#include "global.h"

#include <ctime>

#include <QApplication>
#include <QtGlobal>
#include <QDir>
#include <QTextCodec>
#include <QFile>
#include <QSettings>
#include <QTranslator>
#include <QLibraryInfo>

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

	ActionTools::NativeEventFilteringApplication app("actionaz-exec", argc, argv);
	app.setQuitOnLastWindowClosed(false);

	qAddPostRoutine(cleanup);

	qsrand(std::time(NULL));

#ifdef Q_WS_X11
	notify_init("Actionaz executer");
#endif

	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	QxtCommandOptions options;
	options.setFlagStyle(QxtCommandOptions::DoubleDash);
	options.setScreenWidth(0);
	options.add("code", QObject::tr("switch to code mode, may not be used with -r"));
	options.alias("code", "c");
	options.add("script", QObject::tr("switch to script mode, may not be used with -o"));
	options.alias("script", "s");
	options.add("nocodeqt", QObject::tr("do not include the Qt library into the code"));
	options.alias("nocodeqt", "Q");
	options.add("portable", QObject::tr("starts in portable mode, storing the settings in the executable folder"));
	options.alias("portable", "p");
	options.add("version", QObject::tr("show the program version"));
	options.add("help", QObject::tr("show this help text"));
	options.alias("help", "h");
	options.parse(QCoreApplication::arguments());

	if(options.count("portable"))
	{
		QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QApplication::applicationDirPath() + "/userSettings");
		QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, QApplication::applicationDirPath() + "/systemSettings");
		QSettings::setDefaultFormat(QSettings::IniFormat);
	}

	QSettings settings;

	QString locale = settings.value("locale", QLocale::system().name()).toString();

	QTranslator qtTranslator;
#ifdef Q_WS_WIN
	qtTranslator.load(QString("%1/locale/qt_%2").arg(QApplication::applicationDirPath()).arg(locale));
#else
	qtTranslator.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
	app.installTranslator(&qtTranslator);

	QTranslator toolsTranslator;
	if(!toolsTranslator.load(QString("%1/locale/tools_%2").arg(QApplication::applicationDirPath()).arg(locale)))
	{
#ifndef Q_WS_WIN
		toolsTranslator.load(QString("%1/share/actionaz/locale/tools_%2").arg(ACT_PREFIX).arg(locale));
#endif
	}
	app.installTranslator(&toolsTranslator);

	QTranslator actionToolsTranslator;
	if(!actionToolsTranslator.load(QString("%1/locale/actiontools_%2").arg(QApplication::applicationDirPath()).arg(locale)))
	{
#ifndef Q_WS_WIN
		actionToolsTranslator.load(QString("%1/share/actionaz/locale/actiontools_%2").arg(ACT_PREFIX).arg(locale));
#endif
	}
	app.installTranslator(&actionToolsTranslator);

	QTranslator executerTranslator;
	if(!executerTranslator.load(QString("%1/locale/executer_%2").arg(QApplication::applicationDirPath()).arg(locale)))
	{
#ifndef Q_WS_WIN
		executerTranslator.load(QString("%1/share/actionaz/locale/executer_%2").arg(ACT_PREFIX).arg(locale));
#endif
	}
	app.installTranslator(&executerTranslator);

	QTranslator actexecuterTranslator;
	if(!actexecuterTranslator.load(QString("%1/locale/actexecuter_%2").arg(QApplication::applicationDirPath()).arg(locale)))
	{
#ifndef Q_WS_WIN
		actexecuterTranslator.load(QString("%1/share/actionaz/locale/actexecuter_%2").arg(ACT_PREFIX).arg(locale));
#endif
	}
	app.installTranslator(&actexecuterTranslator);

	qRegisterMetaType<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaType<ActionTools::ActionException::Exception>("Exception");
	qRegisterMetaType<ActionTools::Parameter>("Parameter");
	qRegisterMetaType<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaType<Tools::Version>("Version");

	qRegisterMetaTypeStreamOperators<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaTypeStreamOperators<ActionTools::Parameter>("Parameter");
	qRegisterMetaTypeStreamOperators<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaTypeStreamOperators<Tools::Version>("Version");

	if(options.count("version"))
	{
		QTextStream stream(stdout);
		stream << "Actionaz Executer version " << Global::ACTIONAZ_VERSION.toString() << ", script version " << Global::SCRIPT_VERSION.toString() << "\n";
		stream.flush();
		return 0;
	}
	if(options.count("help") || options.showUnrecognizedWarning() || options.positional().count() < 1 || (options.count("code") && options.count("script")))
	{
		QTextStream stream(stdout);
		stream << QObject::tr("usage: ") << QCoreApplication::arguments().at(0) << " " << QObject::tr("[parameters]") << " " << QObject::tr("filename") << "\n";
		stream << QObject::tr("Parameters are:") << "\n";
		stream << options.getUsage();
		stream.flush();
		return -1;
	}

	app.addLibraryPath(QApplication::applicationDirPath() + "/actions");
	app.addLibraryPath(QApplication::applicationDirPath() + "/plugins");

	if(!options.count("nocodeqt"))
		app.addLibraryPath(QApplication::applicationDirPath() + "/code");

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
		if(filename.endsWith(".ascr"))
			executionMode = MainClass::Script;
		else if(filename.endsWith(".acod"))
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
