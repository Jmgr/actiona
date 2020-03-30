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

	Contact: jmgr@jmgr.info
*/

#include "actiontools/codedatetimeedit.hpp"
#include "actiontools/codelineedit.hpp"

#include <QMenu>
#include <QContextMenuEvent>
#include <QStyleOptionSpinBox>

namespace ActionTools
{
	CodeDateTimeEdit::CodeDateTimeEdit(QWidget *parent)
	: QDateTimeEdit(parent)
	{
		CodeLineEdit *codeLineEdit = new CodeLineEdit(parent);
		codeLineEdit->setEmbedded(true);
		setLineEdit(codeLineEdit);

		setCalendarPopup(true);

        connect(codeLineEdit, &CodeLineEdit::codeChanged, this, &CodeDateTimeEdit::codeChanged);

		addActions(codeLineEdit->actions());
	}

	CodeLineEdit *CodeDateTimeEdit::codeLineEdit() const
	{
		return qobject_cast<CodeLineEdit *>(lineEdit());
	}

	bool CodeDateTimeEdit::isCode() const
	{
		return codeLineEdit()->isCode();
	}

	void CodeDateTimeEdit::setCode(bool code)
	{
		codeLineEdit()->setCode(code);
	}

	void CodeDateTimeEdit::setFromSubParameter(const SubParameter &subParameter)
	{
		setCode(subParameter.isCode());
        codeLineEdit()->setText(subParameter.value());
	}

	void CodeDateTimeEdit::openEditor(int line, int column)
	{
		codeLineEdit()->openEditor(line, column);
	}
	
	void CodeDateTimeEdit::setCompletionModel(QAbstractItemModel *completionModel)
	{
        codeLineEdit()->setCompletionModel(completionModel);
    }

    void CodeDateTimeEdit::setParameterContainer(const ParameterContainer *parameterContainer)
    {
        codeLineEdit()->setParameterContainer(parameterContainer);
    }

    QSet<QString> CodeDateTimeEdit::findVariables() const
    {
        return codeLineEdit()->findVariables();
    }

	void CodeDateTimeEdit::codeChanged(bool code)
	{
		if(!code)
		{
			setCalendarPopup(true);
			setButtonSymbols(QAbstractSpinBox::UpDownArrows);
			setDateTime(QDateTime::currentDateTime());
			setDisplayFormat(QStringLiteral("dd/MM/yyyy hh:mm:ss"));
			codeLineEdit()->textChanged(codeLineEdit()->text());
		}
		else
		{
			setDisplayFormat(QString());
			setCalendarPopup(false);
			setButtonSymbols(QAbstractSpinBox::NoButtons);
		}
	}

	QValidator::State CodeDateTimeEdit::validate(QString &text, int &pos) const
	{
		if(isCode())
			return QValidator::Acceptable;

		return QDateTimeEdit::validate(text, pos);
	}

	QString CodeDateTimeEdit::textFromDateTime(const QDateTime &dateTime) const
	{
		if(isCode())
			return codeLineEdit()->text();

		return QDateTimeEdit::textFromDateTime(dateTime);
	}

	void CodeDateTimeEdit::contextMenuEvent(QContextMenuEvent *event)
	{
		QMenu *menu = codeLineEdit()->createStandardContextMenu();

		menu->addSeparator();

		const uint se = stepEnabled();

		QAction *up = menu->addAction(tr("&Step up"));
		up->setShortcut(QKeySequence(Qt::Key_Up));
		up->setEnabled(se & StepUpEnabled);

		QAction *down = menu->addAction(tr("Step &down"));
		down->setShortcut(QKeySequence(Qt::Key_Down));
		down->setEnabled(se & StepDownEnabled);

		menu->addSeparator();

		codeLineEdit()->addShortcuts(menu);

		const QAction *action = menu->exec(event->globalPos());

		if(action == up)
			stepBy(1);
		else if(action == down)
			stepBy(-1);

		delete menu;

		event->accept();
	}
}
