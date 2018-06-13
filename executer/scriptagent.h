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

#pragma once

#include "executer_global.h"

#include <QScriptEngineAgent>
#include <QStringList>

namespace LibExecuter
{
	class EXECUTERSHARED_EXPORT ScriptAgent : public QObject, public QScriptEngineAgent
	{
		Q_OBJECT
		
	public:
		enum Context
		{
			Unknown,
			ActionInit,
			Parameters,
			Actions
		};
	
		ScriptAgent(QScriptEngine *engine)
			: QScriptEngineAgent(engine),
			mCurrentParameter(-1),
			mCurrentLine(-1),
			mCurrentColumn(-1),
			mContext(Unknown),
			mPaused(false),
			mContinueExecution(true),
			mDebuggerAgent(nullptr),
			mEngineLevel(0)
																			{}
	
		void setContext(Context context)									{ mContext = context; }
		void setCurrentParameter(int currentParameter)						{ mCurrentParameter = currentParameter; }
		void pause(bool pause)												{ mPaused = pause; }
		void setDebuggerAgent(QScriptEngineAgent *debuggerAgent)			{ mDebuggerAgent = debuggerAgent;  }
		void stopExecution(bool emitSignal = true)							{ mContinueExecution = false; if(emitSignal) emit executionStopped(); }
		int currentLine() const												{ return mCurrentLine; }
		int currentColumn() const											{ return mCurrentColumn; }
		Context context() const												{ return mContext; }
		int currentParameter() const										{ return mCurrentParameter; }
		QString currentFile() const											{ return mFiles.count() > 0 ? mFiles.back() : QString(); }

	signals:
		void executionStopped();
		void evaluationStarted();
		void evaluationStopped();
	
	private:
		void contextPop() override ;
		void contextPush() override ;
		void exceptionCatch(qint64 scriptId, const QScriptValue &exception) override ;
		void exceptionThrow(qint64 scriptId, const QScriptValue &exception, bool hasHandler) override ;
		QVariant extension(Extension extension, const QVariant &argument = QVariant()) override ;
		void functionEntry(qint64 scriptId) override ;
		void functionExit(qint64 scriptId, const QScriptValue &returnValue) override ;
		void positionChange(qint64 scriptId, int lineNumber, int columnNumber) override ;
		void scriptLoad(qint64 id, const QString &program, const QString &fileName, int baseLineNumber) override ;
		void scriptUnload(qint64 id) override ;
		bool supportsExtension(Extension extension) const override ;
	
	private:
		int mCurrentParameter;
		int mCurrentLine;
		int mCurrentColumn;
		QStringList mFiles;
		Context mContext;
		bool mPaused;
		bool mContinueExecution;
		QScriptEngineAgent *mDebuggerAgent;
		int mEngineLevel;
	};
}

