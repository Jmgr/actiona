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

#include "numberparameterdefinition.h"
#include "subparameter.h"
#include "codespinbox.h"
#include "codelineedit.h"
#include "actioninstance.h"

namespace ActionTools
{
    NumberParameterDefinition::NumberParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
		mSpinBox(nullptr),
		mMaximum(99),
		mMinimum(0),
		mSingleStep(1)
	{
	}

	void NumberParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mSpinBox = new CodeSpinBox(parent);

		mSpinBox->setPrefix(mPrefix);
		mSpinBox->setSuffix(mSuffix);
		mSpinBox->setMaximum(mMaximum);
		mSpinBox->setMinimum(mMinimum);
		mSpinBox->setSingleStep(mSingleStep);
        mSpinBox->setSpecialValueText(mSpecialValueText);

		addEditor(mSpinBox);
	}

	void NumberParameterDefinition::load(const ActionInstance *actionInstance)
	{
		SubParameter subParameter = actionInstance->subParameter(name().original(), QStringLiteral("value"));

        mSpinBox->setFromSubParameter(subParameter);

        if(!subParameter.isCode() && !mSpecialValueText.isEmpty() && subParameter.value() == QString::number(mMinimum))
            mSpinBox->setValue(mMinimum);//Special case to display the special value text
	}

	void NumberParameterDefinition::save(ActionInstance *actionInstance)
	{
        QString text = mSpinBox->text();

        if(!mSpecialValueText.isEmpty() && text == mSpecialValueText)
            text = QString::number(mSpinBox->value());

		actionInstance->setSubParameter(name().original(), QStringLiteral("value"), mSpinBox->isCode(), text);
	}
}
