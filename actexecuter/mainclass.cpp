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

#include "mainclass.h"
#include "codeexecuter.h"
#include "scriptexecuter.h"

const Tools::Version MainClass::ScriptVersion(0, 0, 1);
const Tools::Version MainClass::ActionazVersion(0, 0, 1);

MainClass::MainClass(ExecutionMode executionMode)
	: QObject(0),
	mExecuter(0)
{
	if(executionMode == Script)
		mExecuter = new ScriptExecuter(this);
	else
		mExecuter = new CodeExecuter(this);
}

bool MainClass::start(QFile &file)
{
	return mExecuter->start(file);
}
