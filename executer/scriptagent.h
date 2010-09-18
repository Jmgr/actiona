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

#include "crossplatform.h"

#include <QScriptEngineAgent>
#include <QApplication>

namespace Executer
{
	class ScriptAgent : public QScriptEngineAgent
	{
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
			mPauseDuration(0),
			mDebuggerAgent(0)
																			{}
	
		void setContext(Context context)									{ mContext = context; }
		void setCurrentParameter(int currentParameter)						{ mCurrentParameter = currentParameter; }
		void pause(bool pause)												{ mPaused = pause; }
		void setDebuggerAgent(QScriptEngineAgent *debuggerAgent)			{ mDebuggerAgent = debuggerAgent;  }
		void setPauseDuration(qint64 duration)								{ mPauseDuration = duration; }
	
		int currentLine() const												{ return mCurrentLine; }
		int currentColumn() const											{ return mCurrentColumn; }
		Context context() const												{ return mContext; }
		int currentParameter() const										{ return mCurrentParameter; }
	
	private:
		void contextPop()
		{
			mDebuggerAgent->contextPop();
		}
		
		void contextPush()
		{
			mDebuggerAgent->contextPush();
		}
		
		void exceptionCatch(qint64 scriptId, const QScriptValue &exception)
		{
			mDebuggerAgent->exceptionCatch(scriptId, exception);
		}
		
		void exceptionThrow(qint64 scriptId, const QScriptValue &exception, bool hasHandler)
		{
			mDebuggerAgent->exceptionThrow(scriptId, exception, hasHandler);
		}
		
		QVariant extension(Extension extension, const QVariant &argument = QVariant())
		{
			return mDebuggerAgent->extension(extension, argument);
		}
		
		void functionEntry(qint64 scriptId)
		{
			mDebuggerAgent->functionEntry(scriptId);
		}
		
		void functionExit(qint64 scriptId, const QScriptValue &returnValue)
		{
			mDebuggerAgent->functionExit(scriptId, returnValue);
			
			while(mPauseDuration > 0)
			{
				QApplication::processEvents();
				
				ActionTools::CrossPlatform::sleep(10);
				
				if(!mPaused)
					mPauseDuration -= 10;
			}
		}
		
		void positionChange(qint64 scriptId, int lineNumber, int columnNumber)
		{
			mDebuggerAgent->positionChange(scriptId, lineNumber, columnNumber);

			mCurrentLine = lineNumber;
			mCurrentColumn = columnNumber;
		}
		
		void scriptLoad(qint64 id, const QString &program, const QString &fileName, int baseLineNumber)
		{
			mDebuggerAgent->scriptLoad(id, program, fileName, baseLineNumber);
		}
		
		void scriptUnload(qint64 id)
		{
			mDebuggerAgent->scriptUnload(id);
		}
		
		bool supportsExtension(Extension extension) const
		{
			return mDebuggerAgent->supportsExtension(extension);
		}
	
	private:
		int mCurrentParameter;
		int mCurrentLine;
		int mCurrentColumn;
		Context mContext;
		bool mPaused;
		qint64 mPauseDuration;
		QScriptEngineAgent *mDebuggerAgent;
	};
}

#endif // SCRIPTAGENT_H
