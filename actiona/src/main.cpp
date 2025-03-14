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

#include <QtGlobal>

#include "mainwindow.hpp"
#include "actiontools/actioninstance.hpp"
#include "actiontools/parameter.hpp"
#include "actiontools/subparameter.hpp"
#include "actiontools/actioninstancebuffer.hpp"
#include "global.hpp"
#include "progresssplashscreen.hpp"
#include "tools/languages.hpp"
#include "actiontools/qtsingleapplication/QtSingleApplication"
#include "themeselection.hpp"

#ifdef Q_OS_WIN
#include "client/windows/handler/exception_handler.h"
#include <QStandardPaths>
#endif

#ifdef ACT_PROFILE
#include "tools/highresolutiontimer.hpp"
#endif

#include <ctime>

#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QTextCodec>
#include <QElapsedTimer>
#include <QSettings>
#include <QCommandLineParser>
#include <QSettings>

#ifdef Q_OS_UNIX
#include <QMessageBox>

#undef signals
#include <libnotify/notify.h>
#define signals

#include "actiontools/keysymhelper.hpp"
#include "actiontools/x11info.hpp"
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
#ifdef ACT_PROFILE
    Tools::HighResolutionTimer timer(QStringLiteral("Application run"));
#endif

	QtSingleApplication app(QStringLiteral("actiona-gui"), argc, argv);

#ifdef Q_OS_UNIX
    if(!ActionTools::X11Info::display())
    {
        qCritical() << QObject::tr("X11 was not detected. Actiona only works in X11 sessions (Wayland is not supported). Please refer to https://wiki.actiona.tools/doku.php?id=en:x11notdetected for more information.");

        QMessageBox::critical(nullptr,
                              QObject::tr("X11 not detected"),
                              QObject::tr("X11 was not detected. Actiona only works in X11 sessions (Wayland is not supported). Please refer to <a href=\"https://wiki.actiona.tools/doku.php?id=en:x11notdetected\">this wiki page</a> for more information."));
        return 0;
    }
#endif

	app.setQuitOnLastWindowClosed(false);

	app.setOrganizationName(QStringLiteral("Actiona"));
	app.setOrganizationDomain(QStringLiteral("actiona.tools"));
	app.setApplicationName(QStringLiteral("Actiona"));
	app.setApplicationVersion(Global::ACTIONA_VERSION.toString() + QStringLiteral(", script ") + Global::SCRIPT_VERSION.toString());

	qAddPostRoutine(cleanup);

#ifdef Q_OS_WIN
    QString dumpPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    dumpPath = QDir(dumpPath).filePath(QStringLiteral("CrashDumps"));
    if (!QDir(dumpPath).exists()) {
        QDir().mkpath(dumpPath);
    }

    auto buffer = std::make_unique<wchar_t[]>(dumpPath.length() + 1);
    int charsCopied = dumpPath.toWCharArray(buffer.get());
    buffer[charsCopied] = L'\0';

    google_breakpad::ExceptionHandler eh(buffer.get(), nullptr, nullptr, nullptr, -1);
#endif

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
    Tools::Languages::installTranslator(QStringLiteral("actiona"), locale);

	optionsParser.addOptions(
	{
		{{QStringLiteral("s"), QStringLiteral("nosplash")}, QObject::tr("Disable the splash screen.")},
		{{QStringLiteral("t"), QStringLiteral("notrayicon")}, QObject::tr("Disable the tray icon.")},
		{{QStringLiteral("E"), QStringLiteral("noexecutionwindow")}, QObject::tr("Do not show the execution window.")},
		{{QStringLiteral("C"), QStringLiteral("noconsolewindow")}, QObject::tr("Do not show the console window.")},
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

    //TODO: Move this in the constructor of these classes
    qRegisterMetaType<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaType<ActionTools::ActionException::Exception>("Exception");
	qRegisterMetaType<ActionTools::Parameter>("Parameter");
	qRegisterMetaType<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaType<ActionTools::ActionInstanceBuffer>("ActionInstanceBuffer");
	qRegisterMetaType<QVersionNumber>("Version");

#ifdef Q_OS_UNIX
	{
#ifdef ACT_PROFILE
        Tools::HighResolutionTimer timer(QStringLiteral("Load key codes"));
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

    QSettings settings;
    auto theme = static_cast<ThemeSelection::Theme>(settings.value(QStringLiteral("gui/theme"), QVariant(static_cast<int>(ThemeSelection::Theme::Default))).toInt());
    ThemeSelection::selectTheme(theme);

	MainWindow mainWindow(optionsParser, splash, startScript, locale);

	QObject::connect(&app, &QtSingleApplication::messageReceived, &mainWindow, &MainWindow::otherInstanceMessage);

	app.setActivationWindow(&mainWindow);

	QObject::connect(&mainWindow, &MainWindow::needToShow, &app, &QtSingleApplication::activateWindow);

	if(!optionsParser.isSet(QStringLiteral("execute")))
		mainWindow.show();

	return app.exec();
}
