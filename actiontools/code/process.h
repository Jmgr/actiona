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

#ifndef PROCESS_H
#define PROCESS_H

#include "actiontools_global.h"
#include "crossplatform.h"

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Process : public QObject, public QScriptable
	{
		Q_OBJECT
		Q_ENUMS(ActionTools::CrossPlatform::KillMode)
		Q_ENUMS(ActionTools::CrossPlatform::ProcessStatus)

	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue constructor(int processId, QScriptContext *context, QScriptEngine *engine);
		static int parameter(QScriptContext *context);
		
		static QScriptValue list(QScriptContext *context, QScriptEngine *engine);

		Process();
		Process(const Process &other);
		Process(int processId);

		Process &operator=(Process other);
		Process &operator=(int processId);

		void swap(Process &other);
		void swap(int &processId);

		int processId() const;

	public slots:
		QScriptValue clone() const;
		bool equals(const QScriptValue &other) const;
		QString toString() const;
		int id() const;
		bool kill(ActionTools::CrossPlatform::KillMode killMode = ActionTools::CrossPlatform::GracefulThenForceful, int timeout = 3000) const;
		bool isRunning() const;

	private:
		int mProcessId;
	};
}

#endif // PROCESS_H
