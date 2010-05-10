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

#include "pch.h"
#include "mainwindow.h"
#include "action.h"
#include "parameter.h"
#include "subparameter.h"
#include "actionbuffer.h"
#include "global.h"
#include "version.h"
#include "actioninfo.h"

#include <QxtApplication>
#include <QxtCommandOptions>

#ifdef QT_WS_WIN
#include <windows.h>
#endif

int main(int argc, char **argv)
{
	QxtApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);

	QxtCommandOptions options;
	options.setFlagStyle(QxtCommandOptions::DoubleDash);
	options.setScreenWidth(0);
	options.add("nosplash", "disable the splash screen");//TODO
	options.alias("nosplash", "s");
	options.add("notrayicon", "disable the tray icon");//TODO
	options.alias("notrayicon", "t");
	options.add("noexecutionwindow", "do not show the execution window");//TODO
	options.alias("noexecutionwindow", "E");
	options.add("noconsolewindow", "do not show the console window");//TODO
	options.alias("noconsolewindow", "C");
	options.add("execute", "execute the current script");//TODO
	options.alias("execute", "e");
	options.add("exitatend", "close Actionaz after execution");//TODO
	options.alias("exitatend", "x");
	options.add("help", "show this help test");
	options.alias("help", "h");
	options.parse(QCoreApplication::arguments());
	if(options.count("help") || options.showUnrecognizedWarning())
	{
	     options.showUsage();
	     return -1;
	}

	//QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

#ifdef QT_WS_WIN
	AllowSetForegroundWindow(ASFW_ANY);
#endif

#ifdef QT_WS_WIN
	// This is needed so that relative paths will work on Windows regardless of where the app is launched from.
	QDir::setCurrent(app.applicationDirPath());
#endif

	//QApplication::setStyle(new QCleanlooksStyle);

	app.addLibraryPath(QDir::currentPath() + "/actions");

	qRegisterMetaType<ActionTools::Action>("Action");
	qRegisterMetaType<ActionTools::Action::ExecutionException>("Action::ExecutionException");
	qRegisterMetaType<ActionTools::Parameter>("Parameter");
	qRegisterMetaType<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaType<ActionTools::ActionBuffer>("ActionBuffer");
	qRegisterMetaType<Tools::Version>("Tools::Version");
	qRegisterMetaType<Tools::ActionInfo>("Tools::ActionInfo");
	qRegisterMetaType<Tools::ActionPackInfo>("Tools::ActionPackInfo");
	qRegisterMetaType<Tools::ActionPackInfoList>("Tools::ActionPackInfoList");

	qRegisterMetaTypeStreamOperators<ActionTools::Action>("Action");
	qRegisterMetaTypeStreamOperators<ActionTools::Parameter>("Parameter");
	qRegisterMetaTypeStreamOperators<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaTypeStreamOperators<ActionTools::ActionBuffer>("ActionBuffer");
	qRegisterMetaTypeStreamOperators<Tools::Version>("Tools::Version");
	qRegisterMetaTypeStreamOperators<Tools::ActionInfo>("Tools::ActionInfo");
	qRegisterMetaTypeStreamOperators<Tools::ActionPackInfo>("Tools::ActionPackInfo");
	qRegisterMetaTypeStreamOperators<Tools::ActionPackInfoList>("Tools::ActionPackInfoList");

	app.setOrganizationName("Jmgr.info");
	app.setOrganizationDomain("jmgr.info");
	app.setApplicationName("Actionaz");
	app.setApplicationVersion(Global::ACTIONAZ_VERSION.toString());

	QSplashScreen *splash = new QSplashScreen(QPixmap(":/images/start.png"), Qt::WindowStaysOnTopHint);
	splash->show();
	app.processEvents();

	MainWindow mainWindow(splash);
	mainWindow.setWindowOpacity(0.0);
	mainWindow.show();

	try
	{
		return app.exec();
	}
	catch(...)
	{
		return -1;
	}
}
