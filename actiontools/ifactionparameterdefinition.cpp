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
#include "action.h"
#include "script.h"

namespace ActionTools
{
	StringListPair IfActionParameterDefinition::actions = qMakePair(
		QStringList() << "do_nothing" << "goto",
		QStringList() << tr("Do nothing") << tr("Goto line"));

	IfActionParameterDefinition::IfActionParameterDefinition(Category category, const QString &name, const QString &translatedName, QObject *parent)
		: ItemsParameterDefinition(category, name, translatedName, parent),
		mActionEdit(0),
		mLineEdit(0),
		mAllowWait(false)
	{
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
		mActionEdit->setObjectName("action");

		mActionEdit->addItems(mItems.second);

		addEditor(mActionEdit);

		mLineEdit = new LineComboBox(script->labels(), script->actionCount(), parent);
		mLineEdit->setObjectName("line");

		mLineEdit->setEnabled(false);

		addEditor(mLineEdit);

		connect(mActionEdit->codeLineEdit(), SIGNAL(textChanged(const QString &)), this, SLOT(textChanged(const QString &)));
		connect(mActionEdit->codeLineEdit(), SIGNAL(codeChanged(bool)), this, SLOT(codeChanged(bool)));
	}

	void IfActionParameterDefinition::load(const Action *action)
	{
		const SubParameter &actionSubParameter = action->subParameter(name(), "action");
		mActionEdit->setCode(actionSubParameter.isCode());
		mActionEdit->setEditText(translatedNameFromOriginalName(actionSubParameter.value().toString()));

		const SubParameter &lineSubParameter = action->subParameter(name(), "line");
		mLineEdit->setCode(lineSubParameter.isCode());
		mLineEdit->setEditText(lineSubParameter.value().toString());
	}

	void IfActionParameterDefinition::save(Action *action)
	{
		action->setSubParameter(name(), "action", mActionEdit->isCode(), originalNameFromTranslatedName(mActionEdit->currentText()));
		action->setSubParameter(name(), "line", mLineEdit->isCode(), mLineEdit->currentText());
	}

	void IfActionParameterDefinition::setDefaultValues(Parameter &data)
	{
		data.subParameters()["action"].setValue(option("defaultAction", actions.second[DoNothing]));
		data.subParameters()["line"].setValue(option("defaultLine"));
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
