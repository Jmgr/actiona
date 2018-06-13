/*
	Contact : jmgr@jmgr.info
*/

#pragma once

#include "actiontools_global.h"
#include "crossplatform.h"
#include "code/codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT ProcessHandle : public CodeClass
	{
		Q_OBJECT
		Q_ENUMS(KillMode)
		Q_ENUMS(Priority)

	public:
		enum KillMode
		{
			Graceful,
			Forceful,
			GracefulThenForceful
		};
		enum Priority
		{
			AboveNormal,
			BelowNormal,
			High,
			Idle,
			Normal,
			Realtime
		};

		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue constructor(int processId, QScriptEngine *engine);
		static int parameter(QScriptContext *context, QScriptEngine *engine);

		static void registerClass(QScriptEngine *scriptEngine);

		ProcessHandle();
		ProcessHandle(const ProcessHandle &other);
		ProcessHandle(int processId);

		ProcessHandle &operator=(ProcessHandle other);
		ProcessHandle &operator=(int processId);

		void swap(ProcessHandle &other);
		void swap(int &processId);

		int processId() const;

	public slots:
		QScriptValue clone() const;
		bool equals(const QScriptValue &other) const override;
		QString toString() const override;
		int id() const;
        int parentId() const;
		bool kill(KillMode killMode = GracefulThenForceful, int timeout = 3000) const;
		bool isRunning() const;
		QString command() const;
		Priority priority() const;

	private:
		int mProcessId;
	};
}

