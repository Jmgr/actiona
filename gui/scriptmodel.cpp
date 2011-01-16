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

#include "scriptmodel.h"
#include "script.h"
#include "actioninstance.h"
#include "actiondefinition.h"
#include "actionfactory.h"

#include <QIcon>
#include <QClipboard>
#include <QUndoStack>
#include <QBrush>
#include <QFont>
#include <QPalette>
#include <QApplication>
#include <QMimeData>
#include <QUrl>

ScriptModel::ScriptModel(ActionTools::Script *script, ActionTools::ActionFactory *actionFactory, QObject *parent)
	: QAbstractTableModel(parent),
	mScript(script),
	mActionFactory(actionFactory),
	mSelectionModel(0),
	mUndoStack(new QUndoStack(this))
{
}

void ScriptModel::setActionsEnabled(bool enabled)
{
	QList<int> rows;

	for(int row = 0; row < rowCount(); ++row)
		rows << row;

	mUndoStack->push(new ChangeEnabledCommand(rows, enabled, this));

	emit scriptEdited();
}

void ScriptModel::setActionsEnabled(const QList<int> &rows, bool enabled)
{
	if(rows.count() == 0)
		return;

	mUndoStack->push(new ChangeEnabledCommand(rows, enabled, this));

	emit scriptEdited();
}

void ScriptModel::setActionsColor(const QList<int> &rows, const QColor &color)
{
	if(rows.count() == 0)
		return;

	mUndoStack->push(new ChangeColorCommand(rows, color, this));

	emit scriptEdited();
}

void ScriptModel::insertAction(int row, const ActionTools::ActionInstanceBuffer &actionInstanceBuffer)
{
	mUndoStack->push(new InsertNewActionCommand(row, actionInstanceBuffer, this));

	mSelectionModel->select(index(row, 0), QItemSelectionModel::Clear | QItemSelectionModel::Select | QItemSelectionModel::Rows);
	mSelectionModel->setCurrentIndex(index(row, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);

	emit scriptEdited();
}

void ScriptModel::removeActions(const QList<int> &rows)
{
	QList<int> localRows = rows;

	if(localRows.count() == 0)
		return;

	qSort(localRows.begin(), localRows.end(), qGreater<int>());

	mUndoStack->push(new RemoveActionCommand(localRows, this));

	if(rowCount() > 0)
	{
		int lastPosition = localRows.at(0);
		if(lastPosition > rowCount() - 1)
			lastPosition = rowCount() - 1;

		mSelectionModel->select(index(lastPosition, 0), QItemSelectionModel::Clear | QItemSelectionModel::Select | QItemSelectionModel::Rows);
		mSelectionModel->setCurrentIndex(index(lastPosition, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
	}

	emit scriptEdited();
}

void ScriptModel::moveActions(MoveDirection moveDirection, const QList<int> &rows)
{
	mUndoStack->push(new MoveActionOneRowCommand(rows, (moveDirection == UP), this));

	emit scriptEdited();
}

void ScriptModel::copyActions(const QList<int> &rows)
{
	QModelIndexList indexes;

	foreach(int row, rows)
	{
		indexes << index(row, 0, QModelIndex());
	}

	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setMimeData(mimeData(indexes));
}

void ScriptModel::pasteActions(int row)
{
	const QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData = clipboard->mimeData();

	dropMimeData(mimeData, Qt::CopyAction, row, 0, QModelIndex());
}

int ScriptModel::rowCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : mScript->actionCount();
}

int ScriptModel::columnCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : ColumnsCount;
}

QVariant ScriptModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return QVariant();

	ActionTools::ActionInstance *actionInstance = mScript->actionAt(index.row());
	if(!actionInstance)
		return QVariant();

	switch(role)
	{
	case ActionDataRole:
		return QVariant::fromValue(*actionInstance);
	case ActionIdRole:
		return actionInstance->definition()->id();
	case Qt::BackgroundRole:
		{
			const QColor &color = actionInstance->color();

			if(color.isValid())
				return QBrush(color);

			return QBrush();
		}
	case Qt::ForegroundRole:
		{
			const QColor &color = actionInstance->color();
			if(color.isValid())
			{
				if(color.lightness() < 128)
					return QBrush(Qt::white);
				else
					return QBrush(Qt::black);
			}
			else
			{
				const QPalette &palette = QApplication::palette();

				if(!actionInstance->isEnabled())
					return QBrush(palette.color(QPalette::Disabled, QPalette::WindowText));

				return QBrush();
			}
		}
	}

	switch(index.column())
	{
	case ColumnLabel:
		switch(role)
		{
			case Qt::CheckStateRole:
				return QVariant(actionInstance->isEnabled() ? Qt::Checked : Qt::Unchecked);
			case Qt::DisplayRole:
			{
				QString labelString = actionInstance->label();
				if(!labelString.isNull() && !labelString.isEmpty())
					return labelString;

				return QString("%1").arg(index.row() + 1, 3, 10, QChar('0'));
			}
			case Qt::EditRole:
				return actionInstance->label();
		}
		break;
	case ColumnActionName:
		switch(role)
		{
			case Qt::ToolTipRole:
				return tr("Double-clic to edit the action");
			case Qt::DisplayRole:
				return actionInstance->definition()->name();
			case Qt::DecorationRole:
				return QIcon(actionInstance->definition()->icon());
			case Qt::TextAlignmentRole:
				return Qt::AlignCenter;
		}
		break;
	case ColumnComment:
		switch(role)
		{
			case Qt::DisplayRole:
			case Qt::EditRole:
				return actionInstance->comment();
		}
		break;
	}

	return QVariant();
}

