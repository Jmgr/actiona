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

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>

class QProcess;

namespace Code
{
	class Process : public QObject, public QScriptable
	{
		Q_OBJECT
		Q_CLASSINFO("type", "CodeClass")
		Q_PROPERTY(QScriptValue onError READ onError WRITE setOnError)
		Q_PROPERTY(QScriptValue onFinished READ onFinished WRITE setOnFinished)
		Q_PROPERTY(QScriptValue onReadyReadStandardError READ onReadyReadStandardError WRITE setOnReadyReadStandardError)
		Q_PROPERTY(QScriptValue onReadyReadStandardOutput READ onReadyReadStandardOutput WRITE setOnReadyReadStandardOutput)
		Q_PROPERTY(QScriptValue onStarted READ onStarted WRITE setOnStarted)
		Q_PROPERTY(QScriptValue onStateChanged READ onStateChanged WRITE setOnStateChanged)

	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

		Process();
		~Process();

		void setOnError(const QScriptValue &onError)									{ mOnError = onError; }
		void setOnFinished(const QScriptValue &onFinished)								{ mOnFinished = onFinished; }
		void setOnReadyReadStandardError(const QScriptValue &onReadyReadStandardError)	{ mOnReadyReadStandardError = onReadyReadStandardError; }
		void setOnReadyReadStandardOutput(const QScriptValue &onReadyReadStandardOutput){ mOnReadyReadStandardOutput = onReadyReadStandardOutput; }
		void setOnStarted(const QScriptValue &onStarted)								{ mOnStarted = onStarted; }
		void setOnStateChanged(const QScriptValue &onStateChanged)						{ mOnStateChanged = onStateChanged; }

		QScriptValue onError() const													{ return mOnError; }
		QScriptValue onFinished() const													{ return mOnFinished; }
		QScriptValue onReadyReadStandardError() const									{ return mOnReadyReadStandardError; }
		QScriptValue onReadyReadStandardOutput() const									{ return mOnReadyReadStandardOutput; }
		QScriptValue onStarted() const													{ return mOnStarted; }
		QScriptValue onStateChanged() const												{ return mOnStateChanged; }

	public slots:
		QString toString() const					{ return "Process"; }

	private:
		QProcess *mProcess;
		QScriptValue mOnError;
		QScriptValue mOnFinished;
		QScriptValue mOnReadyReadStandardError;
		QScriptValue mOnReadyReadStandardOutput;
		QScriptValue mOnStarted;
		QScriptValue mOnStateChanged;
		QScriptValue mThisObject;
	};
}

#endif // PROCESS_H
