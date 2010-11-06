/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#include "pointlistwidget.h"
#include "ui_pointlistwidget.h"

#include <QDebug>

namespace ActionTools
{
	PointListWidget::PointListWidget(QWidget *parent)
		: QWidget(parent),
		ui(new Ui::PointListWidget)
	{
		ui->setupUi(this);

		updateClearStatus();
		on_list_itemSelectionChanged();

		connect(ui->addPositionPushButton, SIGNAL(positionChosen(QPoint)), this, SLOT(positionChosen(QPoint)));
		connect(ui->capturePathPushButton, SIGNAL(positionChosen(QPoint)), this, SLOT(stopCapture()));
		connect(&mCaptureTimer, SIGNAL(timeout()), this, SLOT(capture()));
	}

	PointListWidget::~PointListWidget()
	{
		delete ui;
	}

	QList<QPoint> PointListWidget::points() const
	{
		QList<QPoint> back;

		for(int index = 0; index < ui->list->count(); ++index)
		{
			QStringList points = ui->list->item(index)->text().split(':');

			if(points.size() != 2)
				continue;

			back.append(QPoint(points.at(0).toInt(), points.at(1).toInt()));
		}

		return back;
	}

	void PointListWidget::setPoints(const QList<QPoint> &points)
	{
		on_clearPushButton_clicked();

		foreach(const QPoint &point, points)
		{
			QListWidgetItem *item = new QListWidgetItem(QString("%1:%2").arg(point.x()).arg(point.y()));
			item->setFlags(item->flags() | Qt::ItemIsEditable);
			ui->list->addItem(item);
		}

		updateClearStatus();
	}

	void PointListWidget::addPoint(const QPoint &point)
	{
		QListWidgetItem *item = new QListWidgetItem(QString("%1:%2").arg(point.x()).arg(point.y()));
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		ui->list->addItem(item);
		ui->list->scrollToItem(item);

		updateClearStatus();
	}

	void PointListWidget::clear()
	{
		on_clearPushButton_clicked();
	}

	void PointListWidget::on_addPushButton_clicked()
	{
		QListWidgetItem *item = new QListWidgetItem("0:0");
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		ui->list->addItem(item);
		ui->list->scrollToItem(item);

		updateClearStatus();
	}

	void PointListWidget::on_addPositionPushButton_clicked()
	{
		updateClearStatus();
	}

	void PointListWidget::on_removePushButton_clicked()
	{
		ui->list->takeItem(ui->list->row(ui->list->currentItem()));

		updateClearStatus();
	}

	void PointListWidget::on_clearPushButton_clicked()
	{
		ui->list->clear();

		updateClearStatus();
	}

	void PointListWidget::positionChosen(QPoint position)
	{
		addPoint(position);
	}

	void PointListWidget::on_list_itemSelectionChanged()
	{
		ui->removePushButton->setEnabled(!ui->list->selectedItems().isEmpty());
	}

	void PointListWidget::on_capturePathPushButton_chooseStarted()
	{
		mCaptureTimer.start(ui->captureIntervalSpinBox->value());

		qDebug() << "dfdf";
	}

	void PointListWidget::capture()
	{
		addPoint(QCursor::pos());
	}

	void PointListWidget::stopCapture()
	{
		mCaptureTimer.stop();
	}

	void PointListWidget::updateClearStatus()
	{
		ui->clearPushButton->setEnabled(ui->list->count() > 0);
	}
}
