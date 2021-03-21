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

#include "backend/windowing-x11.hpp"
#include "backend/backend.hpp"
#include "backend/positionchooser-x11.hpp"
#include "backend/areachooser-x11.hpp"
#include "backend/windowchooser-x11.hpp"

#include <QWidget>
#include <QX11Info>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

namespace Backend
{
    // from wmctrl 1.07 (http://tomas.styblo.name/wmctrl/) by Tomas Styblo + UBUNTU PATCH for 64bit
    // from giuspen-x-osk by Giuseppe Penone
    #define MAX_PROPERTY_VALUE_LEN 4096

    QString get_property(Display *disp, Window win, Atom xa_prop_type, const char *prop_name)
    {
        Atom           xa_ret_type;
        int            ret_format;
        unsigned long  ret_nitems;
        unsigned long  ret_bytes_after;
        unsigned long  tmp_size;

        auto xa_prop_name = XInternAtom(disp, prop_name, False);

        /* MAX_PROPERTY_VALUE_LEN / 4 explanation (XGetWindowProperty manpage):
         *
         * long_length = Specifies the length in 32-bit multiples of the
         *               data to be retrieved.
         *
         * NOTE:  see
         * http://mail.gnome.org/archives/wm-spec-list/2003-March/msg00067.html
         * In particular:
         *
         * 	When the X window system was ported to 64-bit architectures, a
         * rather peculiar design decision was made. 32-bit quantities such
         * as Window IDs, atoms, etc, were kept as longs in the client side
         * APIs, even when long was changed to 64 bits.
         *
         */
        unsigned char *property = nullptr;

        if(XGetWindowProperty(disp, win, xa_prop_name, 0, MAX_PROPERTY_VALUE_LEN / 4, False,
                              xa_prop_type, &xa_ret_type, &ret_format,
                              &ret_nitems, &ret_bytes_after, &property) != Success)
        {
            if(property)
                XFree(property);

            throw BackendError(QObject::tr("failed to get property %1 from a window").arg(QString::fromLocal8Bit(prop_name)));
        }

        auto ret_prop = std::unique_ptr<unsigned char, void(*)(unsigned char *)>(property, [](unsigned char *pointer){ XFree(pointer); });

        if(xa_ret_type != xa_prop_type)
            throw BackendError(QObject::tr("invalid window property for %1").arg(QString::fromLocal8Bit(prop_name)));

        /* null terminate the result to make string handling easier */
        tmp_size = (ret_format / 8) * ret_nitems;

        /* UBUNTU PATCH Correct 64 Architecture implementation of 32 bit data */
        if(ret_format==32) tmp_size *= sizeof(long)/4;

        return QString::fromLocal8Bit(reinterpret_cast<const char*>(ret_prop.get()), tmp_size);
    }

    QString get_window_title(Display *disp, Window win)
    {
        QString wm_name = get_property(disp, win, XA_STRING, "WM_NAME");
        QString net_wm_name = get_property(disp, win, XInternAtom(disp, "UTF8_STRING", False), "_NET_WM_NAME");

        if(!net_wm_name.isNull())
            return net_wm_name;
        else
            return wm_name;
    }

    QString get_window_class(Display *disp, Window win)
    {
        return get_property(disp, win, XA_STRING, "WM_CLASS");
    }

    void setForegroundWindowX11(WId windowId)
    {
        auto atomActiveWindow = XInternAtom(QX11Info::display(), "_NET_ACTIVE_WINDOW", False);

        XEvent event;
        memset(&event, 0, sizeof(event));
        event.type = ClientMessage;
        event.xclient.display = QX11Info::display();
        event.xclient.window = windowId;
        event.xclient.message_type = atomActiveWindow;
        event.xclient.format = 32;
        event.xclient.data.l[0] = 1;//Message from a normal application
        event.xclient.data.l[1] = CurrentTime;

        XWindowAttributes windowAttributes;
        if(!XGetWindowAttributes(QX11Info::display(), windowId, &windowAttributes))
            throw BackendError(QObject::tr("failed to get window attributes for window id %1").arg(windowId));
        if(!XSendEvent(QX11Info::display(), windowAttributes.screen->root, False, SubstructureNotifyMask | SubstructureRedirectMask, &event))
            throw BackendError(QObject::tr("failed to send an event to window id %1").arg(windowId));
        XMapRaised(QX11Info::display(), windowId);
    }

    QString titleX11(WId windowId)
    {
        return get_window_title(QX11Info::display(), windowId);
    }

    QString classnameX11(WId windowId)
    {
        return get_window_class(QX11Info::display(), windowId);
    }

    QRect rectX11(WId windowId, bool useBorders)
    {
        XWindowAttributes windowAttributes;

        if(!XGetWindowAttributes(QX11Info::display(), windowId, &windowAttributes))
            throw BackendError(QObject::tr("failed to get window attributes for window id %1").arg(windowId));

        Window unused;
        int positionX, positionY;

        XTranslateCoordinates(QX11Info::display(), windowId, windowAttributes.root, -windowAttributes.border_width, -windowAttributes.border_width,
                              &positionX, &positionY, &unused);

        if(useBorders)
            return {positionX, positionY, windowAttributes.width + windowAttributes.border_width, windowAttributes.height + windowAttributes.border_width};
        else
            return {positionX, positionY, windowAttributes.width, windowAttributes.height};
    }

