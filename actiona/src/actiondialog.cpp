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

#include "actiondialog.hpp"
#include "ui_actiondialog.h"
#include "actiontools/actioninstance.hpp"
#include "actiontools/script.hpp"
#include "actiontools/actiondefinition.hpp"
#include "actiontools/elementdefinition.hpp"
#include "actiontools/groupdefinition.hpp"
#include "actiontools/parameterdefinition.hpp"
#include "actiontools/abstractcodeeditor.hpp"
#include "actiontools/codelineedit.hpp"
#include "actiontools/linecombobox.hpp"
#include "actiontools/helpbutton.hpp"

#ifdef ACT_PROFILE
#include "tools/highresolutiontimer.hpp"
#endif

#include <QGroupBox>
#include <QLabel>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QMenu>

#include <limits>
#include <algorithm>

ActionDialog::ActionDialog(QAbstractItemModel *completionModel, ActionTools::Script *script, const ActionTools::ActionDefinition *actionDefinition, const QString &localeName, QWidget *parent)
	: QDialog(parent),
      ParameterContainer(script),
	  ui(new Ui::ActionDialog),
	  mActionInstance(nullptr),
	  mScript(script),
	  mCurrentLine(-1),
	  mCurrentColumn(-1),
	  mCurrentException(-1),
	  mCompletionModel(completionModel),
	  mExceptionsLayout(new QGridLayout),
	  mTabWidget(new QTabWidget(this)),
	  mExceptionsTabWidget(new QWidget(mTabWidget)),
	  mCommonTabWidget(new QWidget(mTabWidget)),
	  mPauseBeforeSpinBox(new QSpinBox(this)),
	  mPauseAfterSpinBox(new QSpinBox(this)),
	  mTimeoutSpinBox(new QSpinBox(this))
{
#ifdef ACT_PROFILE
    Tools::HighResolutionTimer timer(QStringLiteral("ActionDialog creation ") + actionDefinition->id());
#endif
	ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);

	//Init of texts & images
    ui->actionIcon->setPixmap(actionDefinition->cachedIcon());
	ui->actionName->setText(QStringLiteral("<h2>") + actionDefinition->name() + QStringLiteral("</h2>"));
	ui->actionDescription->setText(QStringLiteral("<i>") + actionDefinition->description() + QStringLiteral("</i>"));
	ui->helpPushButton->setTopic(QStringLiteral("%1:actions:%2").arg(localeName.left(2)).arg(actionDefinition->id().toLower()));

	bool worksUnderThisOS = actionDefinition->worksUnderThisOS();
	ui->actionOSAvailability->setVisible(!worksUnderThisOS);
	ui->actionOSAvailabilityIcon->setVisible(!worksUnderThisOS);

	//Init of tabs & group boxes
	QStringList tabs = actionDefinition->tabs();
    if(tabs.count() == 0)
        tabs << QStringLiteral(QT_TRANSLATE_NOOP("ActionTabs", "Parameters"));

    int tabCount = tabs.count();

    QVector<QGroupBox *> groupBoxes[2];

    for(auto &parameterLayout: mParameterLayouts)
    {
        for(int i = 0; i < tabCount; ++i)
            parameterLayout.append(new QFormLayout);
    }

    int tabIndex = 0;
    for(const QString &tab: qAsConst(tabs))
    {
        QWidget *widget = new QWidget;
        auto layout = new QVBoxLayout(widget);

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
        mTabWidget->addTab(widget, QApplication::translate("ActionTabs", tab.toUtf8().constData()));

        ++tabIndex;
    }

	ui->parametersLayout->addWidget(mTabWidget);
	
	//Init of common parameters
	auto commonParametersLayout = new QVBoxLayout;
	QGroupBox *inputCommonParametersGroupBox = new QGroupBox(tr("Input parameters"), mCommonTabWidget);
	auto inputCommonParametersLayout = new QFormLayout;
	
	mPauseBeforeSpinBox->setToolTip(tr("Pause before executing the action"));
	mPauseAfterSpinBox->setToolTip(tr("Pause after executing the action"));
	mTimeoutSpinBox->setToolTip(tr("Action maximal execution time"));
	
	mPauseBeforeSpinBox->setSingleStep(100);
	mPauseAfterSpinBox->setSingleStep(100);
	mTimeoutSpinBox->setSingleStep(100);
	
	mPauseBeforeSpinBox->setMaximum(std::numeric_limits<int>::max());
	mPauseAfterSpinBox->setMaximum(std::numeric_limits<int>::max());
	mTimeoutSpinBox->setMaximum(std::numeric_limits<int>::max());
	
	mPauseBeforeSpinBox->setSuffix(tr(" ms", "milliseconds"));
	mPauseAfterSpinBox->setSuffix(tr(" ms", "milliseconds"));
	mTimeoutSpinBox->setSuffix(tr(" ms", "milliseconds"));
	
	mPauseBeforeSpinBox->setSpecialValueText(tr("No pause before"));
	mPauseAfterSpinBox->setSpecialValueText(tr("No pause after"));
	mTimeoutSpinBox->setSpecialValueText(tr("No timeout"));
	
	inputCommonParametersLayout->addRow(tr("Pause before:"), mPauseBeforeSpinBox);
	inputCommonParametersLayout->addRow(tr("Pause after:"), mPauseAfterSpinBox);
	inputCommonParametersLayout->addRow(tr("Timeout:"), mTimeoutSpinBox);
	
	inputCommonParametersGroupBox->setLayout(inputCommonParametersLayout);
	commonParametersLayout->addWidget(inputCommonParametersGroupBox);
	mCommonTabWidget->setLayout(commonParametersLayout);
	mTabWidget->addTab(mCommonTabWidget, tr("Common"));
	
	//Init of exceptions
	const QList<ActionTools::ElementDefinition *> elements(actionDefinition->elements());

	if(!elements.empty())
	{
		QStringList exceptionActionsNames;
        for(const auto &exceptionActionName: ActionTools::ActionException::ExceptionActionName)
            exceptionActionsNames << QApplication::translate("ActionException::ExceptionActionName", exceptionActionName.toLatin1().constData());

		QList<ActionTools::ActionException *> actionExceptions = actionDefinition->exceptions();

		for(int i = 0, exceptionIndex = 0; i < ActionTools::ActionException::ExceptionCount + actionExceptions.count(); ++i)
		{
			QString exceptionName;
			int exceptionId;

			if(i < ActionTools::ActionException::ExceptionCount)
			{
				exceptionName = QApplication::translate("ActionException::ExceptionName", ActionTools::ActionException::ExceptionName[i].toLatin1().constData());
				exceptionId = i;
			}
			else
			{
				ActionTools::ActionException *actionException = actionExceptions.at(exceptionIndex);
				exceptionName = actionException->name();
				exceptionId = actionException->id();
				++exceptionIndex;
			}

			QLabel *exceptionNameLabel = new QLabel(exceptionName + tr(":"), this);
			exceptionNameLabel->setProperty("id", exceptionId);
			mExceptionsLayout->addWidget(exceptionNameLabel, i, 0, Qt::AlignLeft);

			auto actionComboBox = new QComboBox(this);
			actionComboBox->addItems(exceptionActionsNames);
			actionComboBox->setProperty("row", i);

			connect(actionComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ActionDialog::currentExceptionActionChanged);

			mExceptionsLayout->addWidget(actionComboBox, i, 1, Qt::AlignCenter);

            ActionTools::LineComboBox *actionLineComboBox = new ActionTools::LineComboBox(*mScript, this);
			actionLineComboBox->codeLineEdit()->setAllowTextCodeChange(false);
			actionLineComboBox->codeLineEdit()->setShowEditorButton(false);

			mExceptionsLayout->addWidget(actionLineComboBox, i, 2, Qt::AlignCenter);
		}

		auto layout = new QVBoxLayout;
		layout->addLayout(mExceptionsLayout);
		layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
		mExceptionsTabWidget->setLayout(layout);

		mTabWidget->addTab(mExceptionsTabWidget, tr("Exceptions"));
	}
    else
        delete mExceptionsLayout;

	//Init of parameters
	for(ActionTools::ElementDefinition *element: elements)
	{
		if(auto currentParameter = qobject_cast<ActionTools::ParameterDefinition *>(element))
			addParameter(currentParameter, currentParameter->tab());
		else if(auto currentGroup = qobject_cast<ActionTools::GroupDefinition *>(element))
		{
            const auto parameters = currentGroup->members();
            for(ActionTools::ParameterDefinition *parameter: parameters)
				addParameter(parameter, currentGroup->tab());
		}
	}

	for(ActionTools::ElementDefinition *element: elements)
	{
		if(auto currentGroup = qobject_cast<ActionTools::GroupDefinition *>(element))
			currentGroup->init();
	}

	for(int i = 0; i < tabCount; ++i)
	{
		int parameterCount = 0;
		
		for(int parameterType = 0; parameterType < 2; ++parameterType)
		{
			if(mParameterLayouts[parameterType].at(i)->count() == 0)
				groupBoxes[parameterType][i]->hide();
			else
				++parameterCount;
		}

        if(parameterCount == 0)
            mParameterTabWidgets[i]->layout()->addWidget(new QLabel(tr("<i>No parameters</i>"), mParameterTabWidgets[i]));
	}

	adjustSize();
}

