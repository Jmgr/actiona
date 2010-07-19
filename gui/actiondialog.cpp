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
#include "actioninstance.h"
#include "script.h"
#include "actiondefinition.h"
#include "elementdefinition.h"
#include "groupdefinition.h"
#include "parameterdefinition.h"
#include "abstractcodeeditor.h"
#include "codelineedit.h"
#include "linecombobox.h"

#include <QGroupBox>
#include <QLabel>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QComboBox>

ActionDialog::ActionDialog(QAbstractItemModel *completionModel, ActionTools::Script *script, ActionTools::ActionInstance *actionInstance, QWidget *parent)
	: QDialog(parent),
	ui(new Ui::ActionDialog),
	mActionInstance(actionInstance),
	mScript(script),
	mCurrentLine(-1),
	mCurrentColumn(-1),
	mCurrentException(-1),
	mCompletionModel(completionModel),
	mExceptionsLayout(new QGridLayout),
	mTabWidget(new QTabWidget(this)),
	mExceptionsTabWidget(new QWidget)
{
#ifdef ACT_PROFILE
	Tools::HighResolutionTimer timer("ActionDialog creation");
#endif
	ui->setupUi(this);

	const ActionTools::ActionDefinition *actionDefinition(actionInstance->definition());
	const QList<ActionTools::ElementDefinition *> elements(actionDefinition->elements());

	ui->actionIcon->setPixmap(actionDefinition->icon());
	ui->actionName->setText("<h2>" + actionDefinition->name() + "</h2>");
	ui->actionDescription->setText("<i>" + actionDefinition->description() + "</i>");

	QString informations;
	QString author = actionDefinition->author();
	QString email = actionDefinition->email();
	QString website = actionDefinition->website();
	Tools::Version version = actionDefinition->version();
	ActionTools::ActionDefinition::Status status = actionDefinition->status();
	QStringList tabs = actionDefinition->tabs();
	if(tabs.count() == 0)
		tabs << tr("Parameters");

	int tabCount = tabs.count();

	QVector<QGroupBox *> groupBoxes[2];

	for(int parameterType = 0; parameterType < 2; ++parameterType)
	{
		for(int i = 0; i < tabCount; ++i)
			mParameterLayouts[parameterType].append(new QFormLayout);
	}

	int tabIndex = 0;
	foreach(const QString &tab, tabs)
	{
		QWidget *widget = new QWidget;
		QVBoxLayout *layout = new QVBoxLayout(widget);

		QGroupBox *inputParametersGroupBox = new QGroupBox(tr("Input parameters"), widget);
		inputParametersGroupBox->setLayout(mParameterLayouts[InputParameters][tabIndex]);
		groupBoxes[InputParameters].append(inputParametersGroupBox);
		QGroupBox *outputParametersGroupBox = new QGroupBox(tr("Output parameters"), widget);
		outputParametersGroupBox->setLayout(mParameterLayouts[OutputParameters][tabIndex]);
		groupBoxes[OutputParameters].append(outputParametersGroupBox);

		layout->addWidget(inputParametersGroupBox);
		layout->addWidget(outputParametersGroupBox);

		widget->setLayout(layout);

		mParameterTabWidgets.append(widget);
		mTabWidget->addTab(widget, tab);

		++tabIndex;
	}

	ui->parametersLayout->addWidget(mTabWidget);

	QVBoxLayout *layout = new QVBoxLayout;

	QStringList exceptionActionsNames;
	for(int i = 0; i < ActionTools::ActionException::ExceptionActionCount; ++i)
		exceptionActionsNames << ActionTools::ActionException::ExceptionActionName[i];

	const ActionTools::ExceptionActionInstancesHash exceptionActionInstances = mActionInstance->exceptionActionInstances();
	QList<ActionTools::ActionException *> actionExceptions = mActionInstance->definition()->exceptions();

	for(int i = 0, exceptionIndex = 0; i < ActionTools::ActionException::ExceptionCount + actionExceptions.count(); ++i)
	{
		QString exceptionName;
		int exceptionId;

		if(i < ActionTools::ActionException::ExceptionCount)
		{
			exceptionName = ActionTools::ActionException::ExceptionName[i];
			exceptionId = i;
		}
		else
		{
			ActionTools::ActionException *actionException = actionExceptions.at(exceptionIndex);
			exceptionName = actionException->name();
			exceptionId = actionException->id();
			++exceptionIndex;
		}

		QLabel *exceptionNameLabel = new QLabel(exceptionName, this);
		exceptionNameLabel->setProperty("id", exceptionId);
		mExceptionsLayout->addWidget(exceptionNameLabel, i, 0, Qt::AlignLeft);

		ActionTools::ActionException::ExceptionActionInstance exceptionActionInstance = exceptionActionInstances.value(static_cast<ActionTools::ActionException::Exception>(exceptionId));

		QComboBox *actionComboBox = new QComboBox(this);
		actionComboBox->addItems(exceptionActionsNames);
		actionComboBox->setProperty("row", i);
		actionComboBox->setCurrentIndex(exceptionActionInstance.action());

		connect(actionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(currentExceptionActionChanged(int)));

		mExceptionsLayout->addWidget(actionComboBox, i, 1, Qt::AlignCenter);

		ActionTools::LineComboBox *actionLineComboBox = new ActionTools::LineComboBox(mScript->labels(), mScript->actionCount(), this);
		actionLineComboBox->codeLineEdit()->setAllowTextCodeChange(false);
		actionLineComboBox->codeLineEdit()->setShowEditorButton(false);
		actionLineComboBox->codeLineEdit()->setText(exceptionActionInstance.line());
		actionLineComboBox->setEnabled(exceptionActionInstance.action() == ActionTools::ActionException::GotoLineExceptionAction);

		mExceptionsLayout->addWidget(actionLineComboBox, i, 2, Qt::AlignCenter);
	}

	layout->addLayout(mExceptionsLayout);
	layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
	mExceptionsTabWidget->setLayout(layout);

	mTabWidget->addTab(mExceptionsTabWidget, tr("Exceptions"));

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
	case ActionTools::ActionDefinition::Alpha:		statusString = tr("Alpha"); break;
	case ActionTools::ActionDefinition::Beta:		statusString = tr("Beta"); break;
	case ActionTools::ActionDefinition::Testing:	statusString = tr("Testing"); break;
	case ActionTools::ActionDefinition::Stable:		statusString = tr("Stable"); break;
	}

	informations += QString(" (%1)").arg(statusString);

	ui->actionInfo->setText(informations);

	QFormLayout *parameterLayout;
	foreach(ActionTools::ElementDefinition *element, elements)
	{
		if(ActionTools::ParameterDefinition *currentParameter = qobject_cast<ActionTools::ParameterDefinition *>(element))
		{
			int parameterType = (currentParameter->category() == ActionTools::ParameterDefinition::INPUT ? InputParameters : OutputParameters);

			parameterLayout = mParameterLayouts[parameterType][currentParameter->tab()];

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

	for(int parameterType = 0; parameterType < 2; ++parameterType)
	{
		for(int i = 0; i < tabCount; ++i)
		{
			if(mParameterLayouts[parameterType].at(i)->count() == 0)
				groupBoxes[parameterType][i]->hide();
		}
	}

	adjustSize();
}

ActionDialog::~ActionDialog()
{
	const ActionTools::ActionDefinition *actionDefinition(mActionInstance->definition());
	const QList<ActionTools::ElementDefinition *> elements(actionDefinition->elements());
	foreach(ActionTools::ElementDefinition *element, elements)
	{
		if(ActionTools::GroupDefinition *currentGroup = qobject_cast<ActionTools::GroupDefinition *>(element))
			currentGroup->setGroupBox(0);
	}

	delete ui;
}

void ActionDialog::accept()
{
#ifdef ACT_PROFILE
	Tools::HighResolutionTimer timer("ActionDialog accept");
#endif
	foreach(ActionTools::ParameterDefinition *parameter, mParameters)
		parameter->save(mActionInstance);

	for(int i = 0; i < mExceptionsLayout->rowCount(); ++i)
	{
		QLabel *exceptionNameLabel = qobject_cast<QLabel *>(mExceptionsLayout->itemAtPosition(i, 0)->widget());
		QComboBox *exceptionActionComboBox = qobject_cast<QComboBox *>(mExceptionsLayout->itemAtPosition(i, 1)->widget());
		ActionTools::LineComboBox *lineComboBox = qobject_cast<ActionTools::LineComboBox *>(mExceptionsLayout->itemAtPosition(i, 2)->widget());
		ActionTools::ActionException::Exception exception = static_cast<ActionTools::ActionException::Exception>(exceptionNameLabel->property("id").toInt());
		ActionTools::ActionException::ExceptionAction exceptionAction = static_cast<ActionTools::ActionException::ExceptionAction>(exceptionActionComboBox->currentIndex());

		mActionInstance->setExceptionActionInstance(exception,
													ActionTools::ActionException::ExceptionActionInstance(exceptionAction,
																										  lineComboBox->currentText()));
	}

	QDialog::accept();
}

int ActionDialog::exec()
{
	QTimer::singleShot(1, this, SLOT(postInit()));

	return QDialog::exec();
}

void ActionDialog::postInit()
{
#ifdef ACT_PROFILE
	Tools::HighResolutionTimer timer("ActionDialog postInit");
#endif
	if(!mCurrentField.isEmpty())
	{
		foreach(ActionTools::ParameterDefinition *parameterDefinition, mParameters)
		{
			if(parameterDefinition->name() == mCurrentField && parameterDefinition->editors().count() > 0)
			{
				QWidget *editorWidget = parameterDefinition->editors().at(0);
				if(!editorWidget)
					continue;

				mTabWidget->setCurrentWidget(mParameterTabWidgets.at(parameterDefinition->tab()));
				editorWidget->setFocus();

				if(!mCurrentSubField.isEmpty())
				{
					QString value = mActionInstance->subParameter(mCurrentField, mCurrentSubField).value().toString();
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
	if(mCurrentException != -1)
	{
		for(int i = 0; i < mExceptionsLayout->rowCount(); ++i)
		{
			QLabel *exceptionNameLabel = qobject_cast<QLabel *>(mExceptionsLayout->itemAtPosition(i, 0)->widget());
			ActionTools::LineComboBox *lineComboBox = qobject_cast<ActionTools::LineComboBox *>(mExceptionsLayout->itemAtPosition(i, 2)->widget());
			ActionTools::ActionException::Exception exception = static_cast<ActionTools::ActionException::Exception>(exceptionNameLabel->property("id").toInt());

			if(exception == mCurrentException)
			{
				lineComboBox->setFocus();
				mTabWidget->setCurrentWidget(mExceptionsTabWidget);
				break;
			}
		}
	}
}

void ActionDialog::currentExceptionActionChanged(int index)
{
	QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
	if(!comboBox)
		return;

	ActionTools::ActionException::ExceptionAction exceptionAction = static_cast<ActionTools::ActionException::ExceptionAction>(index);
	int row = comboBox->property("row").toInt();
	QLayoutItem *item = mExceptionsLayout->itemAtPosition(row, 2);
	if(!item)
		return;

	QWidget *widget = item->widget();
	if(!widget)
		return;

	widget->setEnabled(exceptionAction == ActionTools::ActionException::GotoLineExceptionAction);
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

	int parameterType = (group->category() == ActionTools::ParameterDefinition::INPUT ? InputParameters : OutputParameters);
	QFormLayout *parameterLayout = mParameterLayouts[parameterType][group->tab()];
	parameterLayout->addRow(groupBox);
}

QLayout *ActionDialog::addParameter(ActionTools::ParameterDefinition *parameter)
{
	QBoxLayout *layout = new QBoxLayout(parameter->editorsOrientation() == Qt::Horizontal ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom );
	layout->setMargin(0);

	parameter->buildEditors(mScript, this);
	foreach(QWidget *editor, parameter->editors())
	{
		if(ActionTools::AbstractCodeEditor *codeEditor = dynamic_cast<ActionTools::AbstractCodeEditor *>(editor))
			codeEditor->setCompletionModel(mCompletionModel);

		layout->addWidget(editor);
	}

	parameter->load(mActionInstance);

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
