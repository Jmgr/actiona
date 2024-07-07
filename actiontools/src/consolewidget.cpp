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

#include "actiontools/consolewidget.hpp"
#include "ui_consolewidget.h"

#include <QStandardItemModel>

namespace ActionTools
{
	ConsoleWidget::ConsoleWidget(QWidget *parent)
		: QWidget(parent),
		ui(new Ui::ConsoleWidget)
		
	{
		ui->setupUi(this);

        ui->console->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->console->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		ui->clearPushButton->setEnabled(false);
	}

	ConsoleWidget::~ConsoleWidget()
	{
		delete ui;
	}
	
    void ConsoleWidget::setup(QStandardItemModel *model, int maxEntries)
	{
		mModel = (model ? model : new QStandardItemModel(0, 1, this));
        mMaxEntries = maxEntries;
		
		QItemSelectionModel *oldModel = ui->console->selectionModel();
		ui->console->setModel(mModel);
		delete oldModel;
		
        // Note: this connection is still string based because it uses a private signal
        connect(mModel, SIGNAL(rowsInserted(QModelIndex,int,int)), ui->console, SLOT(scrollToBottom()));
	}

    void ConsoleWidget::addScriptParameterLine(Type type, const QString &message, int parameter, int line, int column)
	{
		auto item = new QStandardItem();

		item->setData(parameter, ParameterRole);
		item->setData(line, LineRole);
		item->setData(column, ColumnRole);

        addLine(message, item, Parameters, type);
    }

    void ConsoleWidget::addResourceLine(ConsoleWidget::Type type, const QString &message, const QString &resourceKey)
    {
        auto item = new QStandardItem();

        item->setData(resourceKey, ResourceRole);

        addLine(message, item, Resources, type);
    }

    void ConsoleWidget::addActionLine(Type type, const QString &message, qint64 actionRuntimeId, const QString &field, const QString &subField, int line, int column)
	{
		auto item = new QStandardItem();

		item->setData(actionRuntimeId, ActionRole);
		item->setData(field, FieldRole);
		item->setData(subField, SubFieldRole);
		item->setData(line, LineRole);
		item->setData(column, ColumnRole);

		addLine(message, item, Action, type);
	}

    void ConsoleWidget::addUserLine(Type type, const QString &message, qint64 actionRuntimeId, const QString &field, const QString &subField, int line, int column)
	{
		auto item = new QStandardItem();

		item->setData(actionRuntimeId, ActionRole);
		item->setData(field, FieldRole);
		item->setData(subField, SubFieldRole);
		item->setData(line, LineRole);
		item->setData(column, ColumnRole);

		addLine(message, item, User, type);
	}
	
    void ConsoleWidget::addExceptionLine(Type type, const QString &message, qint64 actionRuntimeId, int exception)
	{
		auto item = new QStandardItem();
		
		item->setData(actionRuntimeId, ActionRole);
		item->setData(exception, ExceptionRole);

		addLine(message, item, Exception, type);
	}
	
    void ConsoleWidget::addDesignErrorLine(Type type, const QString &message)
	{
		auto item = new QStandardItem();

		addLine(message, item, DesignError, type);
	}

	void ConsoleWidget::addStartSeparator()
	{
		mStartTime = QDateTime::currentDateTime();
		QStandardItem *item = new QStandardItem(tr("Execution started at %1").arg(mStartTime.toString(QStringLiteral("dd/MM/yyyy hh:mm:ss:zzz"))));
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
        mStartTime = mStartTime.addDays(-days);

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
		int startMSec = mStartTime.toString(QStringLiteral("z")).toInt();
		int endMSec = currentDateTime.toString(QStringLiteral("z")).toInt();
		int msec = (endMSec > startMSec) ? (endMSec - startMSec) : (1000 - (startMSec - endMSec));

		durationString += tr("%n millisecond(s)", "", msec);

		QStandardItem *item = new QStandardItem(tr("Execution ended at %1\n(%2)").arg(currentDateTime.toString(QStringLiteral("dd/MM/yyyy hh:mm:ss:zzz"))).arg(durationString));
		item->setTextAlignment(Qt::AlignCenter);
		addSeparator(item);
	}

	void ConsoleWidget::clear()
	{
		mModel->removeRows(0, mModel->rowCount());

        ui->clearPushButton->setEnabled(false);
    }

    void ConsoleWidget::clearExceptSeparators()
    {
        int rowCount = mModel->rowCount();

        for(int index = rowCount - 1; index >= 0; --index)
        {
            QStandardItem *item = mModel->item(index);

            Type type = item->data(TypeRole).value<Type>();

            if(type != Separator)
                mModel->removeRow(index);
        }

        if(mModel->rowCount() == 0)
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

	void ActionTools::ConsoleWidget::on_console_clicked(const QModelIndex &index)
	{
		emit itemClicked(index.row());
	}
	
	void ConsoleWidget::addLine(const QString &message, QStandardItem *item, Source source, Type type)
	{
		QIcon icon;

		switch(type)
		{
		case Information:
			icon = QIcon(QStringLiteral(":/images/information.png"));
			break;
		case Warning:
			icon = QIcon(QStringLiteral(":/images/warning.png"));
			break;
		case Error:
			icon = QIcon(QStringLiteral(":/images/error.png"));
			break;
        case Separator:
            Q_ASSERT(false && "Should use addSeparator instead");
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

        if(mMaxEntries > 0)
        {
            if(mModel->rowCount() > mMaxEntries)
                mModel->removeRows(0, 1);
        }

        qApp->processEvents(); // This is needed so that the console output gets displayed before a blocking call (such as sleep)
        // It would be better not to have any blocking code calls, but then this would cause some bugs when the user cancels the execution during a non-blocking sleep
        // Pausing the execution and then resuming it after some time seems to be the best way to do this, but would require important changes in the code

		ui->clearPushButton->setEnabled(true);
	}

	void ConsoleWidget::addSeparator(QStandardItem *item)
	{
        item->setFlags({});
		item->setBackground(QBrush(Qt::lightGray));
                item->setForeground(QBrush(Qt::white));

		QFont appFont = QApplication::font();
		appFont.setPointSize(7);

		item->setFont(appFont);
        item->setData(QVariant::fromValue<Type>(Separator), TypeRole);

		mModel->appendRow(item);
	}
}
