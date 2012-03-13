/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

	ActionTools::StringListPair MessageBoxInstance::textmodes = qMakePair(
			QStringList() << "automatic" << "html" << "text",
			QStringList()
			<< QT_TRANSLATE_NOOP("MessageBoxInstance::textmodes", "Automatic")
			<< QT_TRANSLATE_NOOP("MessageBoxInstance::textmodes", "HTML")
			<< QT_TRANSLATE_NOOP("MessageBoxInstance::textmodes", "Plain text"));

	MessageBoxInstance::MessageBoxInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		mMessageBox(0)
	{
	}

	void MessageBoxInstance::startExecution()
	{
		bool ok = true;

		QString message = evaluateString(ok, "message");
		QString title = evaluateString(ok, "title");
		Icon icon = evaluateListElement<Icon>(ok, icons, "icon");
		TextMode textMode = evaluateListElement<TextMode>(ok, textmodes, "textMode");
		Buttons button = evaluateListElement<Buttons>(ok, buttons, "type");
		QString customIcon = evaluateString(ok, "customIcon");
		QString windowIcon = evaluateString(ok, "windowIcon");
		mIfYes = evaluateIfAction(ok, "ifYes");
		mIfNo = evaluateIfAction(ok, "ifNo");

		mMessageBox = 0;

		if(!ok)
			return;

		mMessageBox = new QMessageBox();

		mMessageBox->setIcon(messageBoxIcon(icon));
		mMessageBox->setWindowModality(Qt::NonModal);
		mMessageBox->setText(message);
		mMessageBox->setWindowTitle(title);

		switch(textMode)
		{
		case HtmlTextMode:
			mMessageBox->setTextFormat(Qt::RichText);
			break;
		case PlainTextMode:
			mMessageBox->setTextFormat(Qt::PlainText);
			break;
		case AutoTextMode:
		default:
			mMessageBox->setTextFormat(Qt::AutoText);
			break;
		}

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
		closeAndDelete();
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
		bool ok = true;

		QString line;

		if(mMessageBox->clickedButton() == mMessageBox->button(QMessageBox::Yes))
		{
			line = evaluateSubParameter(ok, mIfYes.actionParameter());
			if(!ok)
			{
				closeAndDelete();

				return;
			}

			if(mIfYes.action() == ActionTools::IfActionValue::GOTO)
				setNextLine(line);
		}
		else if(mMessageBox->clickedButton() == mMessageBox->button(QMessageBox::No))
		{
			line = evaluateSubParameter(ok, mIfNo.actionParameter());
			if(!ok)
			{
				closeAndDelete();

				return;
			}

			if(mIfNo.action() == ActionTools::IfActionValue::GOTO)
				setNextLine(line);
		}

		closeAndDelete();

		emit executionEnded();
	}

	void MessageBoxInstance::closeAndDelete()
	{
		if(mMessageBox)
		{
			mMessageBox->close();
			mMessageBox->deleteLater();

			mMessageBox = 0;
		}
	}
}
