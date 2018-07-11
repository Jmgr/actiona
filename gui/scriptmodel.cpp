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

#include "scriptmodel.h"
#include "script.h"
#include "actioninstance.h"
#include "actiondefinition.h"
#include "actionfactory.h"
#include "scriptproxymodel.h"
#include "numberformat.h"

#include <QIcon>
#include <QClipboard>
#include <QUndoStack>
#include <QBrush>
#include <QFont>
#include <QPalette>
#include <QApplication>
#include <QMimeData>
#include <QUrl>

#include <algorithm>

ScriptModel::ScriptModel(ActionTools::Script *script, ActionTools::ActionFactory *actionFactory, QObject *parent)
	: QAbstractTableModel(parent),
	mScript(script),
	mActionFactory(actionFactory),
    mUndoStack(new QUndoStack(this))
{
}

void ScriptModel::setActionsEnabled(bool enabled)
{
	QList<int> rows;

	for(int row = 0; row < rowCount(); ++row)
		rows << row;

    mUndoStack->push(new ChangeEnabledCommand(rows, enabled, this, mProxyModel));

	emit scriptEdited();
}

void ScriptModel::setActionsEnabled(const QList<int> &rows, bool enabled)
{
	if(rows.count() == 0)
		return;

    mUndoStack->push(new ChangeEnabledCommand(rows, enabled, this, mProxyModel));

	emit scriptEdited();
}

void ScriptModel::setActionsColor(const QList<int> &rows, const QColor &color)
{
	if(rows.count() == 0)
		return;

    mUndoStack->push(new ChangeColorCommand(rows, color, this, mProxyModel));

	emit scriptEdited();
}

void ScriptModel::insertAction(int row, const ActionTools::ActionInstanceBuffer &actionInstanceBuffer)
{
    mUndoStack->push(new InsertNewActionCommand(row, actionInstanceBuffer, this, mProxyModel));

    auto actionIndex = mProxyModel->mapFromSource(index(row, 0));

    mSelectionModel->select(actionIndex, QItemSelectionModel::Clear | QItemSelectionModel::Select | QItemSelectionModel::Rows);
    mSelectionModel->setCurrentIndex(actionIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);

	emit scriptEdited();
}

void ScriptModel::removeActions(const QList<int> &rows)
{
	QList<int> localRows = rows;

	if(localRows.count() == 0)
		return;

    std::sort(localRows.begin(), localRows.end(), qGreater<int>());

    mUndoStack->push(new RemoveActionCommand(localRows, this, mProxyModel));

	if(rowCount() > 0)
	{
		int lastPosition = localRows.at(0);
		if(lastPosition > rowCount() - 1)
			lastPosition = rowCount() - 1;

        auto actionIndex = mProxyModel->mapFromSource(index(lastPosition, 0));

        mSelectionModel->select(actionIndex, QItemSelectionModel::Clear | QItemSelectionModel::Select | QItemSelectionModel::Rows);
        mSelectionModel->setCurrentIndex(actionIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
	}

	emit scriptEdited();
}

void ScriptModel::moveActions(MoveDirection moveDirection, const QList<int> &rows)
{
    mUndoStack->push(new MoveActionOneRowCommand(rows, (moveDirection == UP), this, mProxyModel));

	emit scriptEdited();
}

void ScriptModel::copyActions(const QList<int> &rows)
{
	QModelIndexList indexes;

	for(int row: rows)
        indexes << mProxyModel->mapFromSource(index(row, 0, QModelIndex()));

	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setMimeData(mimeData(indexes));
}

void ScriptModel::pasteActions(int row)
{
	const QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData = clipboard->mimeData();

    dropMimeData(mimeData, Qt::CopyAction, row, 0, QModelIndex());
}

void ScriptModel::setHeatmapMode(HeatmapMode heatmapMode)
{
    mHeatmapMode = heatmapMode;

    emit dataChanged(index(0, 0), index(rowCount(), ColumnsCount - 1), {Qt::ToolTipRole, Qt::BackgroundColorRole, Qt::ForegroundRole});
}

void ScriptModel::setHeatmapColors(const std::pair<QColor, QColor> &heatmapColors)
{
    bool changed = (mHeatmapColors != heatmapColors);

    mHeatmapColors = heatmapColors;

    if(mHeatmapMode != HeatmapMode::None && changed)
        emit dataChanged(index(0, 0), index(rowCount(), ColumnsCount - 1), {Qt::ToolTipRole, Qt::BackgroundColorRole});
}

int ScriptModel::rowCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : mScript->actionCount();
}

int ScriptModel::columnCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : ColumnsCount;
}

