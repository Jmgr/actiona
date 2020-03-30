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

#include "actiontools_global.hpp"

#include <QWidget>
#include <QTimer>

namespace Ui
{
    class PointListWidget;
}

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT PointListWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit PointListWidget(QWidget *parent = nullptr);
		~PointListWidget() override;

		QPolygon points() const;
		void setPoints(const QPolygon &points);

		void addPoint(const QPoint &point);
		void clear();

	private slots:
		void on_addPushButton_clicked();
		void on_addPositionPushButton_clicked();
		void on_removePushButton_clicked();
		void on_clearPushButton_clicked();
        void positionChosen(QPointF position);
		void on_list_itemSelectionChanged();
		void on_capturePathPushButton_chooseStarted();
		void capture();
		void stopCapture();

	private:
		void updateClearStatus();

		Ui::PointListWidget *ui;
		QTimer mCaptureTimer;

		Q_DISABLE_COPY(PointListWidget)
	};
}

