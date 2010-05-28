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

#ifndef ACTIONMESSAGEBOXINSTANCE_H
#define ACTIONMESSAGEBOXINSTANCE_H

#include "actionexecution.h"
#include "action.h"

#include <QMessageBox>

class ActionMessageBoxInstance : public ActionTools::Action
{
	Q_OBJECT
	Q_ENUMS(Icon)
	Q_ENUMS(Buttons)
	Q_ENUMS(Result)

public:
	enum Icon
	{
		None,
		Information,
		Question,
		Warning,
		Error
	};
	enum Buttons
	{
		OkButton,
		YesNoButtons
	};
	enum Result
	{
		Ok,
		Yes,
		No
	};

	ActionMessageBoxInstance(ActionTools::ActionInterface *interface, QObject *parent = 0);
	ActionMessageBoxInstance(QObject *parent = 0);
	~ActionMessageBoxInstance();

	static ActionTools::StringListPair icons;
	static ActionTools::StringListPair types;

	void startExecution();
	void stopExecution();

private:
	QMessageBox::Icon messageBoxIcon(Icon icon) const;

private slots:
	void buttonClicked();

public slots:
	ActionMessageBoxInstance::Result messageBox(const QString &text, const QString &title = QString(), Icon icon = None, Buttons buttons = OkButton);

private:
	QMessageBox *mMessageBox;
	QString mIfYesAction;
	QString mIfYesLine;
	QString mIfNoAction;
	QString mIfNoLine;

	Q_DISABLE_COPY(ActionMessageBoxInstance)
};

#endif // ACTIONMESSAGEBOXINSTANCE_H
