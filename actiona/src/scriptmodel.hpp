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

#include "scriptmodelundocommands.hpp"
#include "heatmapmode.hpp"

#include <QAbstractTableModel>
#include <QItemSelectionModel>
#include <QColor>

#include <utility>

namespace ActionTools
{
	class Script;
	class ActionInstance;
	class ActionFactory;
}

class QItemSelectionModel;
class QUndoStack;
class ScriptProxyModel;

class ScriptModel : public QAbstractTableModel
{
	Q_OBJECT

	friend class ChangeEnabledCommand;
	friend class ChangeColorCommand;
	friend class ChangeCommentCommand;
	friend class ChangeLabelCommand;
	friend class ChangeDataCommand;
	friend class CopyActionCommand;
	friend class InsertNewActionCommand;
	friend class RemoveActionCommand;
	friend class MoveActionCommand;
	friend class MoveActionOneRowCommand;

public:
	enum Roles
	{
		ActionIdRole = Qt::UserRole,
		ActionDataRole
	};
	enum MoveDirection
	{
		UP,
		DOWN
	};
	enum Columns
	{
		ColumnLabel,
		ColumnActionName,
		ColumnComment,

		ColumnsCount
	};

    ScriptModel(ActionTools::Script *script, ActionTools::ActionFactory *actionFactory, QObject *parent = nullptr);

	void setSelectionModel(QItemSelectionModel *selectionModel)			{ mSelectionModel = selectionModel; }
    void setProxyModel(ScriptProxyModel *proxyModel)                    { mProxyModel = proxyModel; }
	QUndoStack *undoStack() const										{ return mUndoStack; }
    void setHeatmapColors(const std::pair<QColor, QColor> &heatmapColors);
    std::pair<QColor, QColor> heatmapColors() const                     { return mHeatmapColors; }

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	void reset();
	void appendActions(QList<ActionTools::ActionInstance *> instances);

public slots:
	void setActionsEnabled(bool enabled);
	void setActionsEnabled(const QList<int> &rows, bool enabled);
	void setActionsColor(const QList<int> &rows, const QColor &color);
	void insertAction(int row, const ActionTools::ActionInstanceBuffer &actionInstanceBuffer);
	void removeActions(const QList<int> &rows);
	void moveActions(MoveDirection moveDirection, const QList<int> &rows);
	void copyActions(const QList<int> &rows);
	void pasteActions(int row);
    void setHeatmapMode(HeatmapMode heatmapMode);

signals:
	void scriptEdited();
	void wantToAddAction(int row, const QString &actionId);
	void scriptFileDropped(const QString &scriptFilename);
	void scriptContentDropped(const QString &scriptContent);

protected:
	void emitDataChanged(const QModelIndex &index)						{ emit dataChanged(index, index); }

private:
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QMap<int, QVariant> itemData(const QModelIndex &index) const override;
	Qt::DropActions supportedDropActions() const override;
	QMimeData* mimeData(const QModelIndexList &indexes) const override;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
	QStringList mimeTypes() const override;
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool moveRow(int row, int destination);
    QColor computeHeatmapColor(const ActionTools::ActionInstance &actionInstance) const;

	ActionTools::Script *mScript;
	ActionTools::ActionFactory *mActionFactory;
    QItemSelectionModel *mSelectionModel{};
    ScriptProxyModel *mProxyModel{};
	QUndoStack *mUndoStack;
    HeatmapMode mHeatmapMode{HeatmapMode::None};
    std::pair<QColor, QColor> mHeatmapColors{QColor{Qt::yellow}, QColor{Qt::red}};

	Q_DISABLE_COPY(ScriptModel)
};

