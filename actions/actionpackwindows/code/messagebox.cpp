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

#include "messagebox.h"

#include <QScriptValueIterator>

namespace Code
{
	QScriptValue MessageBox::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		MessageBox *messageBox = new MessageBox;
		messageBox->setupConstructorParameters(context->argument(0));
		
		QScriptValueIterator it(context->argument(0));
		
		while(it.hasNext())
		{
			it.next();
	
			if(it.name() == "title")
				messageBox->mMessageBox->setWindowTitle(it.value().toString());
			else if(it.name() == "text")
				messageBox->mMessageBox->setText(it.value().toString());
			else if(it.name() == "detailedText")
				messageBox->mMessageBox->setDetailedText(it.value().toString());
			else if(it.name() == "informativeText")
				messageBox->mMessageBox->setInformativeText(it.value().toString());
			else if(it.name() == "buttons")
				messageBox->mMessageBox->setStandardButtons(static_cast<QMessageBox::StandardButton>(it.value().toInt32()));
			else if(it.name() == "icon")
				messageBox->mMessageBox->setIcon(static_cast<QMessageBox::Icon>(it.value().toInt32()));
			else if(it.name() == "defaultButton")
				messageBox->mMessageBox->setDefaultButton(static_cast<QMessageBox::StandardButton>(it.value().toInt32()));
			else if(it.name() == "escapeButton")
				messageBox->mMessageBox->setEscapeButton(static_cast<QMessageBox::StandardButton>(it.value().toInt32()));
			else if(it.name() == "onButtonPressed")
				messageBox->mOnButtonPressed = it.value();
		}

		return messageBox->mThisObject = engine->newQObject(messageBox, QScriptEngine::ScriptOwnership);
	}
	
	MessageBox::MessageBox()
		: Window(),
		mMessageBox(new QMessageBox)
	{
		setWidget(mMessageBox);
		
		mMessageBox->setWindowModality(Qt::WindowModal);
		
		connect(mMessageBox, SIGNAL(finished(int)), this, SLOT(finished(int)));
	}
	
	MessageBox::~MessageBox()
	{
		delete mMessageBox;
	}
	
	QScriptValue MessageBox::setTitle(const QString &title)
	{
		mMessageBox->setWindowTitle(title);
		
		return context()->thisObject();
	}

	QScriptValue MessageBox::setText(const QString &text)
	{
		mMessageBox->setText(text);
		
		return context()->thisObject();
	}
	
	QScriptValue MessageBox::setDetailedText(const QString &detailedText)
	{
		mMessageBox->setDetailedText(detailedText);
		
		return context()->thisObject();
	}
	
	QScriptValue MessageBox::setInformativeText(const QString &informativeText)
	{
		mMessageBox->setInformativeText(informativeText);
		
		return context()->thisObject();
	}
	
	QScriptValue MessageBox::setButtons(StandardButton buttons)
	{
		mMessageBox->setStandardButtons(static_cast<QMessageBox::StandardButton>(buttons));
		
		return context()->thisObject();
	}
	
	QScriptValue MessageBox::setIcon(Icon icon)
	{
		mMessageBox->setIcon(static_cast<QMessageBox::Icon>(icon));
		
		return context()->thisObject();
	}
	
	QScriptValue MessageBox::setDefaultButton(StandardButton button)
	{
		mMessageBox->setDefaultButton(static_cast<QMessageBox::StandardButton>(button));
		
		return context()->thisObject();
	}

	QScriptValue MessageBox::setEscapeButton(StandardButton button)
	{
		mMessageBox->setEscapeButton(static_cast<QMessageBox::StandardButton>(button));
		
		return context()->thisObject();
	}

	QScriptValue MessageBox::show()
	{
		mMessageBox->show();
		
		return context()->thisObject();
	}

	int MessageBox::showModal()
	{
		return mMessageBox->exec();
	}
	
	QScriptValue MessageBox::close()
	{
		mMessageBox->close();
		
		return context()->thisObject();
	}
	
	void MessageBox::finished(int result)
	{
		if(mOnButtonPressed.isValid())
			mOnButtonPressed.call(mThisObject, QScriptValueList() << result);
	}
}
