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

#include "consolewidget.h"
#include "ui_consolewidget.h"

#include <QStandardItemModel>

namespace ActionTools
{
	ConsoleWidget::ConsoleWidget(QStandardItemModel *model, QWidget *parent)
		: QWidget(parent),
		ui(new Ui::ConsoleWidget),
		mModel(model)
	{
		init();
	}

	ConsoleWidget::ConsoleWidget(QWidget *parent)
		: QWidget(parent),
		ui(new Ui::ConsoleWidget),
		mModel(new QStandardItemModel(0, 1, this))
	{
		init();
	}

	ConsoleWidget::~ConsoleWidget()
	{
		delete ui;
	}

	void ConsoleWidget::addScriptParameterLine(const QString &message, int parameter, int line, int column, Type type)
	{
		QStandardItem *item = new QStandardItem();

		item->setData(parameter, ParameterRole);
		item->setData(line, LineRole);
		item->setData(column, ColumnRole);

		addLine(message, item, Parameters, type);
	}

	void ConsoleWidget::addActionLine(const QString &message, int action, const QString &field, const QString &subField, int line, int column, Type type)
	{
		QStandardItem *item = new QStandardItem();

		item->setData(action, ActionRole);
		item->setData(field, FieldRole);
		item->setData(subField, SubFieldRole);
		item->setData(line, LineRole);
		item->setData(column, ColumnRole);

		addLine(message, item, Action, type);
	}

	void ConsoleWidget::addUserLine(const QString &message, int action, const QString &field, const QString &subField, int line, int column, const QStringList &backtrace, Type type)
	{
		QStandardItem *item = new QStandardItem();

		item->setData(action, ActionRole);
		item->setData(field, FieldRole);
		item->setData(subField, SubFieldRole);
		item->setData(line, LineRole);
		item->setData(column, ColumnRole);
		item->setData(backtrace, BacktraceRole);

		addLine(message, item, User, type);
	}
	
	void ConsoleWidget::addExceptionLine(const QString &message, int action, int exception, Type type)
	{
		QStandardItem *item = new QStandardItem();
		
		item->setData(action, ActionRole);
		item->setData(exception, ExceptionRole);

		addLine(message, item, Exception, type);
	}
	
	void ConsoleWidget::addDesignErrorLine(const QString &message, Type type)
	{
		QStandardItem *item = new QStandardItem();

		addLine(message, item, DesignError, type);
	}

	void ConsoleWidget::addStartSeparator()
	{
		mStartTime = QDateTime::currentDateTime();
		QStandardItem *item = new QStandardItem(tr("Execution started at %1").arg(mStartTime.toString("dd/MM/yyyy hh:mm:ss:zzz")));
		item->setTextAlignment(Qt::AlignCenter);
		addSeparator(item);
	}

	void ConsoleWidget::addEndSeparator()
	{
		const QDateTime &currentDateTime = QDateTime::currentDateTime();
		int days = mStartTime.daysTo(currentDateTime);

		QString durationString;
		if(days > 0)
			durationString += tr("%n day(s) ", "", days);
		mStartTime.addDays(-days);

		int seconds = mStartTime.secsTo(currentDateTime);
		int hours = seconds / 3600;
		seconds = seconds % 3600;
		int minutes = seconds / 60;
		seconds = seconds % 60;

		if(hours > 0)
			durationString += tr("%n hour(s) ", "", hours);
		if(minutes > 0)
			durationString += tr("%n minute(s) ", "", minutes);
		if(seconds > 0)
			durationString += tr("%n second(s) ", "", seconds);
		int startMSec = mStartTime.toString("z").toInt();
		int endMSec = currentDateTime.toString("z").toInt();
		int msec = (endMSec > startMSec) ? (endMSec - startMSec) : (1000 - (startMSec - endMSec));

		durationString += tr("%n millisecond(s)", "", msec);

		QStandardItem *item = new QStandardItem(tr("Execution ended at %1\n(%2)").arg(currentDateTime.toString("dd/MM/yyyy hh:mm:ss:zzz")).arg(durationString));
		item->setTextAlignment(Qt::AlignCenter);
		addSeparator(item);
	}

	void ConsoleWidget::clear()
	{
		mModel->removeRows(0, mModel->rowCount());

		ui->clearPushButton->setEnabled(false);
	}

	void ConsoleWidget::updateClearButton()
	{
		ui->clearPushButton->setEnabled(mModel->rowCount() > 0);
	}

	void ConsoleWidget::on_clearPushButton_clicked()
	{
		clear();
	}

	void ConsoleWidget::on_console_doubleClicked(const QModelIndex &index)
	{
		emit itemDoubleClicked(index.row());
	}

	void ConsoleWidget::init()
	{
		ui->setupUi(this);

		QItemSelectionModel *oldModel = ui->console->selectionModel();
		ui->console->setModel(mModel);
		delete oldModel;

		ui->console->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
		ui->console->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

		ui->clearPushButton->setEnabled(false);

		connect(ui->console->model(), SIGNAL(rowsInserted(QModelIndex,int,int)), ui->console, SLOT(scrollToBottom()));
	}

	void ConsoleWidget::addLine(const QString &message, QStandardItem *item, Source source, Type type)
	{
		QIcon icon;

		switch(type)
		{
		case Information:
			icon = QIcon(":/images/information.png");
			break;
		case Warning:
			icon = QIcon(":/images/warning.png");
			break;
		case Error:
			icon = QIcon(":/images/error.png");
			break;
		}

		item->setText(message);
		if(source == DesignError)
			item->setToolTip(message);
		else
			item->setToolTip(message + tr("\nDouble-click to show"));
		item->setIcon(icon);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		item->setData(QVariant::fromValue<Source>(source), SourceRole);
		item->setData(QVariant::fromValue<Type>(type), TypeRole);

		mModel->appendRow(item);

		ui->clearPushButton->setEnabled(true);
	}

	void ConsoleWidget::addSeparator(QStandardItem *item)
	{
		item->setFlags(0);
		item->setBackground(QBrush(Qt::lightGray));
                item->setForeground(QBrush(Qt::white));

		QFont appFont = QApplication::font();
		appFont.setPointSize(7);

		item->setFont(appFont);

		mModel->appendRow(item);
	}
}
