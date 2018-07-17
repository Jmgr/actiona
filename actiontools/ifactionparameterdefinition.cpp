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

#include "ifactionparameterdefinition.h"
#include "subparameter.h"
#include "codecombobox.h"
#include "linecombobox.h"
#include "codelineedit.h"
#include "actioninstance.h"
#include "script.h"

#include <QApplication>

namespace ActionTools
{
    bool IfActionParameterDefinition::translated = false;

    Tools::StringListPair IfActionParameterDefinition::actions =
    {
        {
            QStringLiteral("do_nothing"),
            QStringLiteral("goto"),
            QStringLiteral("run_code"),
            QStringLiteral("call_procedure")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("IfActionParameterDefinition::actions", "Go to next action")),
            QStringLiteral(QT_TRANSLATE_NOOP("IfActionParameterDefinition::actions", "Go to line")),
            QStringLiteral(QT_TRANSLATE_NOOP("IfActionParameterDefinition::actions", "Run code")),
            QStringLiteral(QT_TRANSLATE_NOOP("IfActionParameterDefinition::actions", "Call procedure"))
        }
    };

    IfActionParameterDefinition::IfActionParameterDefinition(const Name &name, QObject *parent)
        : ItemsParameterDefinition(name, parent),
          mActionEdit(nullptr),
          mLineComboBox(nullptr),
          mCodeLineEdit(nullptr),
          mProcedureComboBox(nullptr),
          mAllowWait(false)
    {
        if(!translated)
        {
            translated = true;

            for(int index = 0; index < actions.second.size(); ++index)
                actions.second[index] = QCoreApplication::translate("IfActionParameterDefinition::actions", actions.second.at(index).toUtf8().constData());
        }
    }

    void IfActionParameterDefinition::buildEditors(Script *script, QWidget *parent)
    {
        ParameterDefinition::buildEditors(script, parent);

        mScript = script;

        setItems(actions);

        if(mAllowWait)
        {
            mItems.first << QStringLiteral("wait");
            mItems.second << tr("Wait");
        }

        mActionEdit = new CodeComboBox(parent);

        mActionEdit->addItems(mItems.second);

        addEditor(mActionEdit);

        mLineComboBox = new LineComboBox(*script, parent);
        mLineComboBox->setVisible(false);

        addEditor(mLineComboBox);

        mCodeLineEdit = new CodeLineEdit(parent);
        mCodeLineEdit->setCode(true);
        mCodeLineEdit->setAllowTextCodeChange(false);
        mCodeLineEdit->setVisible(false);

        addEditor(mCodeLineEdit);

        mProcedureComboBox = new CodeComboBox(parent);
        mProcedureComboBox->setVisible(false);

        addEditor(mProcedureComboBox);

        connect(mActionEdit->codeLineEdit(), &CodeLineEdit::textChanged, this, &IfActionParameterDefinition::textChanged);
        connect(mActionEdit->codeLineEdit(), &CodeLineEdit::codeChanged, this, &IfActionParameterDefinition::codeChanged);
    }

    void IfActionParameterDefinition::load(const ActionInstance *actionInstance)
    {
        const SubParameter &actionSubParameter = actionInstance->subParameter(name().original(), QStringLiteral("action"));
        mActionEdit->setCode(actionSubParameter.isCode());
        mActionEdit->setEditText(translatedNameFromOriginalName(actionSubParameter.value()));

        const SubParameter &lineSubParameter = actionInstance->subParameter(name().original(), QStringLiteral("line"));
        switch(findAppropriateEditor(mActionEdit->codeLineEdit()->text()))
        {
        case LineEditor:
            mLineComboBox->setFromSubParameter(lineSubParameter);
            break;
        case CodeEditor:
            mCodeLineEdit->setText(lineSubParameter.value());
            break;
        case TextCodeEditor:
            mCodeLineEdit->setCode(lineSubParameter.isCode());
            mCodeLineEdit->setText(lineSubParameter.value());
            break;
        case ProcedureEditor:
            mProcedureComboBox->setCode(lineSubParameter.isCode());
            mProcedureComboBox->setEditText(lineSubParameter.value());
            break;
        case NoEditor:
        default:
            break;
        }
    }

    void IfActionParameterDefinition::save(ActionInstance *actionInstance)
    {
        actionInstance->setSubParameter(name().original(), QStringLiteral("action"), mActionEdit->isCode(), originalNameFromTranslatedName(mActionEdit->currentText()));

        //Note: this should not be called "line" each time, but we need to keep this name for backward compatibility
        switch(findAppropriateEditor(mActionEdit->codeLineEdit()->text()))
        {
        case LineEditor:
            actionInstance->setSubParameter(name().original(), QStringLiteral("line"), mLineComboBox->isCode(), mLineComboBox->currentText());
            break;
        case CodeEditor:
            actionInstance->setSubParameter(name().original(), QStringLiteral("line"), true, mCodeLineEdit->text());
            break;
        case TextCodeEditor:
            actionInstance->setSubParameter(name().original(), QStringLiteral("line"), mCodeLineEdit->isCode(), mCodeLineEdit->text());
            break;
        case ProcedureEditor:
            actionInstance->setSubParameter(name().original(), QStringLiteral("line"), mProcedureComboBox->isCode(), mProcedureComboBox->currentText());
            break;
        case NoEditor:
        default:
            break;
        }
    }

    void IfActionParameterDefinition::setDefaultValues(ActionInstance *actionInstance)
    {
        actionInstance->setSubParameter(name().original(), QStringLiteral("action"), defaultAction(actions.second[DoNothing]));
        actionInstance->setSubParameter(name().original(), QStringLiteral("line"), defaultLine());
    }

    void IfActionParameterDefinition::setAllowWait(bool allowWait)
    {
        mAllowWait = allowWait;

        if(allowWait)
            setDefaultAction(QStringLiteral("wait"));
    }

    void IfActionParameterDefinition::actionUpdate(Script *script)
    {
        script->updateLineModel();
        mProcedureComboBox->clear();
        mProcedureComboBox->addItems(script->procedureNames());
    }

    void IfActionParameterDefinition::codeChanged(bool code)
    {
        Q_UNUSED(code);

        updateStatus(mActionEdit->codeLineEdit()->text());
    }

    void IfActionParameterDefinition::textChanged(const QString &text)
    {
        updateStatus(text);
    }

    IfActionParameterDefinition::Editor IfActionParameterDefinition::findAppropriateEditor(const QString &actionText) const
    {
        if(mActionEdit->codeLineEdit()->isCode())
            return TextCodeEditor;

        if(actionText == mItems.first.at(DoNothing) || actionText == mItems.second.at(DoNothing) ||
            (mAllowWait && (actionText == mItems.first.at(Wait) || actionText == mItems.second.at(Wait))))
            return NoEditor;

        if(actionText == mItems.first.at(Goto) || actionText == mItems.second.at(Goto))
            return LineEditor;

        if(actionText == mItems.first.at(RunCode) || actionText == mItems.second.at(RunCode))
            return CodeEditor;

        if(actionText == mItems.first.at(CallProcedure) || actionText == mItems.second.at(CallProcedure))
            return ProcedureEditor;

        return TextCodeEditor;
    }

    void IfActionParameterDefinition::updateStatus(const QString &actionText)
    {
        mLineComboBox->setVisible(false);
        mCodeLineEdit->setVisible(false);
        mProcedureComboBox->setVisible(false);

        switch(findAppropriateEditor(actionText))
        {
        case LineEditor:
            mLineComboBox->setVisible(true);
            mLineComboBox->codeLineEdit()->setAllowTextCodeChange(true);
            break;
        case CodeEditor:
            mCodeLineEdit->setVisible(true);
            mCodeLineEdit->setCode(true);
            mCodeLineEdit->setAllowTextCodeChange(false);
            break;
        case TextCodeEditor:
            mCodeLineEdit->setVisible(true);
            mCodeLineEdit->setAllowTextCodeChange(true);
            break;
        case ProcedureEditor:
            mProcedureComboBox->setVisible(true);
            break;
        case NoEditor:
        default:
            break;
        }
    }
}
