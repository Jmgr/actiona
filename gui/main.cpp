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

#include <QtGlobal>

#include "mainwindow.h"
#include "actioninstance.h"
#include "parameter.h"
#include "subparameter.h"
#include "actioninstancebuffer.h"
#include "global.h"
#include "version.h"
#include "globalshortcut/globalshortcutmanager.h"
#include "qxtcommandoptions/qxtcommandoptions.h"
#include "progresssplashscreen.h"
#include "nativeeventfilteringapplication.h"

#include <ctime>

#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QTextCodec>
#include <QElapsedTimer>
#include <QTranslator>
#include <QLibraryInfo>
#include <QSettings>

#ifdef Q_WS_X11
#undef signals
#include <libnotify/notify.h>
#define signals

#include "xdisplayhelper.h"
#include "keysymhelper.h"
#endif

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

static void cleanup()
{
	ActionTools::GlobalShortcutManager::clear();
}

int main(int argc, char **argv)
{
#if (QT_VERSION < 0x040700)
		#error("You need Qt 4.7.0 or later to compile Actionaz");
#endif

#ifdef ACT_PROFILE
	Tools::HighResolutionTimer timer("Application run");
#endif
	ActionTools::NativeEventFilteringApplication app("actionaz-gui", argc, argv);
	app.setQuitOnLastWindowClosed(false);

	app.setOrganizationName("Actionaz");
	app.setOrganizationDomain("actionaz.org");
	app.setApplicationName("Actionaz");
	app.setApplicationVersion(Global::ACTIONAZ_VERSION.toString());

	qAddPostRoutine(cleanup);

	qsrand(std::time(NULL));

	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	QxtCommandOptions options;
	options.setFlagStyle(QxtCommandOptions::DoubleDash);
	options.setScreenWidth(0);
	options.add("nosplash", QObject::tr("disable the splash screen"));
	options.alias("nosplash", "s");
	options.add("notrayicon", QObject::tr("disable the tray icon"));
	options.alias("notrayicon", "t");
	options.add("noexecutionwindow", QObject::tr("do not show the execution window"));
	options.alias("noexecutionwindow", "E");
	options.add("noconsolewindow", QObject::tr("do not show the console window"));
	options.alias("noconsolewindow", "C");
	options.add("nocodeqt", QObject::tr("do not include the Qt library into the code"));
	options.alias("nocodeqt", "Q");
	options.add("execute", QObject::tr("execute the current script"));
	options.alias("execute", "e");
	options.add("exitatend", QObject::tr("close Actionaz after execution - requires execute"));
	options.alias("exitatend", "x");
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

	QString locale = settings.value("locale").toString();

	if(locale.isEmpty())
	{
		locale = QLocale::system().name();

#ifdef Q_WS_WIN
		QString installerLanguage = settings.value("installerLanguage").toString();
		if(!installerLanguage.isEmpty())
		{
			if(installerLanguage == "english")
				locale = "en_US";
			else if(installerLanguage == "french")
				locale = "fr_FR";
		}
#endif
	}

#ifdef Q_WS_WIN
	QTranslator qtTranslator;
	qtTranslator.load(QString("%1/locale/qt_%2").arg(QApplication::applicationDirPath()).arg(locale));
	app.installTranslator(&qtTranslator);
#else
	QTranslator qtTranslator;
	qtTranslator.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&qtTranslator);
