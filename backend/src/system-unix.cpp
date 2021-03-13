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

#include "backend/system-unix.hpp"
#include "backend/backend.hpp"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QList>
#include <QVariant>

#include <vector>

namespace Backend
{
    struct Service
    {
        QString name;
        QString path;
        QString interface;
    };
    struct Endpoint
    {
        Service service;
        QDBusConnection connection;
        QString function;
        std::function<QList<QVariant>(bool force)> boolToArgs;
    };

    const Service gnomeSessionManager
    {
        QStringLiteral("org.gnome.SessionManager"),
        QStringLiteral("/org/gnome/SessionManager"),
        QStringLiteral("org.gnome.SessionManager"),
    };
    const Service ksmServer
    {
        QStringLiteral("org.kde.ksmserver"),
        QStringLiteral("/KSMServer"),
        QStringLiteral("org.kde.KSMServerInterface"),
    };
    const Service logind
    {
        QStringLiteral("org.freedesktop.login1"),
        QStringLiteral("/org/freedesktop/login1"),
        QStringLiteral("org.freedesktop.login1.Manager"),
    };
    const Service consoleKit
    {
        QStringLiteral("org.freedesktop.ConsoleKit"),
        QStringLiteral("/org/freedesktop/ConsoleKit/Manager"),
        QStringLiteral("org.freedesktop.ConsoleKit.Manager"),
    };
    const Service uPower
    {
        QStringLiteral("org.freedesktop.UPower"),
        QStringLiteral("/org/freedesktop/UPower"),
        QStringLiteral("org.freedesktop.UPower"),
    };
    const Service deviceKitPower
    {
        QStringLiteral("org.freedesktop.DeviceKit.Power"),
        QStringLiteral("/org/freedesktop/DeviceKit/Power"),
        QStringLiteral("org.freedesktop.DeviceKit.Power"),
    };
    const Service gnomeScreenSaver
    {
        QStringLiteral("org.gnome.ScreenSaver"),
        QStringLiteral("/ScreenSaver"),
        QStringLiteral("org.gnome.ScreenSaver"),
    };
    const Service freedesktopScreenSaver
    {
        QStringLiteral("org.freedesktop.ScreenSaver"),
        QStringLiteral("/ScreenSaver"),
        QStringLiteral("org.freedesktop.ScreenSaver"),
    };
    const Service kdeScreenSaver
    {
        QStringLiteral("org.kde.screensaver"),
        QStringLiteral("/ScreenSaver"),
        QStringLiteral("org.freedesktop.ScreenSaver"),
    };

