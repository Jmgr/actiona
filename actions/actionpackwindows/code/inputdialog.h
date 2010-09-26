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

#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include "window.h"

class QInputDialog;

namespace Code
{
	class InputDialog : public Window
	{
		Q_OBJECT

	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

		InputDialog();

	private:
		QInputDialog *mInputDialog;
		QScriptValue mThisObject;
	};
}

#endif // INPUTDIALOG_H
