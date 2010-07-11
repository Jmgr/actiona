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

#include "actionmessageboxinstance.h"
#include "consolewidget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

ActionTools::StringListPair ActionMessageBoxInstance::icons = qMakePair(
		QStringList() << "none" << "information" << "question" << "warning" << "error",
		QStringList() << tr("None") << tr("Information") << tr("Question") << tr("Warning") << tr("Error"));

ActionTools::StringListPair ActionMessageBoxInstance::types = qMakePair(
		QStringList() << "ok" << "yesno",
		QStringList() << tr("Ok") << tr("Yes-No"));

ActionMessageBoxInstance::ActionMessageBoxInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
	: ActionTools::ActionInstance(definition, parent),
	mMessageBox(0)
{
}

void ActionMessageBoxInstance::startExecution()
{
	ActionTools::ActionInstanceExecutionHelper actionExecution(this, script(), scriptEngine());
	QString message;
	QString title;
	int icon;
	int type;
	
	mMessageBox = 0;

	if(!actionExecution.evaluateString(message, "message") ||
		!actionExecution.evaluateString(title, "title") ||
		!actionExecution.evaluateListElement(icon, "icon", "value", icons) ||
		!actionExecution.evaluateListElement(type, "type", "value", types) ||
		!actionExecution.evaluateString(mIfYesAction, "ifyes", "action") ||
		!actionExecution.evaluateString(mIfYesLine, "ifyes", "line") ||
		!actionExecution.evaluateString(mIfNoAction, "ifno", "action") ||
		!actionExecution.evaluateString(mIfNoLine, "ifno", "line"))
		return;

	mMessageBox = new QMessageBox();

	mMessageBox->setIcon(messageBoxIcon(static_cast<Icon>(icon)));
	mMessageBox->setWindowModality(Qt::NonModal);
	mMessageBox->setText(message);
	mMessageBox->setWindowTitle(title);

	Buttons buttons = static_cast<Buttons>(type);
	switch(buttons)
	{
	case OkButton:
		mMessageBox->setStandardButtons(QMessageBox::Ok);
		break;
	case YesNoButtons:
		mMessageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		break;
	}

	mMessageBox->adjustSize();
	QRect screenGeometry = QApplication::desktop()->availableGeometry();
	mMessageBox->move(screenGeometry.center());
	mMessageBox->move(mMessageBox->pos().x() - mMessageBox->width()/2, mMessageBox->pos().y() - mMessageBox->height()/2);

	mMessageBox->open(this, SLOT(buttonClicked()));
}

void ActionMessageBoxInstance::stopExecution()
{
	if(mMessageBox)
	{
		mMessageBox->close();
		mMessageBox->deleteLater();
	}
}

QMessageBox::Icon ActionMessageBoxInstance::messageBoxIcon(Icon icon) const
{
	switch(icon)
	{
	case Information:	return QMessageBox::Information;
	case Question:		return QMessageBox::Question;
	case Warning:		return QMessageBox::Warning;
	case Error:			return QMessageBox::Critical;
	default:			return QMessageBox::NoIcon;
	}
}

void ActionMessageBoxInstance::buttonClicked()
{
	QScriptValue script = scriptEngine()->globalObject().property("Script");
	if(mMessageBox->clickedButton() == mMessageBox->button(QMessageBox::Yes) && mIfYesAction == "goto")
		script.setProperty("nextLine", scriptEngine()->newVariant(QVariant(mIfYesLine)));
	else if(mMessageBox->clickedButton() == mMessageBox->button(QMessageBox::No) && mIfNoAction == "goto")
		script.setProperty("nextLine", scriptEngine()->newVariant(QVariant(mIfNoLine)));

	mMessageBox->disconnect();
	mMessageBox->deleteLater();

	emit executionEnded();
}

ActionMessageBoxInstance::Result ActionMessageBoxInstance::messageBox(const QString &text, const QString &title, Icon icon, Buttons buttons)
{
	QMessageBox::StandardButtons messageBoxButtons;

	if(buttons == OkButton)
		messageBoxButtons = QMessageBox::Ok;
	else
		messageBoxButtons = QMessageBox::Yes | QMessageBox::No;

	QMessageBox *messageBox = new QMessageBox(messageBoxIcon(icon), title, text, messageBoxButtons);

	messageBox->setWindowFlags(messageBox->windowFlags() & Qt::WA_DeleteOnClose);

	messageBox->adjustSize();
	QRect screenGeometry = QApplication::desktop()->availableGeometry();
	messageBox->move(screenGeometry.center());
	messageBox->move(messageBox->pos().x() - messageBox->width()/2, messageBox->pos().y() - messageBox->height()/2);

	int result = messageBox->exec();

	if(buttons == OkButton)
		return Ok;

	return (result == QMessageBox::Yes ? Yes : No);
}
