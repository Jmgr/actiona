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

#include "globalshortcut/globalshortcutmanager.h"
#include "qxtcommandoptions/qxtcommandoptions.h"
#include "actioninstance.h"
#include "version.h"
#include "mainclass.h"
#include "qtsingleapplication/qtsingleapplication.h"
#include "global.h"
#include "settings.h"
#include "languages.h"

#ifdef ACT_PROFILE
#include "highresolutiontimer.h"
#endif

#include <ctime>

#include <QApplication>
#include <QtGlobal>
#include <QDir>
#include <QTextCodec>
#include <QFile>
#include <QSettings>
#include <QUrl>
#include <QNetworkProxy>
#include <QDataStream>
#include <QUrlQuery>

#ifdef Q_OS_LINUX
#undef signals
#include <libnotify/notify.h>
#define signals
#include "keysymhelper.h"
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#endif

static void cleanup()
{
	ActionTools::GlobalShortcutManager::clear();
}

#ifdef Q_OS_WIN
static void pause()
{
	system("pause");
}

static void createConsole()
{
	AllocConsole();

	HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int conHandle = _open_osfhandle(reinterpret_cast<intptr_t>(stdHandle), _O_TEXT);

	*stdout = *_fdopen(conHandle, "w");
	setvbuf(stdout, NULL, _IONBF, 0);

	stdHandle = GetStdHandle(STD_INPUT_HANDLE);
	conHandle = _open_osfhandle(reinterpret_cast<intptr_t>(stdHandle), _O_TEXT);

	*stdin = *_fdopen(conHandle, "r");
	setvbuf(stdin, NULL, _IONBF, 0);

	stdHandle = GetStdHandle(STD_ERROR_HANDLE);
	conHandle = _open_osfhandle(reinterpret_cast<intptr_t>(stdHandle), _O_TEXT);

	*stderr = *_fdopen(conHandle, "w");
	setvbuf(stderr, NULL, _IONBF, 0);
}

#endif

