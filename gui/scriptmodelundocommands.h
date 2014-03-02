/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef SCRIPTMODELUNDOCOMMANDS_H
#define SCRIPTMODELUNDOCOMMANDS_H

#include <QUndoCommand>
#include <QModelIndex>
#include <QVariant>

#include "actioninstancebuffer.h"

class ScriptModel;

class ChangeEnabledCommand : public QUndoCommand
{
public:
	ChangeEnabledCommand(const QList<int> &rows, bool enabled, ScriptModel *model);
	void redo();
	void undo();

private:
	ScriptModel *mModel;
	QList<int> mRows;
	bool mNew;
	QList<bool> mOld;
};

class ChangeColorCommand : public QUndoCommand
{
public:
	ChangeColorCommand(const QList<int> &rows, const QColor &color, ScriptModel *model);
	void redo();
	void undo();

private:
	ScriptModel *mModel;
	QList<int> mRows;
	QColor mNew;
	QList<QColor> mOld;
};

class ChangeCommentCommand : public QUndoCommand
{
public:
	ChangeCommentCommand(const QString &value, int row, ScriptModel *model);
	void redo();
	void undo();

private:
	ScriptModel *mModel;
	QString mNew;
	QString mOld;
	int mRow;
};

class ChangeLabelCommand : public QUndoCommand
{
public:
	ChangeLabelCommand(const QString &value, int row, ScriptModel *model);
	void redo();
	void undo();

private:
	ScriptModel *mModel;
	QString mNew;
	QString mOld;
	int mRow;
};

class ChangeDataCommand : public QUndoCommand
{
public:
	ChangeDataCommand(const QModelIndex &index, const ActionTools::ParametersData &value, ScriptModel *model);
	void redo();
	void undo();

private:
	ScriptModel *mModel;
	ActionTools::ParametersData mNew, mOld;
	int mRow, mCol;
};

class CopyActionCommand : public QUndoCommand
{
public:
	CopyActionCommand(int row, const QList<ActionTools::ActionInstanceBuffer> &actionInstanceBuffers, ScriptModel *model);
	void redo();
	void undo();

private:
	int mRow;
	QList<ActionTools::ActionInstanceBuffer> mActionInstanceBuffers;
	ScriptModel *mModel;
};

class InsertNewActionCommand : public QUndoCommand
{
public:
	InsertNewActionCommand(int row, const ActionTools::ActionInstanceBuffer &actionInstanceBuffer, ScriptModel *model);
	void redo();
	void undo();

private:
	int mRow;
	ActionTools::ActionInstanceBuffer mActionInstanceBuffer;
	ScriptModel *mModel;
};

class RemoveActionCommand : public QUndoCommand
{
public:
	RemoveActionCommand(const QList<int> &rows, ScriptModel *model);
	void redo();
	void undo();

private:
	QList<int> mRows;
	QList<ActionTools::ActionInstanceBuffer> mActionInstanceBuffers;
	ScriptModel *mModel;
};

class MoveActionCommand : public QUndoCommand
{
public:
	MoveActionCommand(int row, const QList<int> &previousRows, ScriptModel *model);
	void redo();
	void undo();

private:
	int mRow;
	QList<int> mPreviousRows;
	ScriptModel *mModel;
	int mChangeDest;
	int mChangePrevious;
};

class MoveActionOneRowCommand : public QUndoCommand
{
public:
	MoveActionOneRowCommand(const QList<int> &rows, bool moveUp, ScriptModel *model);
	void redo();
	void undo();

private:
	void moveAllActions(int direction);
	bool move(int &row, int direction);

	QList<int> mRows;
	bool mMoveUp;
	ScriptModel *mModel;
};

#endif // SCRIPTMODELUNDOCOMMANDS_H
