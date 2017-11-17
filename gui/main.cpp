/*
    Actiona
    Copyright (C) 2005-2017 Jonathan Mercier-Ganady

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
#include "languages.h"
#include "qtsingleapplication/qtsingleapplication.h"

#ifdef ACT_PROFILE
#include "highresolutiontimer.h"
#endif

#include <ctime>

#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QTextCodec>
#include <QElapsedTimer>
#include <QSettings>

#ifdef Q_OS_LINUX
#undef signals
#include <libnotify/notify.h>
#define signals

#include "xdisplayhelper.h"
#include "keysymhelper.h"
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

static void cleanup()
{
#ifdef Q_OS_WIN
	CoUninitialize();
#endif

	ActionTools::GlobalShortcutManager::clear();
}

int main(int argc, char **argv)
{
#if (QT_VERSION < 0x050400)
		#error("You need Qt 5.4.0 or later to compile Actiona");
#endif

#ifdef ACT_PROFILE
	Tools::HighResolutionTimer timer("Application run");
#endif

#ifdef Q_OS_WIN
	CoInitializeEx(0, COINIT_APARTMENTTHREADED | COINIT_SPEED_OVER_MEMORY);
#endif

	QtSingleApplication app(QStringLiteral("actiona-gui"), argc, argv);

	app.setQuitOnLastWindowClosed(false);

	app.setOrganizationName(QStringLiteral("Actiona"));
	app.setOrganizationDomain(QStringLiteral("actiona.tools"));
	app.setApplicationName(QStringLiteral("Actiona"));
    app.setApplicationVersion(Global::ACTIONA_VERSION.toString());

	qAddPostRoutine(cleanup);

	qsrand(std::time(NULL));

	QxtCommandOptions preOptions;

    preOptions.setFlagStyle(QxtCommandOptions::DoubleDash);
	preOptions.add(QStringLiteral("portable"), QObject::tr("starts in portable mode, storing the settings in the executable folder"));
	preOptions.alias(QStringLiteral("portable"), QStringLiteral("p"));
	preOptions.parse(QCoreApplication::arguments());

	if(preOptions.count(QStringLiteral("portable")) > 0)
	{
		QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QApplication::applicationDirPath() + QStringLiteral("/userSettings"));
		QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, QApplication::applicationDirPath() + QStringLiteral("/systemSettings"));
		QSettings::setDefaultFormat(QSettings::IniFormat);
	}

    QString locale = Tools::locale();

    Tools::installQtTranslator(locale);
	Tools::installTranslator(QStringLiteral("tools"), locale);
	Tools::installTranslator(QStringLiteral("actiontools"), locale);
	Tools::installTranslator(QStringLiteral("executer"), locale);
	Tools::installTranslator(QStringLiteral("gui"), locale);

	QxtCommandOptions options;

	options.setFlagStyle(QxtCommandOptions::DoubleDash);
	options.setScreenWidth(0);
	options.add(QStringLiteral("nosplash"), QObject::tr("disable the splash screen"));
	options.alias(QStringLiteral("nosplash"), QStringLiteral("s"));
	options.add(QStringLiteral("notrayicon"), QObject::tr("disable the tray icon"));
	options.alias(QStringLiteral("notrayicon"), QStringLiteral("t"));
	options.add(QStringLiteral("noexecutionwindow"), QObject::tr("do not show the execution window"));
	options.alias(QStringLiteral("noexecutionwindow"), QStringLiteral("E"));
	options.add(QStringLiteral("noconsolewindow"), QObject::tr("do not show the console window"));
	options.alias(QStringLiteral("noconsolewindow"), QStringLiteral("C"));
	options.add(QStringLiteral("nocodeqt"), QObject::tr("do not include the Qt library into the code"));
	options.alias(QStringLiteral("nocodeqt"), QStringLiteral("Q"));
	options.add(QStringLiteral("execute"), QObject::tr("execute the current script"));
	options.alias(QStringLiteral("execute"), QStringLiteral("e"));
	options.add(QStringLiteral("exitatend"), QObject::tr("close Actiona after execution - requires execute"));
	options.alias(QStringLiteral("exitatend"), QStringLiteral("x"));
	options.add(QStringLiteral("portable"), QObject::tr("starts in portable mode, storing the settings in the executable folder"));
	options.alias(QStringLiteral("portable"), QStringLiteral("p"));
	options.add(QStringLiteral("version"), QObject::tr("show the program version"));
	options.alias(QStringLiteral("version"), QStringLiteral("v"));
	options.add(QStringLiteral("help"), QObject::tr("show this help text"));
	options.alias(QStringLiteral("help"), QStringLiteral("h"));
	options.parse(QCoreApplication::arguments());

	if(options.count(QStringLiteral("version")))
	{
		QTextStream stream(stdout);
        stream << "Actiona version " << Global::ACTIONA_VERSION.toString() << ", script version " << Global::SCRIPT_VERSION.toString() << "\n";
		stream.flush();
		return 0;
	}
	if(options.count(QStringLiteral("help")) || options.showUnrecognizedWarning() || (options.count(QStringLiteral("exitatend")) && !options.count(QStringLiteral("execute"))))
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

#ifdef Q_OS_LINUX
    notify_init("Actiona");
#endif

#ifdef Q_OS_WIN
	AllowSetForegroundWindow(ASFW_ANY);
#endif

	app.addLibraryPath(QApplication::applicationDirPath() + QStringLiteral("/actions"));
	app.addLibraryPath(QApplication::applicationDirPath() + QStringLiteral("/plugins"));

	if(!options.count(QStringLiteral("nocodeqt")))
		app.addLibraryPath(QApplication::applicationDirPath() + QStringLiteral("/code"));

    //TODO: Move this in the constructor of these classes
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

#ifdef Q_OS_LINUX
	{
#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("Load key codes");
#endif
		ActionTools::KeySymHelper::loadKeyCodes();
	}
#endif

	ProgressSplashScreen *splash = 0;
	if(!options.count(QStringLiteral("nosplash")) && !options.count(QStringLiteral("execute")))
	{
		splash = new ProgressSplashScreen(QPixmap(QStringLiteral(":/images/start.png")), Qt::WindowStaysOnTopHint);
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

	QObject::connect(&app, SIGNAL(messageReceived(const QString &)), &mainWindow, SLOT(otherInstanceMessage(const QString &)));

	app.setActivationWindow(&mainWindow);

	QObject::connect(&mainWindow, SIGNAL(needToShow()), &app, SLOT(activateWindow()));

	if(!options.count(QStringLiteral("execute")))
		mainWindow.show();

	return app.exec();
}
