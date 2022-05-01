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

#pragma once

#include "backend_global.hpp"
#include "backend/windowing.hpp"

//class QProcess;

#include <QAbstractNativeEventFilter>

#include <memory>

struct _XGC;
class QRubberBand;

namespace Backend
{
    /*
    class BACKENDSHARED_EXPORT AreaChooserX11: public AreaChooser
    {
        Q_OBJECT
        Q_DISABLE_COPY(AreaChooserX11)

    public:
        explicit AreaChooserX11(QObject *parent);
        ~AreaChooserX11() override;

        void choose() override;

    private:
        QProcess *mXRectSelProcess;
    };
    */
    class BACKENDSHARED_EXPORT AreaChooserX11 : public AreaChooser, public QAbstractNativeEventFilter
    {
        Q_OBJECT
        Q_DISABLE_COPY(AreaChooserX11)

    public:
        explicit AreaChooserX11(QObject *parent);
        ~AreaChooserX11() override;

        void choose() override;
        bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;

    private:
        void stopMouseCapture();

        unsigned long mTargetCursor;
        bool mButtonPressed{false};
        _XGC *gc;
        unsigned long cursor, cursor2;
        int rx = 0, ry = 0, rw = 0, rh = 0;
        int rect_x = 0, rect_y = 0, rect_w = 0, rect_h = 0;
        int rootX = 0, rootY = 0;
        std::unique_ptr<QRubberBand> mRubberBand;
    };
}


