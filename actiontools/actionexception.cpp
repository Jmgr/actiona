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

#include "actionexception.h"

#include <QDebug>
#include <QDataStream>

namespace ActionTools
{
	QString ActionException::ExceptionName[ExceptionCount] =
	{
		QStringLiteral(QT_TRANSLATE_NOOP("ActionException::ExceptionName", "Invalid parameter")),
		QStringLiteral(QT_TRANSLATE_NOOP("ActionException::ExceptionName", "Code error")),
		QStringLiteral(QT_TRANSLATE_NOOP("ActionException::ExceptionName", "Timeout"))
	};
	
	ActionException::ExceptionAction ActionException::ExceptionDefaultAction[ExceptionCount] =
	{
		StopExecutionExceptionAction,
		StopExecutionExceptionAction,
		SkipExceptionAction
	};

	QString ActionException::ExceptionActionName[ExceptionActionCount] =
	{
		QStringLiteral(QT_TRANSLATE_NOOP("ActionException::ExceptionActionName", "Stop execution")),
		QStringLiteral(QT_TRANSLATE_NOOP("ActionException::ExceptionActionName", "Skip current action")),
		QStringLiteral(QT_TRANSLATE_NOOP("ActionException::ExceptionActionName", "Goto a line"))
	};

	QDataStream &operator >> (QDataStream &s, ActionException::Exception &exception)
	{
		int newException;

		s >> newException;

		exception = static_cast<ActionException::Exception>(newException);

		return s;
	}

	QDataStream &operator >> (QDataStream &s, ActionException::ExceptionAction &exceptionAction)
	{
		int newExceptionAction;

		s >> newExceptionAction;

		exceptionAction = static_cast<ActionException::ExceptionAction>(newExceptionAction);

		return s;
	}

	QDataStream &operator >> (QDataStream &s, ActionException::ExceptionActionInstance &exceptionActionInstance)
	{
		ActionException::ExceptionAction action;
		QString line;

		s >> action;
		s >> line;

		exceptionActionInstance.setAction(action);
		exceptionActionInstance.setLine(line);

		return s;
	}

	QDataStream &operator << (QDataStream &s, const ActionException::ExceptionActionInstance &exceptionActionInstance)
	{
		s << exceptionActionInstance.action();
		s << exceptionActionInstance.line();

		return s;
	}

	QDebug &operator << (QDebug &dbg, const ActionException::ExceptionActionInstance &exceptionActionInstance)
	{
		dbg.space() << exceptionActionInstance.action();
		dbg.space() << exceptionActionInstance.line();

		return dbg.maybeSpace();
	}
}
