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
#include <QCommandLineParser>

#ifdef Q_OS_UNIX
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
#ifdef Q_OS_UNIX
	notify_uninit();
#endif
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

	qsrand(static_cast<uint>(std::time(nullptr)));

#ifdef Q_OS_UNIX
    notify_init("Actiona executer");
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
    Tools::Languages::installTranslator(QStringLiteral("actexecuter"), locale);

	optionsParser.addOptions(
	{
		{{QStringLiteral("c"), QStringLiteral("code")}, QObject::tr("Switch to code mode, may not be used with -s.")},
		{{QStringLiteral("s"), QStringLiteral("script")}, QObject::tr("Switch to script mode, may not be used with -c.")},
		{{QStringLiteral("Q"), QStringLiteral("nocodeqt")}, QObject::tr("Do not include the Qt library into the code.")},
		{{QStringLiteral("proxy-mode")}, QObject::tr(R"(Sets the proxy mode, values are "none", "system" (default) or "custom".)")},
		{{QStringLiteral("proxy-type")}, QObject::tr(R"(Sets the custom proxy type, values are "http" or "socks" (default).)")},
		{{QStringLiteral("proxy-host")}, QObject::tr("Sets the custom proxy host.")},
		{{QStringLiteral("proxy-port")}, QObject::tr("Sets the custom proxy port.")},
		{{QStringLiteral("proxy-user")}, QObject::tr("Sets the custom proxy user.")},
		{{QStringLiteral("proxy-password")}, QObject::tr("Sets the custom proxy password.")},
	});

#ifdef Q_OS_WIN
	optionsParser.addOption({QStringLiteral("console"), QObject::tr("create a console to see debug output.")});
	optionsParser.addOption({QStringLiteral("pause-at-end"), QObject::tr("wait for user input at the end of the execution, used only with --console.")});
#endif

	optionsParser.addPositionalArgument(QStringLiteral("filepath"), QObject::tr("The filepath of a script/code file to execute."));

	optionsParser.process(app);

#ifdef Q_OS_WIN
	if(optionsParser.isSet(QStringLiteral("console")))
    {
        createConsole();

		if(optionsParser.isSet(QStringLiteral("pause-at-end")))
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

	const auto &positionalArguments = optionsParser.positionalArguments();
	if(positionalArguments.count() < 1 || (optionsParser.isSet(QStringLiteral("code")) && optionsParser.isSet(QStringLiteral("script"))))
		optionsParser.showHelp(-1);

	app.addLibraryPath(QApplication::applicationDirPath() + QStringLiteral("/actions"));
	app.addLibraryPath(QApplication::applicationDirPath() + QStringLiteral("/plugins"));

	if(!optionsParser.isSet(QStringLiteral("nocodeqt")))
		app.addLibraryPath(QApplication::applicationDirPath() + QStringLiteral("/code"));

#ifdef Q_OS_UNIX
	{
#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("Load key codes");
#endif
		ActionTools::KeySymHelper::loadKeyCodes();
	}
#endif

	// Proxy settings
	int proxyMode = ActionTools::Settings::PROXY_SYSTEM;
	if(optionsParser.value(QStringLiteral("proxy-mode")) == QLatin1String("none"))
		proxyMode = ActionTools::Settings::PROXY_NONE;
	else if(optionsParser.value(QStringLiteral("proxy-mode")) == QLatin1String("custom"))
		proxyMode = ActionTools::Settings::PROXY_CUSTOM;
	else if(optionsParser.value(QStringLiteral("proxy-mode")) == QLatin1String("system"))
		proxyMode = ActionTools::Settings::PROXY_SYSTEM;
	else if(!optionsParser.value(QStringLiteral("proxy-mode")).isEmpty())
	{
		QTextStream stream(stdout);
		stream << QObject::tr(R"(Unknown proxy mode, values are "none", "system" (default) or "custom")") << "\n";
		stream.flush();

		optionsParser.showHelp(-1);
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
			if(optionsParser.value(QStringLiteral("proxy-type")) == QLatin1String("http"))
				type = ActionTools::Settings::PROXY_TYPE_HTTP;
			else if(optionsParser.value(QStringLiteral("proxy-type")) == QLatin1String("socks"))
				type = ActionTools::Settings::PROXY_TYPE_SOCKS5;
			else if(!optionsParser.value(QStringLiteral("proxy-type")).isEmpty())
			{
				QTextStream stream(stdout);
				stream << QObject::tr(R"(Unknown proxy type, values are "http" or "socks" (default))") << "\n";
				stream.flush();

				optionsParser.showHelp(-1);
			}

			QNetworkProxy proxy;

			if(type == ActionTools::Settings::PROXY_TYPE_HTTP)
				proxy.setType(QNetworkProxy::HttpProxy);
			else
				proxy.setType(QNetworkProxy::Socks5Proxy);

			proxy.setHostName(optionsParser.value(QStringLiteral("proxy-host")));
			proxy.setPort(static_cast<quint16>(optionsParser.value(QStringLiteral("proxy-port")).toUInt()));
			proxy.setUser(optionsParser.value(QStringLiteral("proxy-user")));
			proxy.setPassword(optionsParser.value(QStringLiteral("proxy-password")));
		}
		break;
	}

	QNetworkProxy::setApplicationProxy(proxy);

	QUrl protocolUrl = QUrl::fromEncoded(positionalArguments.first().toUtf8());
	if(protocolUrl.isValid() && protocolUrl.scheme() != QLatin1String("actiona"))
		protocolUrl = QUrl();

	MainClass::ExecutionMode executionMode = MainClass::Unknown;
	MainClass mainClass;

	if(protocolUrl.isValid())
	{
        QString mode;
        for(const auto &queryItem: QUrlQuery(protocolUrl.query()).queryItems())
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

				optionsParser.showHelp(-1);
			}
		}

		if(!mainClass.start(executionMode, protocolUrl))
			return -1;
	}
	else
	{
		QString filename = positionalArguments.first();

		if(optionsParser.isSet(QStringLiteral("code")))
			executionMode = MainClass::Code;
		else if(optionsParser.isSet(QStringLiteral("script")))
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

				optionsParser.showHelp(-1);
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
