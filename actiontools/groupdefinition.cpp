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

#include "groupdefinition.h"
#include "listparameterdefinition.h"
#include "codecombobox.h"
#include "codelineedit.h"

#include <QGroupBox>
#include <QDebug>

namespace ActionTools
{
	GroupDefinition::GroupDefinition(const QString &name, const QString &translatedName, QObject *parent)
		: ElementDefinition(name, translatedName, parent),
		mMasterList(0),
		mGroupBox(0),
		mMasterCodeComboBox(0)
	{
	}

	void GroupDefinition::setMasterList(ListParameterDefinition *masterList)
	{
		disconnect();

		connect(masterList, SIGNAL(editorBuilt()), this, SLOT(masterEditorBuilt()));

		mMasterList = masterList;
	}

	void GroupDefinition::init()
	{
		masterTextChanged(mMasterCodeComboBox->currentText());
		masterCodeChanged(mMasterCodeComboBox->codeLineEdit()->isCode());
	}
	
	void GroupDefinition::setDefaultValues(ActionInstance *actionInstance)
	{
		foreach(ParameterDefinition *parameterDefinition, members())
			parameterDefinition->setDefaultValues(actionInstance);
	}

	void GroupDefinition::masterEditorBuilt()
	{
		mMasterCodeComboBox = mMasterList->codeComboBox();

		connect(mMasterCodeComboBox, SIGNAL(editTextChanged(QString)), this, SLOT(masterTextChanged(QString)));
		connect(mMasterCodeComboBox, SIGNAL(codeChanged(bool)), this, SLOT(masterCodeChanged(bool)));
	}

	void GroupDefinition::masterTextChanged(const QString &text)
	{
		if(!mGroupBox)
			return;

		if(!mMasterCodeComboBox->isCode())
			mGroupBox->setEnabled(mMasterValues.contains(mMasterList->originalNameFromTranslatedName(text)));
	}

	void GroupDefinition::masterCodeChanged(bool code)
	{
		if(!mGroupBox)
			return;

		if(code)
			mGroupBox->setEnabled(true);
		else
			mGroupBox->setEnabled(mMasterValues.contains(mMasterList->originalNameFromTranslatedName(mMasterCodeComboBox->currentText())));
	}


}
