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
        Q_ENUM(Type)
		enum Source
		{
			Action,
			Parameters,
			User,
			Exception,
            DesignError,
            Resources
		};
        Q_ENUM(Source)
		enum Role
		{
			TypeRole = Qt::UserRole + 1,
			SourceRole,
			LineRole,
			ColumnRole,
			ActionRole,
			FieldRole,
			SubFieldRole,
			ParameterRole,
            ExceptionRole,
            ResourceRole
		};

		explicit ConsoleWidget(QWidget *parent = nullptr);
		~ConsoleWidget() override ;
		
        void setup(QStandardItemModel *model = nullptr, int maxEntries = 0);

        void addScriptParameterLine(Type type, const QString &message, int parameter, int line = -1, int column = -1);
        void addResourceLine(Type type, const QString &message, const QString &resourceKey);
        void addActionLine(Type type, const QString &message, qint64 actionRuntimeId, const QString &field = {}, const QString &subField = {}, int line = -1, int column = -1);
        void addUserLine(Type type, const QString &message, qint64 actionRuntimeId, const QString &field, const QString &subField, int line, int column);
        void addExceptionLine(Type type, const QString &message, qint64 actionRuntimeId, int exception);
        void addDesignErrorLine(Type type, const QString &message);
		void addStartSeparator();
		void addEndSeparator();
		void clear();
        void clearExceptSeparators();
        void setMaxEntries(int value) { mMaxEntries = value; }

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
        int mMaxEntries{0};

		Q_DISABLE_COPY(ConsoleWidget)
	};
}
