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

#include "inputdialog.h"

#include <QScriptValueIterator>
#include <QInputDialog>

namespace Code
{
	QScriptValue InputDialog::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		InputDialog *inputDialog = new InputDialog;
		inputDialog->setupConstructorParameters(context->argument(0));

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
/*
			if(it.name() == "title")
				messageBox->mMessageBox->setWindowTitle(it.value().toString());
			else if(it.name() == "text")
				messageBox->mMessageBox->setText(it.value().toString());
			else if(it.name() == "detailedText")
				messageBox->mMessageBox->setDetailedText(it.value().toString());
			else if(it.name() == "informativeText")
				messageBox->mMessageBox->setInformativeText(it.value().toString());
			else if(it.name() == "onButtonPressed")
				messageBox->mOnButtonPressed = it.value();
*/
		}

		return inputDialog->mThisObject = engine->newQObject(inputDialog, QScriptEngine::ScriptOwnership);
	}

	InputDialog::InputDialog()
		: Window(),
		mInputDialog(new QInputDialog)
	{
	}
}
