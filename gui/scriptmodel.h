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

#ifndef SCRIPTMODEL_H
#define SCRIPTMODEL_H

#include <QAbstractTableModel>
#include <QItemSelectionModel>
#include <QColor>

#include "scriptmodelundocommands.h"

namespace ActionTools
{
	class Script;
	class ActionInstance;
	class ActionFactory;
}

class QItemSelectionModel;
class QUndoStack;

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

	ScriptModel(ActionTools::Script *script, ActionTools::ActionFactory *actionFactory, QObject *parent = 0);

	void update()														{ emit layoutChanged(); }
	void setSelectionModel(QItemSelectionModel *selectionModel)			{ mSelectionModel = selectionModel; }
	QUndoStack *undoStack() const										{ return mUndoStack; }

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

public slots:
	void setActionsEnabled(bool enabled);
	void setActionsEnabled(const QList<int> &rows, bool enabled);
	void setActionsColor(const QList<int> &rows, const QColor &color);
	void insertAction(int row, const ActionTools::ActionInstanceBuffer &actionInstanceBuffer);
	void removeActions(const QList<int> &rows);
	void moveActions(MoveDirection moveDirection, const QList<int> &rows);
	void copyActions(const QList<int> &rows);
	void pasteActions(int row);

signals:
	void scriptEdited();
	void wantToAddAction(int row, const QString &actionId);
	void scriptFileDropped(const QString &scriptFilename);
	void scriptContentDropped(const QString &scriptContent);

protected:
	void emitDataChanged(const QModelIndex &index)						{ emit dataChanged(index, index); }

private:
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QMap<int, QVariant> itemData(const QModelIndex &index) const;
	Qt::DropActions supportedDropActions() const;
	QMimeData* mimeData(const QModelIndexList &indexes) const;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
	QStringList mimeTypes() const;
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
	bool moveRow(int row, int destination);

	ActionTools::Script *mScript;
	ActionTools::ActionFactory *mActionFactory;
	QItemSelectionModel *mSelectionModel;
	QUndoStack *mUndoStack;

	Q_DISABLE_COPY(ScriptModel)
};

#endif // SCRIPTMODEL_H
