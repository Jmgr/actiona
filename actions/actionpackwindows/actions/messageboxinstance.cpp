/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#include "messageboxinstance.h"
#include "consolewidget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScriptValue>
#include <QScriptEngine>
#include <QIcon>

namespace Actions
{
	ActionTools::StringListPair MessageBoxInstance::icons = qMakePair(
			QStringList() << "none" << "information" << "question" << "warning" << "error",
			QStringList()
			<< QT_TRANSLATE_NOOP("MessageBoxInstance::icons", "None")
			<< QT_TRANSLATE_NOOP("MessageBoxInstance::icons", "Information")
			<< QT_TRANSLATE_NOOP("MessageBoxInstance::icons", "Question")
			<< QT_TRANSLATE_NOOP("MessageBoxInstance::icons", "Warning")
			<< QT_TRANSLATE_NOOP("MessageBoxInstance::icons", "Error"));

	ActionTools::StringListPair MessageBoxInstance::buttons = qMakePair(
			QStringList() << "ok" << "yesno",
			QStringList()
			<< QT_TRANSLATE_NOOP("MessageBoxInstance::buttons", "Ok")
			<< QT_TRANSLATE_NOOP("MessageBoxInstance::buttons", "Yes-No"));

	MessageBoxInstance::MessageBoxInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		mMessageBox(0)
	{
	}

	void MessageBoxInstance::startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
		QString message;
		QString title;
		Icon icon;
		Buttons button;
		QString customIcon;
		QString windowIcon;

		mMessageBox = 0;

		if(!actionInstanceExecutionHelper.evaluateString(message, "message") ||
			!actionInstanceExecutionHelper.evaluateString(title, "title") ||
			!actionInstanceExecutionHelper.evaluateListElement(icon, icons, "icon") ||
			!actionInstanceExecutionHelper.evaluateListElement(button, buttons, "type") ||
			!actionInstanceExecutionHelper.evaluateString(customIcon, "customIcon") ||
			!actionInstanceExecutionHelper.evaluateString(windowIcon, "windowIcon") ||
			!actionInstanceExecutionHelper.evaluateIfAction(mIfYes, "ifYes") ||
			!actionInstanceExecutionHelper.evaluateIfAction(mIfNo, "ifNo"))
			return;

		mMessageBox = new QMessageBox();

		mMessageBox->setIcon(messageBoxIcon(icon));
		mMessageBox->setWindowModality(Qt::NonModal);
		mMessageBox->setText(message);
		mMessageBox->setWindowTitle(title);

		if(!customIcon.isEmpty())
		{
			QPixmap customIconPixmap;

			if(customIconPixmap.load(customIcon))
				mMessageBox->setIconPixmap(customIconPixmap);
		}

		if(!windowIcon.isEmpty())
		{
			QPixmap windowIconPixmap;

			if(windowIconPixmap.load(windowIcon))
				mMessageBox->setWindowIcon(QIcon(windowIconPixmap));
		}

		switch(button)
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

	void MessageBoxInstance::stopExecution()
	{
		if(mMessageBox)
		{
			mMessageBox->close();
			mMessageBox->deleteLater();
		}
	}

	QMessageBox::Icon MessageBoxInstance::messageBoxIcon(Icon icon) const
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

	void MessageBoxInstance::buttonClicked()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
		QScriptValue script = scriptEngine()->globalObject().property("Script");
		QString line;

		if(mMessageBox->clickedButton() == mMessageBox->button(QMessageBox::Yes))
		{
			if(!actionInstanceExecutionHelper.evaluateSubParameter(line, mIfYes.actionParameter()))
			{
				mMessageBox->disconnect();
				mMessageBox->deleteLater();

				return;
			}

			if(mIfYes.action() == ActionTools::IfActionValue::GOTO)
				script.setProperty("nextLine", scriptEngine()->newVariant(QVariant(line)));
		}
		else if(mMessageBox->clickedButton() == mMessageBox->button(QMessageBox::No))
		{
			if(!actionInstanceExecutionHelper.evaluateSubParameter(line, mIfNo.actionParameter()))
			{
				mMessageBox->disconnect();
				mMessageBox->deleteLater();

				return;
			}

			if(mIfNo.action() == ActionTools::IfActionValue::GOTO)
				script.setProperty("nextLine", scriptEngine()->newVariant(QVariant(line)));
		}

		mMessageBox->disconnect();
		mMessageBox->deleteLater();

		emit executionEnded();
	}
}
