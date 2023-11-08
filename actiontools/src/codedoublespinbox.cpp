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

#include "actiontools/codedoublespinbox.hpp"
#include "actiontools/codelineedit.hpp"

#include <QMenu>
#include <QContextMenuEvent>
#include <QStyleOptionSpinBox>
#include <QDebug>

namespace ActionTools
{
    CodeDoubleSpinBox::CodeDoubleSpinBox(QWidget *parent)
    : QDoubleSpinBox(parent)
	{
		CodeLineEdit *codeLineEdit = new CodeLineEdit(parent);
		codeLineEdit->setEmbedded(true);
		setLineEdit(codeLineEdit);

        connect(codeLineEdit, &CodeLineEdit::codeChanged, this, &CodeDoubleSpinBox::onCodeChanged);

		addActions(codeLineEdit->actions());
	}

    CodeLineEdit *CodeDoubleSpinBox::codeLineEdit() const
	{
		return qobject_cast<CodeLineEdit *>(lineEdit());
	}

    bool CodeDoubleSpinBox::isCode() const
	{
		return codeLineEdit()->isCode();
	}

    void CodeDoubleSpinBox::setCode(bool code)
	{
		codeLineEdit()->setCode(code);
	}

    void CodeDoubleSpinBox::setFromSubParameter(const SubParameter &subParameter)
	{
		setCode(subParameter.isCode());
        codeLineEdit()->setText(subParameter.value());
	}

    void CodeDoubleSpinBox::openEditor(int line, int column)
	{
		codeLineEdit()->openEditor(line, column);
	}
	
    void CodeDoubleSpinBox::setCompletionModel(QAbstractItemModel *completionModel)
	{
        codeLineEdit()->setCompletionModel(completionModel);
    }

    void CodeDoubleSpinBox::setParameterContainer(const ParameterContainer *parameterContainer)
    {
        codeLineEdit()->setParameterContainer(parameterContainer);
    }

    QSet<QString> CodeDoubleSpinBox::findVariables() const
    {
        return codeLineEdit()->findVariables();
    }

    QString CodeDoubleSpinBox::text()
	{
		QString currentPrefix = prefix();
		QString currentSuffix = suffix();

		setPrefix(QString());
		setSuffix(QString());

		QString value = codeLineEdit()->text();

		setPrefix(currentPrefix);
		setSuffix(currentSuffix);

		return value;
	}

    void CodeDoubleSpinBox::onCodeChanged(bool code)
	{
		if(!code)
		{
			if(mPrefix.isEmpty())
				mPrefix = prefix();
			if(mSuffix.isEmpty())
				mSuffix = suffix();

			setPrefix(mPrefix);
			setSuffix(mSuffix);
			
			setButtonSymbols(QAbstractSpinBox::UpDownArrows);
			setValue(text().toInt());
			codeLineEdit()->textChanged(codeLineEdit()->text());
		}
		else
		{
			setButtonSymbols(QAbstractSpinBox::NoButtons);
			
			mPrefix = prefix();
			mSuffix = suffix();
			
			int previousWidth = width();

			setPrefix(QString());
			setSuffix(QString());
			
			setMinimumWidth(previousWidth);//This is a hack to fix widget size under KDE, but a better fix has to be found
		}
	}

    QValidator::State CodeDoubleSpinBox::validate(QString &text, int &pos) const
	{
		if(isCode())
			return QValidator::Acceptable;

        return QDoubleSpinBox::validate(text, pos);
	}

    QString CodeDoubleSpinBox::textFromValue(double value) const
	{
		if(isCode())
			return codeLineEdit()->text();

        return QDoubleSpinBox::textFromValue(value);
	}

    void CodeDoubleSpinBox::contextMenuEvent(QContextMenuEvent *event)
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
