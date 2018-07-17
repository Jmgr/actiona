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

#include "actiontools_global.h"

#include <QWidget>
#include <QModelIndex>
#include <QDateTime>

namespace Ui
{
	class ConsoleWidget;
}

class QStandardItemModel;
class QStandardItem;

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT ConsoleWidget : public QWidget
	{
		Q_OBJECT

	public:
		enum Type
		{
			Information,
			Warning,
            Error,
            Separator
		};
		enum Source
		{
			Action,
			Parameters,
			User,
			Exception,
            DesignError,
            Resources
		};
		enum Role
		{
			TypeRole = Qt::UserRole + 1,
			SourceRole,
			LineRole,
			ColumnRole,
			ActionRole,
			FieldRole,
			SubFieldRole,
			BacktraceRole,
			ParameterRole,
            ExceptionRole,
            ResourceRole
		};

		explicit ConsoleWidget(QWidget *parent = nullptr);
		~ConsoleWidget() override ;
		
		void setup(QStandardItemModel *model = nullptr);

		void addScriptParameterLine(const QString &message, int parameter, int line, int column, Type type);
        void addResourceLine(const QString &message, const QString &resourceKey, Type type);
		void addActionLine(const QString &message, qint64 actionRuntimeId, const QString &field, const QString &subField, int line, int column, Type type);
		void addUserLine(const QString &message, qint64 actionRuntimeId, const QString &field, const QString &subField, int line, int column, const QStringList &backtrace, Type type);
		void addExceptionLine(const QString &message, qint64 actionRuntimeId, int exception, Type type);
		void addDesignErrorLine(const QString &message, Type type);
		void addStartSeparator();
		void addEndSeparator();
		void clear();
        void clearExceptSeparators();

		void updateClearButton();

		QStandardItemModel *model() const					{ return mModel; }

	signals:
		void itemDoubleClicked(int item);
		void itemClicked(int item);

	private slots:
		void on_clearPushButton_clicked();
		void on_console_doubleClicked(const QModelIndex &index);
		void on_console_clicked(const QModelIndex &index);

	private:
		void addLine(const QString &message, QStandardItem *item, Source source, Type type = Information);
		void addSeparator(QStandardItem *item);

		Ui::ConsoleWidget *ui;
		QStandardItemModel *mModel{nullptr};
		QDateTime mStartTime;

		Q_DISABLE_COPY(ConsoleWidget)
	};
}

Q_DECLARE_METATYPE(ActionTools::ConsoleWidget::Type)
Q_DECLARE_METATYPE(ActionTools::ConsoleWidget::Source)