void ScriptModel::reset()
{
	beginResetModel();
	mScript->removeAll();
	endResetModel();
}

void ScriptModel::appendActions(QList<ActionTools::ActionInstance *> instances)
{
	int rowIndex = mScript->actionCount();

	beginInsertRows({}, rowIndex, rowIndex + instances.size() - 1);
	for(auto instance: instances)
		mScript->appendAction(instance);
	endInsertRows();
}

QVariant ScriptModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
        return {};

	ActionTools::ActionInstance *actionInstance = mScript->actionAt(index.row());
	if(!actionInstance)
        return {};

	switch(role)
	{
	case ActionDataRole:
		return QVariant::fromValue(*actionInstance);
	case ActionIdRole:
		return actionInstance->definition()->id();
	case Qt::BackgroundRole:
		{
            if(mHeatmapMode != HeatmapMode::None && mScript->hasBeenExecuted())
                return QBrush{computeHeatmapColor(*actionInstance)};
            else
            {
                const QColor &color = actionInstance->color();

                if(color.isValid())
                    return QBrush{color};
            }
            break;
		}
	case Qt::FontRole:
		{
			if(!actionInstance->definition()->worksUnderThisOS())
			{
				QFont font = QApplication::font();

				font.setItalic(true);

				return font;
			}

            return {};
		}
	case Qt::ForegroundRole:
		{
            auto color = (mHeatmapMode != HeatmapMode::None && mScript->hasBeenExecuted()) ? computeHeatmapColor(*actionInstance) : actionInstance->color();

            if(color.isValid())
            {
                if(color.lightness() < 128)
                    return QBrush{Qt::white};
                else
                    return QBrush{Qt::black};
            }
            else
            {
                const QPalette &palette = QApplication::palette();

                if(!actionInstance->isEnabled())
                    return QBrush{palette.color(QPalette::Disabled, QPalette::WindowText)};

                return {};
            }
		}
    case Qt::CheckStateRole:
        switch(index.column())
        {
            case ColumnLabel:
                return QVariant(actionInstance->isEnabled() ? Qt::Checked : Qt::Unchecked);
        }
        break;
    case Qt::DisplayRole:
        switch(index.column())
        {
            case ColumnLabel:
                {
                    QString labelString = actionInstance->label();
                    if(!labelString.isNull() && !labelString.isEmpty())
                        return labelString;

                    return ActionTools::NumberFormat::labelIndexString(index.row());
                }
            case ColumnActionName:
                return actionInstance->definition()->name();
            case ColumnComment:
                switch(mHeatmapMode)
                {
                case HeatmapMode::ExecutionCount:
                    return tr("%n time(s)", "Execution count", actionInstance->executionCounter());
                case HeatmapMode::ExecutionDuration:
                    return tr("%1 seconds").arg(actionInstance->executionCounter() / 1000.0);
                case HeatmapMode::None:
                    return actionInstance->comment();
                }
        }
        break;
    case Qt::EditRole:
        switch(index.column())
        {
            case ColumnLabel:
                return actionInstance->label();
            case ColumnComment:
                return actionInstance->comment();
        }
        break;
    case Qt::ToolTipRole:
        switch(index.column())
        {
            case ColumnLabel:
                return tr("Double-clic to set the label name");
            case ColumnActionName:
                return tr("Double-clic to edit the action");
            case ColumnComment:
                if(mHeatmapMode == HeatmapMode::None)
                    return tr("Double-clic to write a comment for this action");

                return {};
        }
        break;
    case Qt::DecorationRole:
        switch(index.column())
        {
            case ColumnActionName:
                return actionInstance->definition()->cachedIcon();
        }
        break;
    case Qt::TextAlignmentRole:
        switch(index.column())
        {
            case ColumnActionName:
                return Qt::AlignCenter;
        }
        break;
	}

    return {};
}