    int processIdX11(WId windowId)
    {
        auto atomPid = XInternAtom(QX11Info::display(), "_NET_WM_PID", True);

        Atom type;
        int format;
        unsigned long items;
        unsigned long bytesAfter;
        unsigned char *propPID = nullptr;

        if(XGetWindowProperty(QX11Info::display(), windowId, atomPid, 0, 1, False, XA_CARDINAL,
                              &type, &format, &items, &bytesAfter, &propPID) != Success || !propPID)
            throw BackendError(QObject::tr("failed to get window property for window id %1").arg(windowId));

        auto res = *(reinterpret_cast<unsigned long *>(propPID));
        XFree(propPID);

        return res;
    }

    void closeX11(WId windowId)
    {
        if(!XDestroyWindow(QX11Info::display(), windowId))
            throw BackendError(QObject::tr("failed to destroy window id %1").arg(windowId));
    }

    void killCreatorX11(WId windowId)
    {
        if(!XKillClient(QX11Info::display(), windowId))
            throw BackendError(QObject::tr("failed to kill client of window id %1").arg(windowId));
    }

    void minimizeX11(WId windowId)
    {
        if(!XIconifyWindow(QX11Info::display(), windowId, DefaultScreen(QX11Info::display())))
            throw BackendError(QObject::tr("failed to iconify window id %1").arg(windowId));
    }

    void maximizeX11(WId windowId)
    {
        auto atomState = XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
        auto atomMaximizeVert = XInternAtom(QX11Info::display(), "_NET_WM_STATE_MAXIMIZED_VERT", False);
        auto atomMaximizeHorz = XInternAtom(QX11Info::display(), "_NET_WM_STATE_MAXIMIZED_HORZ", False);

        XEvent event;
        memset(&event, 0, sizeof(event));
        event.type = ClientMessage;
        event.xclient.display = QX11Info::display();
        event.xclient.window = windowId;
        event.xclient.message_type = atomState;
        event.xclient.format = 32;
        event.xclient.data.l[0] = 1;//_NET_WM_STATE_ADD
        event.xclient.data.l[1] = atomMaximizeVert;
        event.xclient.data.l[2] = atomMaximizeHorz;
        event.xclient.data.l[3] = 1;//Message from a normal application

        XWindowAttributes windowAttributes;
        if(!XGetWindowAttributes(QX11Info::display(), windowId, &windowAttributes))
            throw BackendError(QObject::tr("failed to get window attributes for window id %1").arg(windowId));

        if(!XSendEvent(QX11Info::display(), windowAttributes.screen->root, False, SubstructureNotifyMask | SubstructureRedirectMask, &event))
            throw BackendError(QObject::tr("failed to send an event to window id %1").arg(windowId));
    }

    void moveX11(WId windowId, QPoint position)
    {
        if(!XMoveWindow(QX11Info::display(), windowId, position.x(), position.y()))
            throw BackendError(QObject::tr("failed to move window id %1").arg(windowId));
    }

    void resizeX11(WId windowId, QSize size, bool useBorders)
    {
        if(useBorders)
        {
            XWindowAttributes windowAttributes;

            if(!XGetWindowAttributes(QX11Info::display(), windowId, &windowAttributes))
                throw BackendError(QObject::tr("failed to get window attributes for window id %1").arg(windowId));

            size.rwidth() += windowAttributes.border_width;
            size.rheight() += windowAttributes.border_width;
        }

        if(!XResizeWindow(QX11Info::display(), windowId, size.width(), size.height()))
            throw BackendError(QObject::tr("failed to resize window id %1").arg(windowId));
    }

    bool isActiveX11(WId windowId)
    {
        return (Backend::Instance::windowing().foregroundWindow() == windowId);
    }

    WId foregroundWindowX11()
    {
        Window focus{};
        int revert = 0;

        XGetInputFocus(QX11Info::display(), &focus, &revert);

        return focus;
    }

    QList<WId> windowListX11()
    {
        auto net_clients = XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", True);

        int count = 0;
        Window* list = nullptr;
        Atom type = 0;
        int format = 0;
        unsigned long after = 0;
        if(!XGetWindowProperty(QX11Info::display(), QX11Info::appRootWindow(), net_clients, 0, 256 * sizeof(Window), False, AnyPropertyType,
                           &type, &format, reinterpret_cast<unsigned long*>(&count), &after, reinterpret_cast<unsigned char**>(&list)))
            throw BackendError(QObject::tr("failed to get window list"));

        QList<WId> res;
        res.reserve(count);
        for (int i = 0; i < count; ++i)
            res.append(list[i]);

        XFree(list);

        return res;
    }

    PositionChooser *createPositionChooserX11(QObject *parent)
    {
        return new PositionChooserX11(parent);
    }

    AreaChooser *createAreaChooserX11(QObject *parent)
    {
        return new AreaChooserX11(parent);
    }

    WindowChooser *createWindowChooserX11(QObject *parent)
    {
        return new WindowChooserX11(parent);
    }
}
