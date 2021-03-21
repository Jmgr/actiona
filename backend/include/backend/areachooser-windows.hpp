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

#include <QTimer>
#include <QWidget>

class QProcess;

namespace Backend
{
    class TargetWindow : public QWidget
    {
        Q_OBJECT
    public:
        TargetWindow();
        ~TargetWindow() override;

    signals:
        void rectangleSelected(const QRect &rect);

    protected:
        virtual void keyPressEvent(QKeyEvent *event);
        virtual void mousePressEvent(QMouseEvent *event);
        virtual void mouseReleaseEvent(QMouseEvent *event);

        void paintEvent(QPaintEvent *event) override;
        void showEvent(QShowEvent *event) override;
        void hideEvent(QHideEvent *event) override;

    private slots:
        void update();

    private:
        void mouseButtonReleased();

        QTimer mUpdateTimer;
        QPoint mMouseClickPosition;
        bool mMousePressed{false};
        QRect mResult;
    };

    class BACKENDSHARED_EXPORT AreaChooserWindows: public AreaChooser
    {
        Q_OBJECT
        Q_DISABLE_COPY(AreaChooserWindows)

    public:
        explicit AreaChooserWindows(QObject *parent);
        ~AreaChooserWindows() override;

        void choose() override;
    };
}

