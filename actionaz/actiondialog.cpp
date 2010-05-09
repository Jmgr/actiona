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

#include "actiondialog.h"
#include "ui_actiondialog.h"
#include "action.h"
#include "script.h"
#include "actioninterface.h"
#include "elementdefinition.h"
#include "groupdefinition.h"
#include "parameterdefinition.h"
#include "abstractcodeeditor.h"
#include "codelineedit.h"

#include <QGroupBox>
#include <QLabel>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>

ActionDialog::ActionDialog(QAbstractItemModel *completionModel, ActionTools::Script *script, ActionTools::Action *action, QWidget *parent)
	: QDialog(parent),
	ui(new Ui::ActionDialog),
        mAction(action),
	mScript(script),
	mCurrentLine(-1),
	mCurrentColumn(-1),
	mCompletionModel(completionModel)
{
	ui->setupUi(this);

	ActionTools::ActionInterface *actionInterface(action->interface());
	const QList<ActionTools::ElementDefinition *> elements(actionInterface->elements());

	ui->actionIcon->setPixmap(actionInterface->icon());
	ui->actionName->setText("<h2>" + actionInterface->name() + "</h2>");
	ui->actionDescription->setText("<i>" + actionInterface->description() + "</i>");

	QString informations;
	QString author = actionInterface->author();
	QString email = actionInterface->email();
	QString website = actionInterface->website();
	Tools::Version version = actionInterface->version();
	ActionTools::ActionInterface::Status status = actionInterface->status();

	if(!author.isEmpty())
	{
		informations = tr("By ");
		if(!email.isEmpty())
			informations += QString("<a href=\"mailto:%1\">%2</a>").arg(email).arg(author);
		else
			informations += author;
	}

	if(!website.isEmpty())
	{
		if(!author.isEmpty())
			informations += " - ";
		informations += QString("<a href=\"http://%1\">%1</a>").arg(website);
	}

	if(!informations.isEmpty())
		informations += "<br/>";

	informations += tr("Version %1").arg(version.toString());

	QString statusString;

	switch(status)
	{
	case ActionTools::ActionInterface::Alpha:	statusString = tr("Alpha"); break;
	case ActionTools::ActionInterface::Beta:	statusString = tr("Beta"); break;
	case ActionTools::ActionInterface::Testing:	statusString = tr("Testing"); break;
	case ActionTools::ActionInterface::Stable:	statusString = tr("Stable"); break;
	}

	informations += QString(" (%1)").arg(statusString);

	ui->actionInfo->setText(informations);

	QFormLayout *parameterLayout;
	foreach(ActionTools::ElementDefinition *element, elements)
	{
		if(ActionTools::ParameterDefinition *currentParameter = qobject_cast<ActionTools::ParameterDefinition *>(element))
		{
			parameterLayout = (currentParameter->category() == ActionTools::ParameterDefinition::INPUT ? ui->inputParametersLayout : ui->outputParametersLayout);
			parameterLayout->addRow(currentParameter->translatedName() + " :", addParameter(currentParameter));
		}
		else if(ActionTools::GroupDefinition *currentGroup = qobject_cast<ActionTools::GroupDefinition *>(element))
			addGroup(currentGroup);
	}

	foreach(ActionTools::ElementDefinition *element, elements)
	{
		if(ActionTools::GroupDefinition *currentGroup = qobject_cast<ActionTools::GroupDefinition *>(element))
			currentGroup->init();
	}

	if(ui->inputParametersLayout->count() == 0)
		ui->inputParametersLayout->addRow(new QLabel(tr("<i><center>No parameters</center></i>"), this));
	if(ui->outputParametersLayout->count() == 0)
		ui->outputParametersLayout->addRow(new QLabel(tr("<i><center>No parameters</center></i>"), this));

	adjustSize();
}

ActionDialog::~ActionDialog()
{
	ActionTools::ActionInterface *actionInterface(mAction->interface());
	const QList<ActionTools::ElementDefinition *> elements(actionInterface->elements());
	foreach(ActionTools::ElementDefinition *element, elements)
	{
		if(ActionTools::GroupDefinition *currentGroup = qobject_cast<ActionTools::GroupDefinition *>(element))
			currentGroup->setGroupBox(0);
	}

	delete ui;
}

void ActionDialog::accept()
{
	foreach(ActionTools::ParameterDefinition *parameter, mParameters)
		parameter->save(mAction);

	QDialog::accept();
}

int ActionDialog::exec()
{
	QTimer::singleShot(1, this, SLOT(postInit()));

	return QDialog::exec();
}

void ActionDialog::postInit()
{
	if(!mCurrentField.isEmpty())
	{
		foreach(ActionTools::ParameterDefinition *parameterDefinition, mParameters)
		{
			if(parameterDefinition->name() == mCurrentField && parameterDefinition->editors().count() > 0)
			{
				QWidget *editorWidget = parameterDefinition->editors().at(0);
				if(!editorWidget)
					continue;

				editorWidget->setFocus();

				if(!mCurrentSubField.isEmpty())
				{
					QString value = mAction->subParameter(mCurrentField, mCurrentSubField).value().toString();
					if(value.contains('\n'))//Multiline : open the editor
					{
						if(ActionTools::AbstractCodeEditor *codeEditor = dynamic_cast<ActionTools::AbstractCodeEditor *>(editorWidget))
							codeEditor->openEditor(mCurrentLine, mCurrentColumn);
					}
				}

				break;
			}
		}
	}
}

void ActionDialog::addGroup(ActionTools::GroupDefinition *group)
{
	QGroupBox *groupBox = new QGroupBox(group->translatedName(), this);
	QFormLayout *groupLayout = new QFormLayout(groupBox);

	groupLayout->setMargin(5);
	groupLayout->setSpacing(5);

	groupBox->setLayout(groupLayout);

	group->setGroupBox(groupBox);

	foreach(ActionTools::ParameterDefinition *parameter, group->members())
	{
		parameter->setCategory(group->category());
		groupLayout->addRow(QString(parameter->translatedName() + " :"), addParameter(parameter));
	}

	QFormLayout *parameterLayout = (group->category() == ActionTools::ParameterDefinition::INPUT ? ui->inputParametersLayout : ui->outputParametersLayout);
	parameterLayout->addRow(groupBox);
}

QLayout *ActionDialog::addParameter(ActionTools::ParameterDefinition *parameter)
{
	QHBoxLayout *layout = new QHBoxLayout;
	layout->setMargin(0);

	parameter->buildEditors(mScript, this);
	foreach(QWidget *editor, parameter->editors())
	{
		if(ActionTools::AbstractCodeEditor *codeEditor = dynamic_cast<ActionTools::AbstractCodeEditor *>(editor))
			codeEditor->setCompletionModel(mCompletionModel);
		
		layout->addWidget(editor);
	}

	parameter->load(mAction);

	mParameters.append(parameter);

	return layout;
}

void ActionDialog::changeEvent(QEvent *event)
{
	QDialog::changeEvent(event);

	switch(event->type())
	{
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