#endif

	QTranslator toolsTranslator;
	toolsTranslator.load(QString("%1/locale/tools_%2").arg(QApplication::applicationDirPath()).arg(locale));
	app.installTranslator(&toolsTranslator);

	QTranslator actionToolsTranslator;
	actionToolsTranslator.load(QString("%1/locale/actiontools_%2").arg(QApplication::applicationDirPath()).arg(locale));
	app.installTranslator(&actionToolsTranslator);

	QTranslator executerTranslator;
	executerTranslator.load(QString("%1/locale/executer_%2").arg(QApplication::applicationDirPath()).arg(locale));
	app.installTranslator(&executerTranslator);

	QTranslator guiTranslator;
	guiTranslator.load(QString("%1/locale/gui_%2").arg(QApplication::applicationDirPath()).arg(locale));
	app.installTranslator(&guiTranslator);

	if(options.count("version"))
	{
		QTextStream stream(stdout);
		stream << "Actionaz version " << Global::ACTIONAZ_VERSION.toString() << ", script version " << Global::SCRIPT_VERSION.toString() << "\n";
		stream.flush();
		return 0;
	}
	if(options.count("help") || options.showUnrecognizedWarning() || (options.count("exitatend") && !options.count("execute")))
	{
		QTextStream stream(stdout);
		stream << QObject::tr("usage: ") << QCoreApplication::arguments().at(0) << " " << QObject::tr("[parameters]") << " " << QObject::tr("[filename]") << "\n";
		stream << QObject::tr("Parameters are:") << "\n";
		stream << options.getUsage();
		stream.flush();
		return -1;
	}

	QString startScript;
	const QStringList &positionalParameters = options.positional();
	if(positionalParameters.count() > 0)
		startScript = positionalParameters.at(0);

	QFileInfo fileInfo(QDir::current().filePath(startScript));
	if(app.sendMessage(fileInfo.filePath()))
		return 0;

#ifdef Q_WS_X11
	notify_init("Actionaz");
#endif

#ifdef Q_WS_WIN
	AllowSetForegroundWindow(ASFW_ANY);
#endif

	app.addLibraryPath(QApplication::applicationDirPath() + "/actions");
	app.addLibraryPath(QApplication::applicationDirPath() + "/plugins");

	if(!options.count("nocodeqt"))
		app.addLibraryPath(QApplication::applicationDirPath() + "/code");

	qRegisterMetaType<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaType<ActionTools::ActionException::Exception>("Exception");
	qRegisterMetaType<ActionTools::Parameter>("Parameter");
	qRegisterMetaType<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaType<ActionTools::ActionInstanceBuffer>("ActionInstanceBuffer");
	qRegisterMetaType<Tools::Version>("Version");

	qRegisterMetaTypeStreamOperators<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaTypeStreamOperators<ActionTools::Parameter>("Parameter");
	qRegisterMetaTypeStreamOperators<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaTypeStreamOperators<ActionTools::ActionInstanceBuffer>("ActionInstanceBuffer");
	qRegisterMetaTypeStreamOperators<Tools::Version>("Version");

#ifdef Q_WS_X11
	{
#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("Load key codes");
#endif
		ActionTools::KeySymHelper::loadKeyCodes();
	}
#endif

	ProgressSplashScreen *splash = 0;
	if(!options.count("nosplash") && !options.count("execute"))
	{
		splash = new ProgressSplashScreen(QPixmap(":/images/start.png"), Qt::WindowStaysOnTopHint);
		splash->setWindowOpacity(0.0f);
		splash->show();
		app.processEvents();

		float splashScreenOpacity = 0.0f;
		QElapsedTimer splashScreenFadeTime;

		splashScreenFadeTime.start();

		while(splashScreenOpacity < 1.0f)
		{
			splashScreenOpacity = static_cast<float>(splashScreenFadeTime.elapsed()) * 0.003f;

			if(splashScreenOpacity > 1.0f)
				splashScreenOpacity = 1.0f;

			splash->setWindowOpacity(splashScreenOpacity);
			splash->repaint();
		}
	}

	MainWindow mainWindow(&options, splash, startScript, locale);
	mainWindow.setWindowOpacity(0.0);

	QObject::connect(&app, SIGNAL(messageReceived(const QString &)), &mainWindow, SLOT(otherInstanceMessage(const QString &)));

	app.setActivationWindow(&mainWindow);

	QObject::connect(&mainWindow, SIGNAL(needToShow()), &app, SLOT(activateWindow()));

	if(!options.count("execute"))
		mainWindow.show();

	return app.exec();
}
