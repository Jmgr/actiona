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

#include "backend/areachooser-x11.hpp"
#include "backend/x11.hpp"

#include <QApplication>
#include <QCursor>
#include <QX11Info>
#include <QMouseEvent>
#include <QDebug>

#include <QRubberBand>

#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <xcb/xcb.h>

/*
#include <QProcess>
#include <QRegularExpression>
*/

namespace Backend
{
    AreaChooserX11::AreaChooserX11(QObject *parent):
        AreaChooser(parent),
        mTargetCursor(XCreateFontCursor(QX11Info::display(), XC_target)),
        mRubberBand(std::make_unique<QRubberBand>(QRubberBand::Line, nullptr))
    {
        mRubberBand->setWindowOpacity(0.5);
        mRubberBand->setWindowFlag(Qt::WindowTransparentForInput);
    }

    AreaChooserX11::~AreaChooserX11()
    {
        stopMouseCapture();

        XFreeCursor(QX11Info::display(), mTargetCursor);
    }

    void AreaChooserX11::choose()
    {
        XGCValues gcval;
        gcval.foreground = XWhitePixel(QX11Info::display(), 0);
        gcval.function = GXxor;
        gcval.background = XBlackPixel(QX11Info::display(), 0);
        gcval.plane_mask = gcval.background ^ gcval.foreground;
        gcval.subwindow_mode = IncludeInferiors;

        gc = XCreateGC(QX11Info::display(), QX11Info::appRootWindow(),
                       GCFunction | GCForeground | GCBackground | GCSubwindowMode,
                       &gcval);

        cursor = XCreateFontCursor(QX11Info::display(), XC_cross);
        cursor2 = XCreateFontCursor(QX11Info::display(), XC_lr_angle);

        QApplication::instance()->installNativeEventFilter(this);

        auto result = XGrabPointer(QX11Info::display(), QX11Info::appRootWindow(), True, ButtonMotionMask | ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync,
                        None, mTargetCursor, CurrentTime);
        if(result != GrabSuccess)
        {
            emit errorOccurred(BackendError(QStringLiteral("failed to grab the mouse pointer: error is %1").arg(formatGrabError(result))));
        }

        result = XGrabKeyboard(QX11Info::display(), QX11Info::appRootWindow(), True, GrabModeAsync, GrabModeAsync, CurrentTime);
        if(result != GrabSuccess)
        {
            emit errorOccurred(BackendError(QStringLiteral("failed to grab the keyboard: error is %1").arg(formatGrabError(result))));
        }

        /*
        result = XGrabServer(QX11Info::display());
        if(!result)
        {
            emit errorOccurred(BackendError(QStringLiteral("failed to grab the server")));
        }
        */

        mRubberBand->raise();
        mRubberBand->show();
    }