ActionDialog::~ActionDialog()
{
    delete ui;
}

QMenu *ActionDialog::createVariablesMenu(QWidget *parent) const
{
    QSet<QString> thisActionsVariables;
    for(ActionTools::ParameterDefinition *parameter: mParameters)
    {
        for(QWidget *editor: parameter->editors())
        {
            if(auto codeEditor = dynamic_cast<ActionTools::AbstractCodeEditor *>(editor))
                thisActionsVariables.unite(codeEditor->findVariables());
        }
    }

    QStringList variableList = thisActionsVariables.unite(mOtherActionsVariables).values();
    std::sort(variableList.begin(), variableList.end());

    if(variableList.isEmpty())
        return nullptr;

    auto back = new QMenu(parent);

    for(const QString &variable: qAsConst(variableList))
        back->addAction(variable);

    return back;
}

void ActionDialog::accept()
{
#ifdef ACT_PROFILE
    Tools::HighResolutionTimer timer(QStringLiteral("ActionDialog accept"));
#endif
	
	for(ActionTools::ParameterDefinition *parameter: qAsConst(mParameters))
		parameter->save(mActionInstance);
	
	if(!mParameters.empty())
	{
		for(int i = 0; i < mExceptionsLayout->rowCount(); ++i)
		{
			auto exceptionNameLabel = qobject_cast<QLabel *>(mExceptionsLayout->itemAtPosition(i, 0)->widget());
			auto exceptionActionComboBox = qobject_cast<QComboBox *>(mExceptionsLayout->itemAtPosition(i, 1)->widget());
			auto lineComboBox = qobject_cast<ActionTools::LineComboBox *>(mExceptionsLayout->itemAtPosition(i, 2)->widget());
			ActionTools::ActionException::Exception exception = static_cast<ActionTools::ActionException::Exception>(exceptionNameLabel->property("id").toInt());
			auto exceptionAction = static_cast<ActionTools::ActionException::ExceptionAction>(exceptionActionComboBox->currentIndex());

			mActionInstance->setExceptionActionInstance(exception,
														ActionTools::ActionException::ExceptionActionInstance(exceptionAction,
																											  lineComboBox->currentText()));
		}
	}
	
	mActionInstance->setPauseBefore(mPauseBeforeSpinBox->value());
	mActionInstance->setPauseAfter(mPauseAfterSpinBox->value());
	mActionInstance->setTimeout(mTimeoutSpinBox->value());

	QDialog::accept();
}

