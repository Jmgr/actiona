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

#include "backend/capabilities.hpp"

#include <QDebug>

#ifdef Q_OS_UNIX
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QX11Info>
#include <QXmlStreamReader>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif

namespace Backend
{
    Capabilities::Capabilities()
    {
        detectX11();
        detectXTest();
        detectDBus();
    }

    void Capabilities::detectX11()
    {
#ifdef Q_OS_UNIX
        auto display = QX11Info::display();

        if(display)
            mHasX11 = true;
#endif
    }

    void Capabilities::detectXTest()
    {
#ifdef Q_OS_UNIX
        if(!mHasX11)
            return;

        auto display = QX11Info::display();
        int unused;

        if(XTestQueryExtension(display, &unused, &unused, &unused, &unused))
            mHasXTest = true;
#endif
    }

    void Capabilities::detectDBus()
    {
#ifdef Q_OS_UNIX
        auto sessionConnection = QDBusConnection::sessionBus();
        if(!sessionConnection.isConnected())
            return;

        auto servicesReply = sessionConnection.interface()->registeredServiceNames();
        if(!servicesReply.isValid())
            return;

        mServices = servicesReply.value().toSet();

        if(hasDBusService(QStringLiteral("org.gnome.Shell")))
        {
            QDBusInterface gnomeShell(QStringLiteral("org.gnome.Shell"), QStringLiteral("/org/gnome/Shell/Screenshot"), QStringLiteral("org.freedesktop.DBus.Introspectable"));
            if(gnomeShell.isValid())
            {
                auto res = gnomeShell.call(QStringLiteral("Introspect"));
                if(res.type() != QDBusMessage::ErrorMessage && !res.arguments().isEmpty())
                {
                    //qDebug().noquote() << res.arguments().first().toString();
                    QXmlStreamReader stream(res.arguments().first().toByteArray());
                    while(!stream.atEnd() && !stream.hasError())
                    {
                        stream.readNext();

                        if(stream.isStartDocument())
                            continue;

                        if(!stream.isStartElement())
                            continue;

                        if(stream.name() == QLatin1String("method"))
                        {
                            auto attr = stream.attributes();
                            qDebug() << attr.value(QStringLiteral("name"));

                            stream.readNext();

                            for(;!stream.isEndElement() || stream.name() != QLatin1String("method");stream.readNext())
                            {
                                if(!stream.isStartElement())
                                    continue;

                                auto attr = stream.attributes();
                                qDebug() << attr.value(QStringLiteral("name"));
                            }
                        }
                    }

                    /*
                    while(!xml.atEnd())
                    {
                        xml.readNext();

                        if(xml.name() != QStringLiteral("method"))
                            continue;

                        auto attr = xml.attributes();

                        qDebug() << attr.value(QStringLiteral("name"));
                    }
                    */
                    if(stream.hasError())
                    {
                        qDebug() << "xml error" << stream.errorString();
                    }
                }
            }
        }

        //QDBusInterface interface(QStringLiteral("org.gnome.Shell"), QStringLiteral("/org/gnome/Shell/Screenshot"), QStringLiteral("org.gnome.Shell.Screenshot"));
#endif
    }
}
