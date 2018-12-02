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

#include <QtGlobal>

#include "mainwindow.h"
#include "actioninstance.h"
#include "parameter.h"
#include "subparameter.h"
#include "actioninstancebuffer.h"
#include "global.h"
#include "version.h"
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
#include <QCommandLineParser>

#ifdef Q_OS_UNIX
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
#ifdef Q_OS_UNIX
	notify_uninit();
#endif
}

int main(int argc, char **argv)
{
#if (QT_VERSION < 0x050400)
        #error("You need Qt 5.4.0 or later to compile Actiona");
#endif

#ifdef ACT_PROFILE
	Tools::HighResolutionTimer timer("Application run");
#endif

	QtSingleApplication app(QStringLiteral("actiona-gui"), argc, argv);

	app.setQuitOnLastWindowClosed(false);

	app.setOrganizationName(QStringLiteral("Actiona"));
	app.setOrganizationDomain(QStringLiteral("actiona.tools"));
	app.setApplicationName(QStringLiteral("Actiona"));
	app.setApplicationVersion(Global::ACTIONA_VERSION.toString() + QStringLiteral(", script ") + Global::SCRIPT_VERSION.toString());

	qAddPostRoutine(cleanup);

	qsrand(static_cast<uint>(std::time(nullptr)));

	QCommandLineParser optionsParser;
	optionsParser.setApplicationDescription(QObject::tr("Emulates clics, key presses and other actions."));
	optionsParser.addHelpOption();
	optionsParser.addVersionOption();

	optionsParser.addOption({{QStringLiteral("p"), QStringLiteral("portable")}, QObject::tr("Starts in portable mode, storing the settings in the executable folder.")});

	optionsParser.parse(app.arguments());

	if(optionsParser.isSet(QStringLiteral("portable")))
	{
		QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QApplication::applicationDirPath() + QStringLiteral("/userSettings"));
		QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, QApplication::applicationDirPath() + QStringLiteral("/systemSettings"));
		QSettings::setDefaultFormat(QSettings::IniFormat);
	}

    QString locale = Tools::Languages::locale();

    Tools::Languages::installTranslator(QStringLiteral("qtbase"), locale);
    Tools::Languages::installTranslator(QStringLiteral("qtlocation"), locale);
    Tools::Languages::installTranslator(QStringLiteral("qtmultimedia"), locale);
    Tools::Languages::installTranslator(QStringLiteral("qtscript"), locale);
    Tools::Languages::installTranslator(QStringLiteral("qtxmlpatterns"), locale);
    Tools::Languages::installTranslator(QStringLiteral("tools"), locale);
    Tools::Languages::installTranslator(QStringLiteral("actiontools"), locale);
    Tools::Languages::installTranslator(QStringLiteral("executer"), locale);
    Tools::Languages::installTranslator(QStringLiteral("gui"), locale);

	optionsParser.addOptions(
	{
		{{QStringLiteral("s"), QStringLiteral("nosplash")}, QObject::tr("Disable the splash screen.")},
		{{QStringLiteral("t"), QStringLiteral("notrayicon")}, QObject::tr("Disable the tray icon.")},
		{{QStringLiteral("E"), QStringLiteral("noexecutionwindow")}, QObject::tr("Do not show the execution window.")},
		{{QStringLiteral("C"), QStringLiteral("noconsolewindow")}, QObject::tr("Do not show the console window.")},
		{{QStringLiteral("Q"), QStringLiteral("nocodeqt")}, QObject::tr("Do not include the Qt library into the code.")},
		{{QStringLiteral("e"), QStringLiteral("execute")}, QObject::tr("Execute the current script.")},
		{{QStringLiteral("x"), QStringLiteral("exitatend")}, QObject::tr("Close Actiona after execution - requires execute.")},
	});

	optionsParser.addPositionalArgument(QStringLiteral("script"), QObject::tr("The filepath of a script file to open/execute."), QStringLiteral("[script]"));

	optionsParser.process(app);

	if(optionsParser.isSet(QStringLiteral("exitatend")) && !optionsParser.isSet(QStringLiteral("execute")))
		optionsParser.showHelp(-1);

	QString startScript;
	const QStringList &positionalArguments = optionsParser.positionalArguments();
	if(positionalArguments.count() > 0)
	{
		startScript = positionalArguments.first();

		QFileInfo absoluteFileInfo(startScript);
		if(absoluteFileInfo.isFile() && absoluteFileInfo.isReadable())
		{
			if(app.sendMessage(absoluteFileInfo.absoluteFilePath()))
				return 0;
		}

		QFileInfo relativeFileInfo(QDir::current().filePath(startScript));
		if(relativeFileInfo.isFile() && relativeFileInfo.isReadable())
		{
			if(app.sendMessage(absoluteFileInfo.absoluteFilePath()))
				return 0;
		}
	 }

#ifdef Q_OS_UNIX
    notify_init("Actiona");
#endif

	app.addLibraryPath(QApplication::applicationDirPath() + QStringLiteral("/actions"));
	app.addLibraryPath(QApplication::applicationDirPath() + QStringLiteral("/plugins"));

	if(!optionsParser.isSet(QStringLiteral("nocodeqt")))
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

#ifdef Q_OS_UNIX
	{
#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("Load key codes");
#endif
		ActionTools::KeySymHelper::loadKeyCodes();
	}
#endif

	ProgressSplashScreen *splash = nullptr;
	if(!optionsParser.isSet(QStringLiteral("nosplash")) && !optionsParser.isSet(QStringLiteral("execute")))
	{
		splash = new ProgressSplashScreen(QPixmap(QStringLiteral(":/images/start.png")), Qt::WindowStaysOnTopHint);
		splash->setWindowOpacity(0);
		splash->show();
		app.processEvents();

		qreal splashScreenOpacity = 0;
		QElapsedTimer splashScreenFadeTime;

		splashScreenFadeTime.start();

		while(splashScreenOpacity < 1)
		{
			splashScreenOpacity = static_cast<qreal>(splashScreenFadeTime.elapsed()) * 0.003;

			if(splashScreenOpacity > 1)
				splashScreenOpacity = 1;

			splash->setWindowOpacity(splashScreenOpacity);
			splash->repaint();
		}
	}

	MainWindow mainWindow(optionsParser, splash, startScript, locale);

	QObject::connect(&app, &QtSingleApplication::messageReceived, &mainWindow, &MainWindow::otherInstanceMessage);

	app.setActivationWindow(&mainWindow);

	QObject::connect(&mainWindow, &MainWindow::needToShow, &app, &QtSingleApplication::activateWindow);

	if(!optionsParser.isSet(QStringLiteral("execute")))
		mainWindow.show();

	return app.exec();
}
