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

#include "groupdefinition.h"
#include "listparameterdefinition.h"
#include "codecombobox.h"
#include "codelineedit.h"

#include <QGroupBox>
#include <QDebug>

namespace ActionTools
{
	GroupDefinition::GroupDefinition(QObject *parent)
        : ElementDefinition(Name(), parent)
		
	{
	}

    void GroupDefinition::setMasterList(ListParameterDefinition &masterList)
	{
		disconnect();

        connect(&masterList, &ListParameterDefinition::editorBuilt, this, &GroupDefinition::masterEditorBuilt);

        mMasterList = &masterList;
	}

	void GroupDefinition::init()
	{
		masterTextChanged(mMasterCodeComboBox->currentText());
		masterCodeChanged(mMasterCodeComboBox->codeLineEdit()->isCode());
	}
	
	void GroupDefinition::setDefaultValues(ActionInstance *actionInstance)
	{
		for(ParameterDefinition *parameterDefinition: members())
			parameterDefinition->setDefaultValues(actionInstance);
	}

	void GroupDefinition::masterEditorBuilt()
	{
		mMasterCodeComboBox = mMasterList->codeComboBox();

        connect(mMasterCodeComboBox, &CodeComboBox::editTextChanged, this, &GroupDefinition::masterTextChanged);
        connect(mMasterCodeComboBox, &CodeComboBox::codeChanged, this, &GroupDefinition::masterCodeChanged);
	}

	void GroupDefinition::masterTextChanged(const QString &text)
	{
		if(!mMasterCodeComboBox->isCode())
			enableMembers(mMasterValues.contains(mMasterList->originalNameFromTranslatedName(text)));
	}

	void GroupDefinition::masterCodeChanged(bool code)
	{
		if(code)
			enableMembers(true);
		else
			enableMembers(mMasterValues.contains(mMasterList->originalNameFromTranslatedName(mMasterCodeComboBox->currentText())));
	}

	void GroupDefinition::enableMembers(bool enable)
	{
		for(ParameterDefinition *parameterDefinition: members())
			parameterDefinition->parentWidget()->setEnabled(enable);
	}
}