QVariant ScriptModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
		return QVariant();

	switch(section)
	{
	case ColumnLabel:
        return tr("Line/Label");
	case ColumnActionName:
		return tr("Action");
	case ColumnComment:
        switch(mHeatmapMode)
        {
        case HeatmapMode::ExecutionCount:
            return tr("Execution count");
        case HeatmapMode::ExecutionDuration:
            return tr("Execution duration");
        case HeatmapMode::None:
            return tr("Comment");
        }
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

		return true;
	}

	ActionTools::ActionInstance *actionInstance = mScript->actionAt(index.row());
	if(!actionInstance)
		return false;

	if(role == ActionDataRole)
	{
		actionInstance->copyActionDataFrom(value.value<ActionTools::ActionInstance>());

		emit dataChanged(index, index);

		return true;
	}

	if(index.column() == ColumnLabel)
	{
		switch(role)
		{
		case Qt::CheckStateRole:
            mUndoStack->push(new ChangeEnabledCommand(QList<int>() << index.row(), value.toBool(), this, mProxyModel));

			emit scriptEdited();
			return true;
		case Qt::EditRole:
			QString lineNumber(QStringLiteral("%1").arg(index.row() + 1, 3, 10, QLatin1Char('0')));
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

            mUndoStack->push(new ChangeLabelCommand(finalValue, index.row(), this, mProxyModel));

			emit scriptEdited();
			return true;
		}
	}
	else if(index.column() == ColumnComment)
	{
		if(value.toString() == actionInstance->comment())
			return true;

        mUndoStack->push(new ChangeCommentCommand(value.toString(), index.row(), this, mProxyModel));

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
        back |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
        break;
	case ColumnComment:
        if(mHeatmapMode == HeatmapMode::None)
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
        return nullptr;

	auto mimeDataPtr = new QMimeData();
	QByteArray encodedData;
	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	QList<int> rowIdList;

	for(const QModelIndex &index: indexes)
	{
		if(!index.isValid() || index.column() != ColumnLabel)
			continue;

		if(!rowIdList.contains(index.row()))
			rowIdList << index.row();
	}

    std::sort(rowIdList.begin(), rowIdList.end(), qGreater<int>());

	for(int row: rowIdList)
	{
		ActionTools::ActionInstance *actionInstance = mScript->actionAt(row);

		if(!actionInstance)
			continue;

		stream << row;
		stream << ActionTools::ActionInstanceBuffer(actionInstance->definition()->id(), *actionInstance);
	}

	mimeDataPtr->setData(QStringLiteral("application/actiona.action"), encodedData);
	return mimeDataPtr;
}

bool ScriptModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	Q_UNUSED(column)

	if(action == Qt::IgnoreAction)
		return true;

	if(parent.isValid())
		return false;

	if(data->hasFormat(QStringLiteral("application/actiona.add.action")))
	{
		QByteArray encodedData = data->data(QStringLiteral("application/actiona.add.action"));

		if(row == -1)
			row = rowCount(QModelIndex());

		emit wantToAddAction(row, QLatin1String(encodedData));

		return true;
	}
	else if(data->hasFormat(QStringLiteral("application/actiona.action")))
	{
		QByteArray encodedData = data->data(QStringLiteral("application/actiona.action"));
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
            mUndoStack->push(new CopyActionCommand(row, actionInstanceBuffers, this, mProxyModel));
		else if(action == Qt::MoveAction)
            mUndoStack->push(new MoveActionCommand(row, previousRows, this, mProxyModel));

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
    return
    {
        QStringLiteral("application/actiona.action"),
        QStringLiteral("application/actiona.add.action"),
        QStringLiteral("text/uri-list"),
        QStringLiteral("text/plain")
    };
}

bool ScriptModel::insertRows(int row, int count, const QModelIndex &parent)
{
	beginInsertRows(parent, row, row + count - 1);
	for(int i = 0; i < count; ++i)
	{
        mScript->insertAction(row, nullptr);
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

QColor ScriptModel::computeHeatmapColor(const ActionTools::ActionInstance &actionInstance) const
{
    const auto minColor = mHeatmapColors.first.toHsv();
    const auto maxColor = mHeatmapColors.second.toHsv();
    qreal ratio = 0;

    switch(mHeatmapMode)
    {
    case HeatmapMode::ExecutionCount:
    {
        auto minMaxExecutionCounter = mScript->minMaxExecutionCounter();

        ratio = (minMaxExecutionCounter.second - minMaxExecutionCounter.first == 0) ? 0 : (actionInstance.executionCounter() - minMaxExecutionCounter.first) / static_cast<qreal>(minMaxExecutionCounter.second - minMaxExecutionCounter.first);

        break;
    }
    case HeatmapMode::ExecutionDuration:
    {
        auto executionDuration = mScript->executionDuration();

        ratio = (executionDuration == 0) ? 0 : actionInstance.executionDuration() / static_cast<qreal>(executionDuration);

        break;
    }
    case HeatmapMode::None:
        Q_ASSERT(false && "computeHeatmapColor called but no heatmap mode set");
        break;
    }

    return QColor::fromHsvF(
                minColor.hsvHueF() * (1 - ratio) + maxColor.hsvHueF() * ratio,
                minColor.hsvSaturationF() * (1 - ratio) + maxColor.hsvSaturationF() * ratio,
                minColor.valueF() * (1 - ratio) + maxColor.valueF() * ratio)
                .toRgb();
}