    const std::vector<Endpoint> logoutEndpoints
    {
        {
            gnomeSessionManager,
            QDBusConnection::sessionBus(),
            QStringLiteral("Logout"),
            [](bool force){ return QList<QVariant>{static_cast<unsigned int>(force ? 2 : 1)}; }
        },
        {
            ksmServer,
            QDBusConnection::sessionBus(),
            QStringLiteral("logout"),
            [](bool force){ return QList<QVariant>{0, 3, force ? 2 : 1}; }
        }
    };
    const std::vector<Endpoint> restartEndpoints
    {
        {
            gnomeSessionManager,
            QDBusConnection::sessionBus(),
            QStringLiteral("RequestReboot"),
            [](bool){ return QList<QVariant>{}; }
        },
        {
            ksmServer,
            QDBusConnection::sessionBus(),
            QStringLiteral("logout"),
            [](bool force){ return QList<QVariant>{0, 1, force ? 2 : 1}; }
        },
        {
            logind,
            QDBusConnection::systemBus(),
            QStringLiteral("Reboot"),
            [](bool force){ return QList<QVariant>{!force}; }
        },
        {
            consoleKit,
            QDBusConnection::systemBus(),
            QStringLiteral("Restart"),
            [](bool force){ return QList<QVariant>{!force}; }
        }
    };
    const std::vector<Endpoint> shutdownEndpoints
    {
        {
            gnomeSessionManager,
            QDBusConnection::sessionBus(),
            QStringLiteral("RequestShutdown"),
            [](bool){ return QList<QVariant>{}; }
        },
        {
            ksmServer,
            QDBusConnection::sessionBus(),
            QStringLiteral("logout"),
            [](bool force){ return QList<QVariant>{0, 2, force ? 2 : 1}; }
        },
        {
            logind,
            QDBusConnection::systemBus(),
            QStringLiteral("PowerOff"),
            [](bool force){ return QList<QVariant>{!force}; }
        },
        {
            consoleKit,
            QDBusConnection::systemBus(),
            QStringLiteral("Stop"),
            [](bool force){ return QList<QVariant>{!force}; }
        }
    };
    const std::vector<Endpoint> suspendEndpoints
    {
        {
            uPower,
            QDBusConnection::systemBus(),
            QStringLiteral("Suspend"),
            [](bool){ return QList<QVariant>{}; }
        },
        {
            deviceKitPower,
            QDBusConnection::systemBus(),
            QStringLiteral("Suspend"),
            [](bool){ return QList<QVariant>{}; }
        }
    };
    const std::vector<Endpoint> hibernateEndpoints
    {
        {
            uPower,
            QDBusConnection::systemBus(),
            QStringLiteral("Hibernate"),
            [](bool){ return QList<QVariant>{}; }
        },
        {
            deviceKitPower,
            QDBusConnection::systemBus(),
            QStringLiteral("Hibernate"),
            [](bool){ return QList<QVariant>{}; }
        }
    };
    const std::vector<Endpoint> lockScreenEndpoints
    {
        {
            gnomeScreenSaver,
            QDBusConnection::sessionBus(),
            QStringLiteral("Lock"),
            [](bool){ return QList<QVariant>{}; }
        },
        {
            freedesktopScreenSaver,
            QDBusConnection::sessionBus(),
            QStringLiteral("Lock"),
            [](bool){ return QList<QVariant>{}; }
        },
        {
            kdeScreenSaver,
            QDBusConnection::sessionBus(),
            QStringLiteral("Lock"),
            [](bool){ return QList<QVariant>{}; }
        }
    };
    const std::vector<Endpoint> startScreenSaverEndpoints
    {
        {
            gnomeScreenSaver,
            QDBusConnection::sessionBus(),
            QStringLiteral("SetActive"),
            [](bool){ return QList<QVariant>{true}; }
        },
        {
            freedesktopScreenSaver,
            QDBusConnection::sessionBus(),
            QStringLiteral("SetActive"),
            [](bool){ return QList<QVariant>{true}; }
        },
        {
            kdeScreenSaver,
            QDBusConnection::sessionBus(),
            QStringLiteral("SetActive"),
            [](bool){ return QList<QVariant>{true}; }
        }
    };

    void call(const std::vector<Endpoint> &implementations, bool force)
    {
        QString lastError;

        for(const auto &impl: implementations)
        {
            QDBusInterface dbusInterface(impl.service.name, impl.service.path, impl.service.interface, impl.connection);
            if(!dbusInterface.isValid())
                continue;

            auto reply = dbusInterface.callWithArgumentList(QDBus::AutoDetect, impl.function, impl.boolToArgs(force));

            if(reply.type() == QDBusMessage::ErrorMessage)
            {
                lastError = reply.errorMessage();

                continue;
            }

            return;
        }

        throw BackendError(lastError);
    }

    void logoutUnix(bool force)
    {
        call(logoutEndpoints, force);
    }

    void restartUnix(bool force)
    {
        call(restartEndpoints, force);
    }

    void shutdownUnix(bool force)
    {
        call(shutdownEndpoints, force);
    }

    void suspendUnix(bool force)
    {
        call(suspendEndpoints, force);
    }

    void hibernateUnix(bool force)
    {
        call(hibernateEndpoints, force);
    }

    void lockScreenUnix()
    {
        call(lockScreenEndpoints, false);
    }

    void startScreenSaverUnix()
    {
        call(startScreenSaverEndpoints, false);
    }

    QString getUsernameUnix()
    {
        return qEnvironmentVariable("USER");
    }
}