QVariant ScriptModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
		return QVariant();

	switch(section)
	{
	case ColumnLabel:
		return tr("Line/ID");
	case ColumnActionName:
		return tr("Action");
	case ColumnComment:
		return tr("Comment");
	default:
		return QVariant();
	}
}

bool ScriptModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(!index.isValid())
		return false;

	if(role == ActionIdRole)
	{
		mScript->setAction(index.row(), mActionFactory->newActionInstance(value.toString()));

		emit dataChanged(index, index);
		emit scriptEdited();

		return true;
	}

	ActionTools::ActionInstance *actionInstance = mScript->actionAt(index.row());
	if(!actionInstance)
		return false;

	if(role == ActionDataRole)
	{
		actionInstance->copyActionDataFrom(value.value<ActionTools::ActionInstance>());

		emit dataChanged(index, index);
		emit scriptEdited();

		return true;
	}

	if(index.column() == ColumnLabel)
	{
		switch(role)
		{
		case Qt::CheckStateRole:
			mUndoStack->push(new ChangeEnabledCommand(QList<int>() << index.row(), value.toBool(), this));

			emit scriptEdited();
			return true;
		case Qt::EditRole:
			QString lineNumber(QString("%1").arg(index.row() + 1, 3, 10, QChar('0')));
			QString labelString(value.toString());
			QString finalValue;

			if(!labelString.isEmpty() && lineNumber != labelString)
			{
				int labelLine = mScript->labelLine(labelString);
				if(labelLine == -1 || labelLine == index.row())
					finalValue = labelString;
			}

			if(labelString == actionInstance->label() || labelString == lineNumber)
				return true;

			mUndoStack->push(new ChangeLabelCommand(finalValue, index.row(), this));

			emit scriptEdited();
			return true;
		}
	}
	else if(index.column() == ColumnComment)
	{
		if(value.toString() == actionInstance->comment())
			return true;

		mUndoStack->push(new ChangeCommentCommand(value.toString(), index.row(), this));

		emit scriptEdited();
		return true;
	}

	return false;
}

Qt::ItemFlags ScriptModel::flags(const QModelIndex &index) const
{
	if(!index.isValid())
		return Qt::ItemIsDropEnabled;

	Qt::ItemFlags back = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;

	switch(index.column())
	{
	case ColumnLabel:
		back |= Qt::ItemIsUserCheckable;
	case ColumnComment:
		back |= Qt::ItemIsEditable;
		break;
	case ColumnActionName:
		break;
	default:
		return Qt::NoItemFlags;
	}

	return back;
}

