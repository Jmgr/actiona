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

#include "actiontools/actioninstance.hpp"
#include "mainclass.hpp"
#include "actiontools/qtsingleapplication/QtSingleApplication"
#include "global.hpp"
#include "actiontools/settings.hpp"
#include "tools/languages.hpp"

#ifdef ACT_PROFILE
#include "tools/highresolutiontimer.hpp"
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
#include <QMessageBox>

#undef signals
#include <libnotify/notify.h>
#define signals
#include "actiontools/keysymhelper.hpp"
#include "actiontools/x11info.hpp"
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
#endif

int main(int argc, char **argv)
{
    QtSingleApplication app(QStringLiteral("actiona-exec"), argc, argv);

#ifdef Q_OS_UNIX
    if(!ActionTools::X11Info::display())
    {
        qCritical() << QObject::tr("X11 was not detected. Actiona only functions in X11 sessions (Wayland is not supported). Please refer to https://wiki.actiona.tools/doku.php?id=en:x11notdetected for more information.");

        QMessageBox::critical(nullptr,
                              QObject::tr("X11 not detected"),
                              QObject::tr("X11 was not detected. Actiona only functions in X11 sessions (Wayland is not supported). Please refer to <a href=\"https://wiki.actiona.tools/doku.php?id=en:x11notdetected\">this wiki page</a> for more information."));
        return 0;
    }
#endif

    app.setQuitOnLastWindowClosed(false);

	qAddPostRoutine(cleanup);

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
        AllocConsole();

        // Redirect standard in/out streams
        FILE* dummy;
        freopen_s(&dummy, "CONOUT$", "w", stdout);
        freopen_s(&dummy, "CONOUT$", "w", stderr);
        freopen_s(&dummy, "CONIN$", "r", stdin);

        if(optionsParser.isSet(QStringLiteral("pause-at-end")))
            qAddPostRoutine(pause);
    }
#endif

	qRegisterMetaType<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaType<ActionTools::ActionException::Exception>("Exception");
	qRegisterMetaType<ActionTools::Parameter>("Parameter");
	qRegisterMetaType<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaType<QVersionNumber>("Version");

	const auto &positionalArguments = optionsParser.positionalArguments();
	if(positionalArguments.count() < 1 || (optionsParser.isSet(QStringLiteral("code")) && optionsParser.isSet(QStringLiteral("script"))))
		optionsParser.showHelp(-1);

#ifdef Q_OS_UNIX
	{
#ifdef ACT_PROFILE
        Tools::HighResolutionTimer timer(QStringLiteral("Load key codes"));
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
        const auto queryItems = QUrlQuery(protocolUrl.query()).queryItems();
        for(const auto &queryItem: queryItems)
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
