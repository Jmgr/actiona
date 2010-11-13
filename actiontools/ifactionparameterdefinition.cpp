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

	StringListPair IfActionParameterDefinition::actions = qMakePair(
		QStringList() << "do_nothing" << "goto",
		QStringList() << QT_TRANSLATE_NOOP("IfActionParameterDefinition::actions", "Do nothing") << QT_TRANSLATE_NOOP("IfActionParameterDefinition::actions", "Goto line"));

	IfActionParameterDefinition::IfActionParameterDefinition(const QString &name, const QString &translatedName, QObject *parent)
		: ItemsParameterDefinition(name, translatedName, parent),
		mActionEdit(0),
		mLineEdit(0),
		mAllowWait(false)
	{
		if(!translated)
		{
			translated = true;

			for(int index = 0; index < actions.second.size(); ++index)
				actions.second[index] = QApplication::instance()->translate("IfActionParameterDefinition::actions", actions.second.at(index).toLatin1());
		}
	}

	void IfActionParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		setItems(actions);

		if(mAllowWait)
		{
			mItems.first << "wait";
			mItems.second << QObject::tr("Wait");
		}

		mActionEdit = new CodeComboBox(parent);

		mActionEdit->addItems(mItems.second);

		addEditor(mActionEdit);

		mLineEdit = new LineComboBox(script->labels(), script->actionCount(), parent);

		mLineEdit->setEnabled(false);

		addEditor(mLineEdit);

		connect(mActionEdit->codeLineEdit(), SIGNAL(textChanged(const QString &)), this, SLOT(textChanged(const QString &)));
		connect(mActionEdit->codeLineEdit(), SIGNAL(codeChanged(bool)), this, SLOT(codeChanged(bool)));
	}

	void IfActionParameterDefinition::load(const ActionInstance *actionInstance)
	{
		const SubParameter &actionSubParameter = actionInstance->subParameter(name(), "action");
		mActionEdit->setCode(actionSubParameter.isCode());
		mActionEdit->setEditText(translatedNameFromOriginalName(actionSubParameter.value().toString()));

		const SubParameter &lineSubParameter = actionInstance->subParameter(name(), "line");
		mLineEdit->setCode(lineSubParameter.isCode());
		mLineEdit->setEditText(lineSubParameter.value().toString());
	}

	void IfActionParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name(), "action", mActionEdit->isCode(), originalNameFromTranslatedName(mActionEdit->currentText()));
		actionInstance->setSubParameter(name(), "line", mLineEdit->isCode(), mLineEdit->currentText());
	}

	void IfActionParameterDefinition::setDefaultValues(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name(), "action", defaultAction(actions.second[DoNothing]));
		actionInstance->setSubParameter(name(), "line", defaultLine());
	}
	
	void IfActionParameterDefinition::update(Script *script)
	{
		mLineEdit->setup(script->labels(), script->actionCount());
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

	void IfActionParameterDefinition::updateStatus(const QString &text)
	{
		if(mActionEdit->codeLineEdit()->isCode())
		{
			mLineEdit->setEnabled(true);
			return;
		}

		if(text == mItems.first.at(DoNothing) ||
			text == mItems.second.at(DoNothing) ||
			(mAllowWait && (text == mItems.first.at(Wait) ||
			text == mItems.second.at(Wait))))
		{
			mLineEdit->setEditText(QString());
			mLineEdit->setEnabled(false);
		}
		else
			mLineEdit->setEnabled(true);
	}
}
