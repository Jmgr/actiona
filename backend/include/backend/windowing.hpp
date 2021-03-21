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

#include <qwindowdefs.h>
#include <QObject>
#include <QString>
#include <QRect>
#include <QPoint>
#include <QSize>
#include <QList>

#include <functional>
#include <memory>

class QWidget;

namespace Backend
{
    class BACKENDSHARED_EXPORT PositionChooser : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY(PositionChooser)

    public:
        PositionChooser(QObject *parent = nullptr): QObject(parent) {}
        virtual ~PositionChooser() = default;

        virtual void choose() = 0;

    signals:
        void done(const QPoint &position);
        void canceled();
    };

    class BACKENDSHARED_EXPORT PositionChooserDummy final : public PositionChooser
    {
        Q_OBJECT
        Q_DISABLE_COPY(PositionChooserDummy)

    public:
        PositionChooserDummy(QObject *parent = nullptr): PositionChooser(parent) {}
        ~PositionChooserDummy() = default;

        void choose() override { emit done({}); }
    };

    class BACKENDSHARED_EXPORT AreaChooser : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY(AreaChooser)

    public:
        AreaChooser(QObject *parent = nullptr): QObject(parent) {}
        virtual ~AreaChooser() = default;

        virtual void choose() = 0;

    signals:
        void done(const QRect &area);
        void canceled();
    };

    class BACKENDSHARED_EXPORT AreaChooserDummy final : public AreaChooser
    {
        Q_OBJECT
        Q_DISABLE_COPY(AreaChooserDummy)

    public:
        AreaChooserDummy(QObject *parent = nullptr): AreaChooser(parent) {}
        ~AreaChooserDummy() = default;

        void choose() override { emit done({}); }
    };

    class BACKENDSHARED_EXPORT WindowChooser : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY(WindowChooser)

    public:
        WindowChooser(QObject *parent = nullptr): QObject(parent) {}
        virtual ~WindowChooser() = default;

        virtual void choose() = 0;

    signals:
        void done(const WId &window);
        void canceled();
    };

    class BACKENDSHARED_EXPORT WindowChooserDummy final : public WindowChooser
    {
        Q_OBJECT
        Q_DISABLE_COPY(WindowChooserDummy)

    public:
        WindowChooserDummy(QObject *parent = nullptr): WindowChooser(parent) {}
        ~WindowChooserDummy() = default;

        void choose() override { emit done({}); }
    };

    class BACKENDSHARED_EXPORT Windowing final
    {
        Q_DISABLE_COPY(Windowing)

    private:
        Windowing() = default;

    public:
        std::function<void(WId windowId)> setForegroundWindow;
        std::function<QString(WId windowId)> title;
        std::function<QString(WId windowId)> classname;
        std::function<QRect(WId windowId, bool useBorders)> rect;
        std::function<int(WId windowId)> processId;
        std::function<void(WId windowId)> close;
        std::function<void(WId windowId)> killCreator;
        std::function<void(WId windowId)> minimize;
        std::function<void(WId windowId)> maximize;
        std::function<void(WId windowId, const QPoint &position)> move;
        std::function<void(WId windowId, const QSize &size, bool useBorders)> resize;
        std::function<bool(WId windowId)> isActive;
        std::function<WId()> foregroundWindow;
        std::function<QList<WId>()> windowList;
        std::function<PositionChooser*(QObject*)> createPositionChooser;
        std::function<AreaChooser*(QObject*)> createAreaChooser;
        std::function<WindowChooser*(QObject*)> createWindowChooser;

        friend std::unique_ptr<Windowing> std::make_unique<Windowing>();
    };

    // Dummy implementations
    static void setForegroundWindowDummy(WId) {}
    static QString titleDummy(WId) { return {}; }
    static QString classnameDummy(WId) { return {}; }
    static QRect rectDummy(WId, bool) { return {}; }
    static int processIdDummy(WId) { return {}; }
    static void closeDummy(WId) {}
    static void killCreatorDummy(WId) {}
    static void minimizeDummy(WId) {}
    static void maximizeDummy(WId) {}
    static void moveDummy(WId, const QPoint &) {}
    static void resizeDummy(WId, const QSize &, bool) {}
    static bool isActiveDummy(WId) { return true; }
    static WId foregroundWindowDummy() { return {}; }
    static QList<WId> windowListDummy() { return {}; }
    static PositionChooser *createPositionChooserDummy(QObject *parent) { return new PositionChooserDummy(parent); }
    static AreaChooser *createAreaChooserDummy(QObject *parent) { return new AreaChooserDummy(parent); }
    static WindowChooser *createWindowChooserDummy(QObject *parent) { return new WindowChooserDummy(parent); }
}
