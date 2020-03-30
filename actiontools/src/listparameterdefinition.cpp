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

#include "actiontools/listparameterdefinition.hpp"
#include "actiontools/subparameter.hpp"
#include "actiontools/codecombobox.hpp"
#include "actiontools/actioninstance.hpp"

#include <QDebug>

namespace ActionTools
{
    ListParameterDefinition::ListParameterDefinition(const Name &name, QObject *parent)
        : ItemsParameterDefinition(name, parent),
		mComboBox(nullptr)
	{
	}

	void ListParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mComboBox = new CodeComboBox(parent);

		//The second element of the pair are the translated values
		for(int i=0; i<items().first.size(); ++i)
		{
			if(items().first.at(i) == QLatin1String("[header]"))
				mComboBox->addItem(items().second.at(i), QStringLiteral("header"));
			else
				mComboBox->addItem(items().second.at(i));
		}

		addEditor(mComboBox);

		emit editorBuilt();
	}

	void ListParameterDefinition::load(const ActionInstance *actionInstance)
	{
		const SubParameter &subParameter = actionInstance->subParameter(name().original(), QStringLiteral("value"));

		mComboBox->setCode(subParameter.isCode());
        mComboBox->setEditText(translatedNameFromOriginalName(subParameter.value()));
	}

	void ListParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), QStringLiteral("value"), mComboBox->isCode(), originalNameFromTranslatedName(mComboBox->currentText()));
	}
}
