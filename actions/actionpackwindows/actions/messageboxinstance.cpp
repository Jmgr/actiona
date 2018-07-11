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

#include "messageboxinstance.h"
#include "consolewidget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScriptValue>
#include <QScriptEngine>
#include <QIcon>

namespace Actions
{
    Tools::StringListPair MessageBoxInstance::icons =
    {
        {
            QStringLiteral("none"),
            QStringLiteral("information"),
            QStringLiteral("question"),
            QStringLiteral("warning"),
            QStringLiteral("error")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("MessageBoxInstance::icons", "None")),
            QStringLiteral(QT_TRANSLATE_NOOP("MessageBoxInstance::icons", "Information")),
            QStringLiteral(QT_TRANSLATE_NOOP("MessageBoxInstance::icons", "Question")),
            QStringLiteral(QT_TRANSLATE_NOOP("MessageBoxInstance::icons", "Warning")),
            QStringLiteral(QT_TRANSLATE_NOOP("MessageBoxInstance::icons", "Error"))
        }
    };

    Tools::StringListPair MessageBoxInstance::buttons =
    {
        {
            QStringLiteral("ok"),
            QStringLiteral("yesno")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("MessageBoxInstance::buttons", "Ok")),
            QStringLiteral(QT_TRANSLATE_NOOP("MessageBoxInstance::buttons", "Yes-No"))
        }
    };

    Tools::StringListPair MessageBoxInstance::textmodes =
    {
        {
            QStringLiteral("automatic"),
            QStringLiteral("html"),
            QStringLiteral("text")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("MessageBoxInstance::textmodes", "Automatic")),
            QStringLiteral(QT_TRANSLATE_NOOP("MessageBoxInstance::textmodes", "HTML")),
            QStringLiteral(QT_TRANSLATE_NOOP("MessageBoxInstance::textmodes", "Plain text"))
        }
    };

	MessageBoxInstance::MessageBoxInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		mMessageBox(nullptr)
	{
	}

	void MessageBoxInstance::startExecution()
	{
		bool ok = true;

		QString message = evaluateString(ok, QStringLiteral("message"));
		QString title = evaluateString(ok, QStringLiteral("title"));
		Icon icon = evaluateListElement<Icon>(ok, icons, QStringLiteral("icon"));
		auto textMode = evaluateListElement<TextMode>(ok, textmodes, QStringLiteral("textMode"));
		auto button = evaluateListElement<Buttons>(ok, buttons, QStringLiteral("type"));
		QImage customIcon = evaluateImage(ok, QStringLiteral("customIcon"));
		QImage windowIcon = evaluateImage(ok, QStringLiteral("windowIcon"));
		mIfYes = evaluateIfAction(ok, QStringLiteral("ifYes"));
		mIfNo = evaluateIfAction(ok, QStringLiteral("ifNo"));

		mMessageBox = nullptr;

		if(!ok)
			return;

		mMessageBox = new QMessageBox();

		mMessageBox->setIcon(messageBoxIcon(icon));
		mMessageBox->setWindowModality(Qt::NonModal);
		mMessageBox->setText(message);
		mMessageBox->setWindowTitle(title);
        mMessageBox->setWindowFlags(mMessageBox->windowFlags() | Qt::WindowContextHelpButtonHint);

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

        if(!customIcon.isNull())
            mMessageBox->setIconPixmap(QPixmap::fromImage(customIcon));

        if(!windowIcon.isNull())
            mMessageBox->setWindowIcon(QPixmap::fromImage(windowIcon));

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
			else if(mIfYes.action() == ActionTools::IfActionValue::CALLPROCEDURE)
			{
				if(!callProcedure(line))
				{
					closeAndDelete();

					return;
				}
			}
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
			else if(mIfNo.action() == ActionTools::IfActionValue::CALLPROCEDURE)
			{
				if(!callProcedure(line))
				{
					closeAndDelete();

					return;
				}
			}
		}

		closeAndDelete();

		executionEnded();
	}

	void MessageBoxInstance::closeAndDelete()
	{
		if(mMessageBox)
		{
			mMessageBox->close();
			mMessageBox = nullptr;
		}
	}
}
