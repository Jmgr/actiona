/*
	Actiona
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#include "colorpositionparameterdefinition.h"
#include "subparameter.h"
#include "positionedit.h"
#include "coloredit.h"
#include "actioninstance.h"

namespace ActionTools
{
    ColorPositionParameterDefinition::ColorPositionParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
		mPositionEdit(0),
		mColorEdit(0)
	{
	}

	void ColorPositionParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);
		
		mPositionEdit = new PositionEdit(parent);

		addEditor(mPositionEdit);
	
		mColorEdit = new ColorEdit(parent);
		mColorEdit->setChooseByPositionButtonVisible(false);

		addEditor(mColorEdit);
		
        connect(mPositionEdit, SIGNAL(positionChosen(QPointF)), mColorEdit, SLOT(setPosition(QPointF)), Qt::QueuedConnection);
        connect(mColorEdit, SIGNAL(positionChosen(QPointF)), mPositionEdit, SLOT(setPosition(QPointF)), Qt::QueuedConnection);
	}

	void ColorPositionParameterDefinition::load(const ActionInstance *actionInstance)
	{
		mPositionEdit->setFromSubParameter(actionInstance->subParameter(name().original(), "position"));
		mColorEdit->setFromSubParameter(actionInstance->subParameter(name().original(), "color"));
	}

	void ColorPositionParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), "position", mPositionEdit->isCode(), mPositionEdit->text());
		actionInstance->setSubParameter(name().original(), "color", mColorEdit->isCode(), mColorEdit->text());
	}

	void ColorPositionParameterDefinition::setDefaultValues(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), "position", defaultPosition());

        QColor localDefaultColor = defaultColor();
        actionInstance->setSubParameter(name().original(), "color", QString("%1:%2:%3").arg(localDefaultColor.red()).arg(localDefaultColor.green()).arg(localDefaultColor.blue()));
	}
}