QMap<int, QVariant> ScriptModel::itemData(const QModelIndex &index) const
{
	QMap<int, QVariant> roles = QAbstractTableModel::itemData(index);
	roles.insert(ActionIdRole, mScript->actionAt(index.row())->definition()->id());
	roles.insert(ActionDataRole, QVariant::fromValue(*mScript->actionAt(index.row())));
	return roles;
}

Qt::DropActions ScriptModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

QMimeData* ScriptModel::mimeData(const QModelIndexList &indexes) const
{
	if(indexes.isEmpty())
		return 0;

	QMimeData *mimeDataPtr = new QMimeData();
	QByteArray encodedData;
	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	QList<int> rowIdList;

	foreach(const QModelIndex &index, indexes)
	{
		if(!index.isValid() || index.column() != ColumnLabel)
			continue;

		if(!rowIdList.contains(index.row()))
			rowIdList << index.row();
	}

	qSort(rowIdList.begin(), rowIdList.end(), qGreater<int>());

	foreach(int row, rowIdList)
	{
		ActionTools::ActionInstance *actionInstance = mScript->actionAt(row);

		if(!actionInstance)
			continue;

		stream << row;
		stream << ActionTools::ActionInstanceBuffer(actionInstance->definition()->id(), *actionInstance);
	}

	mimeDataPtr->setData("application/actionaz.action", encodedData);
	return mimeDataPtr;
}

bool ScriptModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	Q_UNUSED(column)

	if(action == Qt::IgnoreAction)
		return true;

	if(parent.isValid())
		return false;

	if(data->hasFormat("application/actionaz.add.action"))
	{
		QByteArray encodedData = data->data("application/actionaz.add.action");

		if(row == -1)
			row = rowCount(QModelIndex());

		emit wantToAddAction(row, encodedData);

		return true;
	}
	else if(data->hasFormat("application/actionaz.action"))
	{
		QByteArray encodedData = data->data("application/actionaz.action");
		QDataStream stream(&encodedData, QIODevice::ReadOnly);

		if(row == -1)
			row = rowCount(QModelIndex());

		QList<int> previousRows;
		QList<ActionTools::ActionInstanceBuffer> actionInstanceBuffers;

		while(!stream.atEnd())
		{
			int previousRow;
			ActionTools::ActionInstanceBuffer actionInstanceBuffer;

			stream >> previousRow;
			stream >> actionInstanceBuffer;

			previousRows << previousRow;
			actionInstanceBuffers << actionInstanceBuffer;
		}

		if(action == Qt::CopyAction)
			mUndoStack->push(new CopyActionCommand(row, actionInstanceBuffers, this));
		else if(action == Qt::MoveAction)
			mUndoStack->push(new MoveActionCommand(row, previousRows, this));

		emit scriptEdited();

		return true;
	}
	else if(data->hasUrls())
	{
		if(data->urls().count() != 1)
			return false;

		emit scriptFileDropped(data->urls().at(0).toLocalFile());

		return true;
	}
	else if(data->hasText())
	{
		emit scriptContentDropped(data->text());

		return true;
	}

	return false;
}

QStringList ScriptModel::mimeTypes() const
{
	return QStringList()
			<< "application/actionaz.action"
			<< "application/actionaz.add.action"
			<< "text/uri-list"
			<< "text/plain";
}

bool ScriptModel::insertRows(int row, int count, const QModelIndex &parent)
{
	beginInsertRows(parent, row, row + count - 1);
	for(int i = 0; i < count; ++i)
	{
		mScript->insertAction(row, 0);
	}
	endInsertRows();

	return true;
}

bool ScriptModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row + count - 1);
	for(int i = 0; i < count; ++i)
	{
		mScript->removeAction(row);
	}
	endRemoveRows();

	return true;
}

bool ScriptModel::moveRow(int row, int destination)
{
	if(row == -1 || destination == -1)
		return false;

	if(!beginMoveRows(QModelIndex(), row, row, QModelIndex(),
					  row < destination ? destination+1 : destination))
		return false;

	mScript->moveAction(row, destination);
	endMoveRows();

	return true;
}

