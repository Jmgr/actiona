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

#include "scriptagent.h"

namespace LibExecuter
{
	void ScriptAgent::contextPop()
	{
		if(mDebuggerAgent)
			mDebuggerAgent->contextPop();
	}

	void ScriptAgent::contextPush()
	{
		if(mDebuggerAgent)
			mDebuggerAgent->contextPush();
	}

	void ScriptAgent::exceptionCatch(qint64 scriptId, const QScriptValue &exception)
	{
		if(mDebuggerAgent)
			mDebuggerAgent->exceptionCatch(scriptId, exception);
	}

	void ScriptAgent::exceptionThrow(qint64 scriptId, const QScriptValue &exception, bool hasHandler)
	{
		if(mDebuggerAgent)
			mDebuggerAgent->exceptionThrow(scriptId, exception, hasHandler);
	}

	QVariant ScriptAgent::extension(Extension extension, const QVariant &argument)
	{
		if(mDebuggerAgent)
			return mDebuggerAgent->extension(extension, argument);

		return QScriptEngineAgent::extension(extension, argument);
	}

	void ScriptAgent::functionEntry(qint64 scriptId)
	{
		if(mDebuggerAgent)
			mDebuggerAgent->functionEntry(scriptId);
	}

	void ScriptAgent::functionExit(qint64 scriptId, const QScriptValue &returnValue)
	{
		if(mDebuggerAgent)
			mDebuggerAgent->functionExit(scriptId, returnValue);
	}

	void ScriptAgent::positionChange(qint64 scriptId, int lineNumber, int columnNumber)
	{
		if(mDebuggerAgent)
			mDebuggerAgent->positionChange(scriptId, lineNumber, columnNumber);

		mCurrentLine = lineNumber;
		mCurrentColumn = columnNumber;
	}

	void ScriptAgent::scriptLoad(qint64 id, const QString &program, const QString &fileName, int baseLineNumber)
	{
		if(mDebuggerAgent)
			mDebuggerAgent->scriptLoad(id, program, fileName, baseLineNumber);

		if(mEngineLevel == 0)
			emit evaluationStarted();

		mFiles.append(program);

		++mEngineLevel;
	}

	void ScriptAgent::scriptUnload(qint64 id)
	{
		if(mDebuggerAgent)
			mDebuggerAgent->scriptUnload(id);

		--mEngineLevel;

		mFiles.pop_back();

		if(mEngineLevel == 0)
			emit evaluationStopped();
	}

	bool ScriptAgent::supportsExtension(Extension extension) const
	{
		if(mDebuggerAgent)
			return mDebuggerAgent->supportsExtension(extension);

		return QScriptEngineAgent::supportsExtension(extension);
	}
}
