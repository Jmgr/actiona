/*
	Contact: jmgr@jmgr.info
*/

#pragma once

#include "actiontools/actiontools_global.hpp"
#include "actiontools/crossplatform.hpp"
#include "actiontools/code/codeclass.hpp"

#include <QObject>
#include <QJSValue>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT ProcessHandle : public CodeClass
	{
		Q_OBJECT

	public:
		enum KillMode
		{
			Graceful,
			Forceful,
			GracefulThenForceful
		};
		Q_ENUM(KillMode)
		enum Priority
		{
			AboveNormal,
			BelowNormal,
			High,
			Idle,
			Normal,
			Realtime
		};
		Q_ENUM(Priority)

        Q_INVOKABLE ProcessHandle();
        Q_INVOKABLE explicit ProcessHandle(const ProcessHandle &other);
        Q_INVOKABLE explicit ProcessHandle(int processId);

		ProcessHandle &operator=(ProcessHandle other);
		ProcessHandle &operator=(int processId);

		void swap(ProcessHandle &other);
		void swap(int &processId);

		int processId() const;

        Q_INVOKABLE QJSValue clone() const;
        Q_INVOKABLE bool equals(const QJSValue &other) const;
        Q_INVOKABLE QString toString() const override;
        Q_INVOKABLE int id() const;
        Q_INVOKABLE int parentId() const;
        Q_INVOKABLE bool kill(KillMode killMode = GracefulThenForceful, int timeout = 3000) const;
        Q_INVOKABLE bool isRunning() const;
        Q_INVOKABLE QString command() const;
        Q_INVOKABLE Priority priority() const;

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

	private:
		int mProcessId;
	};
}

