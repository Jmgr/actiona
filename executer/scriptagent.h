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
			mPaused(false)													{}
	
		void setContext(Context context)									{ mContext = context; }
		void setCurrentParameter(int currentParameter)						{ mCurrentParameter = currentParameter; }
		void pause(bool pause)												{ mPaused = pause; }
	
		int currentLine() const												{ return mCurrentLine; }
		int currentColumn() const											{ return mCurrentColumn; }
		Context context() const												{ return mContext; }
		int currentParameter() const										{ return mCurrentParameter; }
	
	private:
		void positionChange(qint64 scriptId, int lineNumber, int columnNumber)
		{
			Q_UNUSED(scriptId);
			mCurrentLine = lineNumber;
			mCurrentColumn = columnNumber;
			
			while(mPaused)
			{
				QApplication::processEvents();
				
				ActionTools::CrossPlatform::sleep(10);
			}
		}
	
	private:
		int mCurrentParameter;
		int mCurrentLine;
		int mCurrentColumn;
		Context mContext;
		bool mPaused;
	};
}

#endif // SCRIPTAGENT_H
