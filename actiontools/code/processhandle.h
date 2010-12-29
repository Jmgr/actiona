/*
	Contact : jmgr@jmgr.info
*/

#ifndef PROCESSHANDLE_H
#define PROCESSHANDLE_H

#include "actiontools_global.h"
#include "crossplatform.h"

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT ProcessHandle : public QObject, public QScriptable
	{
		Q_OBJECT
		Q_ENUMS(ActionTools::CrossPlatform::KillMode)
		Q_ENUMS(ActionTools::CrossPlatform::ProcessStatus)
		Q_ENUMS(Priority)

	public:
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
		static QScriptValue constructor(int processId, QScriptContext *context, QScriptEngine *engine);
		static int parameter(QScriptContext *context);

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
		bool equals(const QScriptValue &other) const;
		QString toString() const;
		int id() const;
		bool kill(ActionTools::CrossPlatform::KillMode killMode = ActionTools::CrossPlatform::GracefulThenForceful, int timeout = 3000) const;
		bool isRunning() const;
		QString command() const;
		Priority priority() const;

	private:
		int mProcessId;
	};
}

#endif // PROCESSHANDLE_H
