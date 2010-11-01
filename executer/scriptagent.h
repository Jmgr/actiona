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

#ifndef SCRIPTAGENT_H
#define SCRIPTAGENT_H

#include "executer_global.h"
#include "crossplatform.h"

#include <QScriptEngine>
#include <QScriptEngineAgent>
#include <QApplication>

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
			mPauseDuration(0),
			mDebuggerAgent(0)
																			{}
	
		void setContext(Context context)									{ mContext = context; }
		void setCurrentParameter(int currentParameter)						{ mCurrentParameter = currentParameter; }
		void pause(bool pause)												{ mPaused = pause; }
		void setDebuggerAgent(QScriptEngineAgent *debuggerAgent)			{ mDebuggerAgent = debuggerAgent;  }
		void setPauseDuration(qint64 duration)								{ mPauseDuration = duration; }
		void emitStopExecution()											{ mContinueExecution = false; emit stopExecution(); }
	
		int currentLine() const												{ return mCurrentLine; }
		int currentColumn() const											{ return mCurrentColumn; }
		Context context() const												{ return mContext; }
		int currentParameter() const										{ return mCurrentParameter; }
		
	signals:
		void stopExecution();
		void evaluationStarted();
		void evaluationStopped();
	
	private:
		void contextPop()
		{
			if(mDebuggerAgent)
				mDebuggerAgent->contextPop();
		}
		
		void contextPush()
		{
			if(mDebuggerAgent)
				mDebuggerAgent->contextPush();
		}
		
		void exceptionCatch(qint64 scriptId, const QScriptValue &exception)
		{
			if(mDebuggerAgent)
				mDebuggerAgent->exceptionCatch(scriptId, exception);
		}
		
		void exceptionThrow(qint64 scriptId, const QScriptValue &exception, bool hasHandler)
		{
			if(mDebuggerAgent)
				mDebuggerAgent->exceptionThrow(scriptId, exception, hasHandler);
		}
		
		QVariant extension(Extension extension, const QVariant &argument = QVariant())
		{
			if(mDebuggerAgent)
				return mDebuggerAgent->extension(extension, argument);
			
			return QScriptEngineAgent::extension(extension, argument);
		}
		
		void functionEntry(qint64 scriptId)
		{
			if(mDebuggerAgent)
				mDebuggerAgent->functionEntry(scriptId);
		}
		
		void functionExit(qint64 scriptId, const QScriptValue &returnValue)
		{
			if(mDebuggerAgent)
				mDebuggerAgent->functionExit(scriptId, returnValue);
			
			while(mContinueExecution && mPauseDuration > 0)
			{
				QApplication::processEvents();
				
				ActionTools::CrossPlatform::sleep(10);
				
				if(!mPaused)
					mPauseDuration -= 10;
			}
		}
		
		void positionChange(qint64 scriptId, int lineNumber, int columnNumber)
		{
			if(mDebuggerAgent)
				mDebuggerAgent->positionChange(scriptId, lineNumber, columnNumber);

			mCurrentLine = lineNumber;
			mCurrentColumn = columnNumber;
		}
		
		void scriptLoad(qint64 id, const QString &program, const QString &fileName, int baseLineNumber)
		{
			if(mDebuggerAgent)
				mDebuggerAgent->scriptLoad(id, program, fileName, baseLineNumber);

			emit evaluationStarted();
		}
		
		void scriptUnload(qint64 id)
		{
			if(mDebuggerAgent)
				mDebuggerAgent->scriptUnload(id);

			emit evaluationStopped();
		}
		
		bool supportsExtension(Extension extension) const
		{
			if(mDebuggerAgent)
				return mDebuggerAgent->supportsExtension(extension);

			return QScriptEngineAgent::supportsExtension(extension);
		}
	
	private:
		int mCurrentParameter;
		int mCurrentLine;
		int mCurrentColumn;
		Context mContext;
		bool mPaused;
		bool mContinueExecution;
		qint64 mPauseDuration;
		QScriptEngineAgent *mDebuggerAgent;
	};
}

#endif // SCRIPTAGENT_H
