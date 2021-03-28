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

#pragma once

#include "backend/backend_global.hpp"
#include "backend/backend.hpp"
#include "backend/feature.hpp"

#include <qwindowdefs.h>
#include <QObject>
#include <QString>
#include <QRect>
#include <QPoint>
#include <QSize>
#include <QList>
#include <QPixmap>

class QWidget;

namespace Backend
{
    class Capabilities;

    class BACKENDSHARED_EXPORT Chooser : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY(Chooser)

    public:
        explicit Chooser(QObject *parent = nullptr): QObject(parent) {}
        virtual ~Chooser() = default;

        virtual void choose() = 0;

    signals:
        void canceled();
        void errorOccurred(const BackendError &error);
    };

    class BACKENDSHARED_EXPORT PositionChooser : public Chooser
    {
        Q_OBJECT
        Q_DISABLE_COPY(PositionChooser)

    public:
        explicit PositionChooser(QObject *parent = nullptr): Chooser(parent) {}
        ~PositionChooser() override = default;

    signals:
        void done(const QPoint &position);
    };

    class BACKENDSHARED_EXPORT PositionChooserDummy final : public PositionChooser
    {
        Q_OBJECT
        Q_DISABLE_COPY(PositionChooserDummy)

    public:
        explicit PositionChooserDummy(QObject *parent = nullptr): PositionChooser(parent) {}
        ~PositionChooserDummy() override = default;

        void choose() override { emit done({}); }
    };

    class BACKENDSHARED_EXPORT AreaChooser : public Chooser
    {
        Q_OBJECT
        Q_DISABLE_COPY(AreaChooser)

    public:
        explicit AreaChooser(QObject *parent = nullptr): Chooser(parent) {}
        ~AreaChooser() override = default;

    signals:
        void done(const QRect &area);
    };

    class BACKENDSHARED_EXPORT AreaChooserDummy final : public AreaChooser
    {
        Q_OBJECT
        Q_DISABLE_COPY(AreaChooserDummy)

    public:
        explicit AreaChooserDummy(QObject *parent = nullptr): AreaChooser(parent) {}
        ~AreaChooserDummy() override = default;

        void choose() override { emit done({}); }
    };

    class BACKENDSHARED_EXPORT WindowChooser : public Chooser
    {
        Q_OBJECT
        Q_DISABLE_COPY(WindowChooser)

    public:
        explicit WindowChooser(QObject *parent = nullptr): Chooser(parent) {}
        ~WindowChooser() override = default;

    signals:
        void done(const WId &window);
    };

    class BACKENDSHARED_EXPORT WindowChooserDummy final : public WindowChooser
    {
        Q_OBJECT
        Q_DISABLE_COPY(WindowChooserDummy)

    public:
        explicit WindowChooserDummy(QObject *parent = nullptr): WindowChooser(parent) {}
        ~WindowChooserDummy() override = default;

        void choose() override { emit done({}); }
    };

    class BACKENDSHARED_EXPORT Screenshooter : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY(Screenshooter)

    public:
        explicit Screenshooter(QObject *parent = nullptr): QObject(parent) {}
        virtual ~Screenshooter() = default;

        virtual void capture(bool includeCursor, bool includeFrame, bool flash) = 0;

    signals:
        void done(const QPixmap &pixmap);
        void canceled();
        void errorOccurred(const BackendError &error);
    };

    class BACKENDSHARED_EXPORT ScreenshooterDummy final : public Screenshooter
    {
        Q_OBJECT
        Q_DISABLE_COPY(ScreenshooterDummy)

    public:
        explicit ScreenshooterDummy(QObject *parent = nullptr): Screenshooter(parent) {}
        ~ScreenshooterDummy() override = default;

        void capture(bool includeCursor, bool includeFrame, bool flash) override
        {
            Q_UNUSED(includeCursor)
            Q_UNUSED(includeFrame)
            Q_UNUSED(flash)

            emit done({});
        }
    };

    class BACKENDSHARED_EXPORT Windowing final
    {
        Q_DISABLE_COPY(Windowing)

    public:
        Windowing(Capabilities &caps);

        Feature<void(WId windowId)> setForegroundWindow
        {
            QStringLiteral("setForegroundWindow"),
            [](WId){}
        };
        Feature<QString(WId windowId)> title
        {
            QStringLiteral("title"),
            [](WId){ return QString{}; }
        };
        Feature<QString(WId windowId)> classname
        {
            QStringLiteral("classname"),
            [](WId){ return QString{}; }
        };
        Feature<QRect(WId windowId, bool useBorders)> rect
        {
            QStringLiteral("rect"),
            [](WId, bool){ return QRect{}; }
        };
        Feature<int(WId windowId)> processId
        {
            QStringLiteral("processId"),
            [](WId){ return 0; }
        };
        Feature<void(WId windowId)> close
        {
            QStringLiteral("close"),
            [](WId){}
        };
        Feature<void(WId windowId)> killCreator
        {
            QStringLiteral("killCreator"),
            [](WId){}
        };
        Feature<void(WId windowId)> minimize
        {
            QStringLiteral("minimize"),
            [](WId){}
        };
        Feature<void(WId windowId)> maximize
        {
            QStringLiteral("maximize"),
            [](WId){}
        };
        Feature<void(WId windowId, const QPoint &position)> move
        {
            QStringLiteral("move"),
            [](WId, const QPoint &){}
        };
        Feature<void(WId windowId, const QSize &size, bool useBorders)> resize
        {
            QStringLiteral("resize"),
            [](WId, const QSize &, bool){}
        };
        Feature<bool(WId windowId)> isActive
        {
            QStringLiteral("isActive"),
            [](WId){ return false; }
        };
        Feature<WId()> foregroundWindow
        {
            QStringLiteral("foregroundWindow"),
            []{ return WId{}; }
        };
        Feature<QList<WId>()> windowList
        {
            QStringLiteral("windowList"),
            []{ return QList<WId>{}; }
        };
        Feature<PositionChooser*(QObject*)> createPositionChooser
        {
            QStringLiteral("createPositionChooser"),
            [](QObject *parent) -> PositionChooser* { return new PositionChooserDummy(parent); }
        };
        Feature<AreaChooser*(QObject*)> createAreaChooser
        {
            QStringLiteral("createAreaChooser"),
            [](QObject *parent) -> AreaChooser* { return new AreaChooserDummy(parent); }
        };
        Feature<WindowChooser*(QObject*)> createWindowChooser
        {
            QStringLiteral("createWindowChooser"),
            [](QObject *parent) -> WindowChooser* { return new WindowChooserDummy(parent); }
        };
        Feature<Screenshooter*(QObject*)> createFullscreenScreenshooter
        {
            QStringLiteral("createFullscreenScreenshooter"),
            [](QObject *parent) -> Screenshooter* { return new ScreenshooterDummy(parent); }
        };
        Feature<Screenshooter*(QObject*)> createAreaScreenshooter
        {
            QStringLiteral("createAreaScreenshooter"),
            [](QObject *parent) -> Screenshooter* { return new ScreenshooterDummy(parent); }
        };
        Feature<Screenshooter*(QObject*)> createWindowScreenshooter
        {
            QStringLiteral("createWindowScreenshooter"),
            [](QObject *parent) -> Screenshooter* { return new ScreenshooterDummy(parent); }
        };
    };
}