int main(int argc, char **argv)
{
#if (QT_VERSION < 0x050400)
		#error("You need Qt 5.4.0 or later to compile Actiona Executer");
#endif

	QtSingleApplication app(QStringLiteral("actiona-exec"), argc, argv);
	app.setQuitOnLastWindowClosed(false);

	qAddPostRoutine(cleanup);

	qsrand(std::time(NULL));

#ifdef Q_OS_LINUX
    notify_init("Actiona executer");
#endif

    QxtCommandOptions preOptions;

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
	Tools::installTranslator(QStringLiteral("actexecuter"), locale);

    const QStringList &arguments = QCoreApplication::arguments();

    QxtCommandOptions options;
    options.setFlagStyle(QxtCommandOptions::DoubleDash);
    options.setScreenWidth(0);
	options.add(QStringLiteral("code"), QObject::tr("switch to code mode, may not be used with -s"));
	options.alias(QStringLiteral("code"), QStringLiteral("c"));
	options.add(QStringLiteral("script"), QObject::tr("switch to script mode, may not be used with -c"));
	options.alias(QStringLiteral("script"), QStringLiteral("s"));
	options.add(QStringLiteral("nocodeqt"), QObject::tr("do not include the Qt library into the code"));
	options.alias(QStringLiteral("nocodeqt"), QStringLiteral("Q"));
	options.add(QStringLiteral("portable"), QObject::tr("starts in portable mode, storing the settings in the executable folder"));
	options.alias(QStringLiteral("portable"), QStringLiteral("p"));
	options.add(QStringLiteral("proxy-mode"), QObject::tr("sets the proxy mode, values are \"none\", \"system\" (default) or \"custom\""));
	options.add(QStringLiteral("proxy-type"), QObject::tr("sets the custom proxy type, values are \"http\" or \"socks\" (default)"));
	options.add(QStringLiteral("proxy-host"), QObject::tr("sets the custom proxy host"));
	options.add(QStringLiteral("proxy-port"), QObject::tr("sets the custom proxy port"));
	options.add(QStringLiteral("proxy-user"), QObject::tr("sets the custom proxy user"));
	options.add(QStringLiteral("proxy-password"), QObject::tr("sets the custom proxy password"));
#ifdef Q_OS_WIN
	options.add(QStringLiteral("console"), QObject::tr("create a console to see debug output"));
	options.add(QStringLiteral("pause-at-end"), QObject::tr("wait for user input at the end of the execution, used only with --console"));
#endif
	options.add(QStringLiteral("version"), QObject::tr("show the program version"));
	options.alias(QStringLiteral("version"), QStringLiteral("v"));
	options.add(QStringLiteral("help"), QObject::tr("show this help text"));
	options.alias(QStringLiteral("help"), QStringLiteral("h"));
    options.parse(arguments);

#ifdef Q_OS_WIN
	if(options.count(QStringLiteral("console")))
    {
        createConsole();

		if(options.count(QStringLiteral("pause-at-end")))
            qAddPostRoutine(pause);
    }
#endif

	qRegisterMetaType<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaType<ActionTools::ActionException::Exception>("Exception");
	qRegisterMetaType<ActionTools::Parameter>("Parameter");
	qRegisterMetaType<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaType<Tools::Version>("Version");

	qRegisterMetaTypeStreamOperators<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaTypeStreamOperators<ActionTools::Parameter>("Parameter");
	qRegisterMetaTypeStreamOperators<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaTypeStreamOperators<Tools::Version>("Version");

	if(options.count(QStringLiteral("version")))
	{
		QTextStream stream(stdout);
        stream << "Actiona Executer version " << Global::ACTIONA_VERSION.toString() << ", script version " << Global::SCRIPT_VERSION.toString() << "\n";
		stream.flush();
		return 0;
	}
	if(options.count(QStringLiteral("help")) || options.showUnrecognizedWarning() || options.positional().count() < 1 || (options.count(QStringLiteral("code")) && options.count(QStringLiteral("script"))))
	{
		QTextStream stream(stdout);
		stream << QObject::tr("usage: ") << QCoreApplication::arguments().at(0) << " " << QObject::tr("[parameters]") << " " << QObject::tr("filename") << "\n";
		stream << QObject::tr("Parameters are:") << "\n";
		stream << options.getUsage();
		stream.flush();
		return -1;
	}

	app.addLibraryPath(QApplication::applicationDirPath() + QStringLiteral("/actions"));
	app.addLibraryPath(QApplication::applicationDirPath() + QStringLiteral("/plugins"));

	if(!options.count(QStringLiteral("nocodeqt")))
		app.addLibraryPath(QApplication::applicationDirPath() + QStringLiteral("/code"));

#ifdef Q_OS_LINUX
	{
#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("Load key codes");
#endif
		ActionTools::KeySymHelper::loadKeyCodes();
	}
#endif

	// Proxy settings
	int proxyMode = ActionTools::Settings::PROXY_SYSTEM;
	if(options.value(QStringLiteral("proxy-mode")).toString() == QLatin1String("none"))
		proxyMode = ActionTools::Settings::PROXY_NONE;
	else if(options.value(QStringLiteral("proxy-mode")).toString() == QLatin1String("custom"))
		proxyMode = ActionTools::Settings::PROXY_CUSTOM;
	else if(options.value(QStringLiteral("proxy-mode")).toString() == QLatin1String("system"))
		proxyMode = ActionTools::Settings::PROXY_SYSTEM;
	else if(!options.value(QStringLiteral("proxy-mode")).toString().isEmpty())
	{
		QTextStream stream(stdout);
		stream << QObject::tr("Unknown proxy mode, values are \"none\", \"system\" (default) or \"custom\"") << "\n";
		stream.flush();
		return -1;
	}

	QNetworkProxy proxy;

	switch(proxyMode)
	{
	case ActionTools::Settings::PROXY_NONE:
		proxy.setType(QNetworkProxy::NoProxy);
		break;
	case ActionTools::Settings::PROXY_SYSTEM:
		{
			QUrl url(Global::CONNECTIVITY_URL);
			QNetworkProxyQuery networkProxyQuery(url);
			QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery(networkProxyQuery);
			if(!listOfProxies.isEmpty())
				proxy = listOfProxies.first();
			else
				proxy.setType(QNetworkProxy::NoProxy);
		}
		break;
	case ActionTools::Settings::PROXY_CUSTOM:
		{
			int type = ActionTools::Settings::PROXY_TYPE_SOCKS5;
			if(options.value(QStringLiteral("proxy-type")).toString() == QLatin1String("http"))
				type = ActionTools::Settings::PROXY_TYPE_HTTP;
			else if(options.value(QStringLiteral("proxy-type")).toString() == QLatin1String("socks"))
				type = ActionTools::Settings::PROXY_TYPE_SOCKS5;
			else if(!options.value(QStringLiteral("proxy-type")).toString().isEmpty())
			{
				QTextStream stream(stdout);
				stream << QObject::tr("Unknown proxy type, values are \"http\" or \"socks\" (default)") << "\n";
				stream.flush();
				return -1;
			}

			QNetworkProxy proxy;

			if(type == ActionTools::Settings::PROXY_TYPE_HTTP)
				proxy.setType(QNetworkProxy::HttpProxy);
			else
				proxy.setType(QNetworkProxy::Socks5Proxy);

			proxy.setHostName(options.value(QStringLiteral("proxy-host")).toString());
			proxy.setPort(options.value(QStringLiteral("proxy-port")).toInt());
			proxy.setUser(options.value(QStringLiteral("proxy-user")).toString());
			proxy.setPassword(options.value(QStringLiteral("proxy-password")).toString());
		}
		break;
	}

	QNetworkProxy::setApplicationProxy(proxy);

	QUrl protocolUrl = QUrl::fromEncoded(arguments.at(1).toUtf8());
	if(protocolUrl.isValid() && protocolUrl.scheme() != QLatin1String("actiona"))
		protocolUrl = QUrl();

	MainClass::ExecutionMode executionMode = MainClass::Unknown;
	MainClass mainClass;

	if(protocolUrl.isValid())
	{
		QString mode;
        using QStringPair = QPair<QString, QString>;
        for(const QStringPair &queryItem: QUrlQuery(protocolUrl.query()).queryItems())
		{
			if(queryItem.first == QLatin1String("mode"))
			{
				mode = queryItem.second;
				break;
			}
		}

		if(mode == QLatin1String("code"))
			executionMode = MainClass::Code;
		else if(mode == QLatin1String("script"))
			executionMode = MainClass::Script;
		else
		{
			if(protocolUrl.path().endsWith(QStringLiteral(".ascr")))
				executionMode = MainClass::Script;
			else if(protocolUrl.path().endsWith(QStringLiteral(".acod")))
				executionMode = MainClass::Code;
			else
			{
				QTextStream stream(stdout);
				stream << QObject::tr("Unknown execution mode, please specify mode=script or mode=code") << "\n";
				stream.flush();
				return -1;
			}
		}

		if(!mainClass.start(executionMode, protocolUrl))
			return -1;
	}
	else
	{
		QString filename = options.positional().at(0);

		if(options.count(QStringLiteral("code")))
			executionMode = MainClass::Code;
		else if(options.count(QStringLiteral("script")))
			executionMode = MainClass::Script;
		else
		{
			if(filename.endsWith(QStringLiteral(".ascr")))
				executionMode = MainClass::Script;
			else if(filename.endsWith(QStringLiteral(".acod")))
				executionMode = MainClass::Code;
			else
			{
				QTextStream stream(stdout);
				stream << QObject::tr("Unknown execution mode, please specify -s (script) or -c (code)") << "\n";
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

		if(!mainClass.start(executionMode, &file, file.fileName()))
		{
			file.close();

			return -1;
		}
	}

	return app.exec();
}