int ActionDialog::exec(ActionTools::ActionInstance *actionInstance, const QString &field, const QString &subField, int currentLine, int currentColumn)
{
	QTimer::singleShot(1, this, SLOT(postInit()));
	
	mActionInstance = actionInstance;
	mCurrentField = field;
	mCurrentSubField = subField;
	mCurrentLine = currentLine;
    mCurrentColumn = currentColumn;

    ui->buttonBox->button(QDialogButtonBox::Ok)->setFocus();

	return QDialog::exec();
}

int ActionDialog::exec(ActionTools::ActionInstance *actionInstance, int exception)
{
	QTimer::singleShot(1, this, SLOT(postInit()));
	
	mActionInstance = actionInstance;
    mCurrentException = exception;

    ui->buttonBox->button(QDialogButtonBox::Ok)->setFocus();
	
	return QDialog::exec();
}

void ActionDialog::postInit()
{
#ifdef ACT_PROFILE
    Tools::HighResolutionTimer timer(QStringLiteral("ActionDialog postInit"));
#endif
    mOtherActionsVariables = mScript->findVariables(nullptr, mActionInstance);//Find in all actions except this one

	for(ActionTools::ParameterDefinition *parameter: qAsConst(mParameters))
	{
        parameter->actionUpdate(mScript);
		parameter->load(mActionInstance);
	}

    mScript->updateLineModel();

	if(!mParameters.empty())
	{
		const ActionTools::ExceptionActionInstancesHash exceptionActionInstances = mActionInstance->exceptionActionInstances();
		QList<ActionTools::ActionException *> actionExceptions = mActionInstance->definition()->exceptions();

		for(int i = 0, exceptionIndex = 0; i < ActionTools::ActionException::ExceptionCount + actionExceptions.count(); ++i)
		{
			int exceptionId;

			if(i < ActionTools::ActionException::ExceptionCount)
				exceptionId = i;
			else
			{
				ActionTools::ActionException *actionException = actionExceptions.at(exceptionIndex);
				exceptionId = actionException->id();
				++exceptionIndex;
			}

			ActionTools::ActionException::ExceptionActionInstance exceptionActionInstance = exceptionActionInstances.value(static_cast<ActionTools::ActionException::Exception>(exceptionId));

			auto exceptionActionComboBox = qobject_cast<QComboBox *>(mExceptionsLayout->itemAtPosition(i, 1)->widget());
			auto lineComboBox = qobject_cast<ActionTools::LineComboBox *>(mExceptionsLayout->itemAtPosition(i, 2)->widget());

            lineComboBox->setCompletionModel(mCompletionModel);
            lineComboBox->setParameterContainer(this);

			exceptionActionComboBox->setCurrentIndex(exceptionActionInstance.action());
            lineComboBox->setValue(lineComboBox->isCode(), exceptionActionInstance.line());
			lineComboBox->setEnabled(exceptionActionInstance.action() == ActionTools::ActionException::GotoLineExceptionAction);
		}
	}
	
	mPauseBeforeSpinBox->setValue(mActionInstance->pauseBefore());
	mPauseAfterSpinBox->setValue(mActionInstance->pauseAfter());
	mTimeoutSpinBox->setValue(mActionInstance->timeout());
	
	if(!mCurrentField.isEmpty())
	{
		for(ActionTools::ParameterDefinition *parameterDefinition: qAsConst(mParameters))
		{
			if(parameterDefinition->name().original() == mCurrentField && parameterDefinition->editors().count() > 0)
			{
				QWidget *editorWidget = parameterDefinition->editors().at(0);
				if(!editorWidget)
					continue;

				mTabWidget->setCurrentWidget(mParameterTabWidgets.at(parameterDefinition->tab()));
				editorWidget->setFocus();

				if(!mCurrentSubField.isEmpty())
				{
                    QString value = mActionInstance->subParameter(mCurrentField, mCurrentSubField).value();
					if(value.contains(QLatin1Char('\n')))//Multiline : open the editor
					{
						if(auto codeEditor = dynamic_cast<ActionTools::AbstractCodeEditor *>(editorWidget))
							codeEditor->openEditor(mCurrentLine, mCurrentColumn);
					}
				}

				break;
			}
		}
	}

	if(!mParameters.empty())
	{
		for(int i = 0; i < mExceptionsLayout->rowCount(); ++i)
		{
			auto exceptionNameLabel = qobject_cast<QLabel *>(mExceptionsLayout->itemAtPosition(i, 0)->widget());
			auto lineComboBox = qobject_cast<ActionTools::LineComboBox *>(mExceptionsLayout->itemAtPosition(i, 2)->widget());
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
	auto comboBox = qobject_cast<QComboBox *>(sender());
	if(!comboBox)
		return;

	auto exceptionAction = static_cast<ActionTools::ActionException::ExceptionAction>(index);
	int row = comboBox->property("row").toInt();
	QLayoutItem *item = mExceptionsLayout->itemAtPosition(row, 2);
	if(!item)
		return;

	QWidget *widget = item->widget();
	if(!widget)
		return;

    widget->setEnabled(exceptionAction == ActionTools::ActionException::GotoLineExceptionAction);
}

void ActionDialog::addParameter(ActionTools::ParameterDefinition *parameter, int tab)
{
#ifdef Q_OS_LINUX
	if(!(parameter->operatingSystems() & ActionTools::WorksOnGnuLinux))
		return;
#endif
#ifdef Q_OS_WIN
	if(!(parameter->operatingSystems() & ActionTools::WorksOnWindows))
		return;
#endif
#ifdef Q_OS_MAC
	if(!(parameter->operatingSystems() & ActionTools::WorksOnMac))
		return;
#endif

	auto layout = new QBoxLayout(parameter->editorsOrientation() == Qt::Horizontal ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom );
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(1);

	QWidget *parentWidget = new QWidget(this);
	parentWidget->setLayout(layout);

	parameter->buildEditors(mScript, parentWidget);
	for(QWidget *editor: parameter->editors())
	{
		if(auto codeEditor = dynamic_cast<ActionTools::AbstractCodeEditor *>(editor))
        {
			codeEditor->setCompletionModel(mCompletionModel);
            codeEditor->setParameterContainer(this);
        }

		layout->addWidget(editor);
	}

	mParameters.append(parameter);

	int parameterType = (parameter->category() == ActionTools::ParameterDefinition::INPUT ? InputParameters : OutputParameters);

	QFormLayout *parameterLayout = mParameterLayouts[parameterType][tab];
	parameterLayout->addRow(parameter->name().translated() + tr(":"), parentWidget);
}