    bool AreaChooserX11::nativeEventFilter(const QByteArray &eventType, void *message, long *)
    {
        if(eventType != "xcb_generic_event_t")
            return false;

        auto *event = static_cast<xcb_generic_event_t *>(message);

        switch(event->response_type)
        {
        case XCB_MOTION_NOTIFY:
        {
            if(!mButtonPressed)
                break;

            auto *motionEvent = reinterpret_cast<xcb_motion_notify_event_t *>(event);


            /*
            if (rect_w) {
              XDrawRectangle(QX11Info::display(), QX11Info::appRootWindow(), gc, rect_x, rect_y, rect_w, rect_h);
            } else {
              XChangeActivePointerGrab(QX11Info::display(),
                                       ButtonMotionMask | ButtonReleaseMask,
                                       cursor2, CurrentTime);
            }
                    */
            rect_x = rx;
            rect_y = ry;
            rect_w = motionEvent->root_x - rect_x;
            rect_h = motionEvent->root_y - rect_y;

            if (rect_w < 0) {
              rect_x += rect_w;
              rect_w = 0 - rect_w;
            }
            if (rect_h < 0) {
              rect_y += rect_h;
              rect_h = 0 - rect_h;
            }
            mRubberBand->setGeometry(rect_x, rect_y, rect_w, rect_h);
            /*
            XDrawRectangle(QX11Info::display(), QX11Info::appRootWindow(), gc, rect_x, rect_y, rect_w, rect_h);
            XFlush(QX11Info::display());
            */

            break;
        }
        case XCB_BUTTON_PRESS:
        {
            auto *buttonPressEvent = reinterpret_cast<xcb_button_press_event_t *>(event);

            mButtonPressed = true;
            rx = buttonPressEvent->root_x;
            ry = buttonPressEvent->root_y;
            rootX = buttonPressEvent->root_x;
            rootY = buttonPressEvent->root_y;
            break;
        }
        case XCB_BUTTON_RELEASE:
        {
            auto *buttonReleaseEvent = reinterpret_cast<xcb_button_release_event_t *>(event);

            mButtonPressed = false;

            rootX = buttonReleaseEvent->root_x;
            rootY = buttonReleaseEvent->root_y;

            stopMouseCapture();
            break;
        }
        case XCB_KEY_PRESS:
        {
            auto *keyPressEvent = reinterpret_cast<xcb_key_press_event_t *>(event);

            rootX = keyPressEvent->root_x;
            rootY = keyPressEvent->root_y;

            if(keyPressEvent->detail == 0x09) // Escape
            {
                stopMouseCapture();
            }
            break;
        }
        }

        return false;
    }

    void AreaChooserX11::stopMouseCapture()
    {
        mRubberBand->hide();

        if (rect_w) {
          XDrawRectangle(QX11Info::display(), QX11Info::appRootWindow(), gc, rect_x, rect_y, rect_w, rect_h);
          XFlush(QX11Info::display());
        }
        rw = rootX - rx;
        rh = rootY - ry;
        /* cursor moves backwards */
        if (rw < 0) {
          rx += rw;
          rw = 0 - rw;
        }
        if (rh < 0) {
          ry += rh;
          rh = 0 - rh;
        }

        emit done(QRect(rx,ry, rw,rh));

        //XUngrabServer(QX11Info::display());
        XUngrabKeyboard(QX11Info::display(), CurrentTime);
        XUngrabPointer(QX11Info::display(), CurrentTime);
        XFlush(QX11Info::display());

        QApplication::instance()->removeNativeEventFilter(this);
    }
    /*
    AreaChooserX11::AreaChooserX11(QObject *parent):
        AreaChooser(parent),
        mXRectSelProcess(new QProcess(this))
    {
        mXRectSelProcess->setProgram(QStringLiteral("backend-xrectsel"));
        connect(mXRectSelProcess, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error)
        {
            if(error == QProcess::FailedToStart)
            {
                qDebug("failed to start");
                emit canceled(); // TODO: add error signal
            }
        });
        connect(mXRectSelProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this](int exitCode, QProcess::ExitStatus exitStatus)
        {
            static QRegularExpression resultRegex(QStringLiteral(R"((\d+)x(\d+)\+(\d+)\+(\d+))"));

            if(exitStatus != QProcess::NormalExit || exitCode != EXIT_SUCCESS)
            {
                emit canceled(); // TODO: add error signal
                return;
            }

            auto result = QString::fromLocal8Bit(mXRectSelProcess->readAllStandardOutput());
            auto match = resultRegex.match(result);
            if(!match.isValid())
            {
                emit canceled();
                return;
            }

            bool ok;
            int w = match.captured(1).toInt(&ok);
            int h = match.captured(2).toInt(&ok);
            int x = match.captured(3).toInt(&ok);
            int y = match.captured(4).toInt(&ok);
            if(!ok)
            {
                emit canceled();
                return;
            }

            emit done(QRect(x, y, w, h));
        });
    }

    AreaChooserX11::~AreaChooserX11()
    {
        mXRectSelProcess->kill();
    }

    void AreaChooserX11::choose()
    {
        mXRectSelProcess->start();
    }
    */
}
