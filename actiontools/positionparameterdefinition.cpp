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

#include "positionparameterdefinition.h"
#include "positionedit.h"
#include "actioninstance.h"

#include <QComboBox>
#include <QApplication>
#include <QDesktopWidget>

namespace ActionTools
{
    PositionParameterDefinition::PositionParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
        mPositionEdit(nullptr),
        mPositionUnitComboBox(nullptr)
	{
	}

	void PositionParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mPositionEdit = new PositionEdit(parent);
        connect(mPositionEdit, &PositionEdit::positionChosen, this, &PositionParameterDefinition::positionChosen);

		addEditor(mPositionEdit);

        mPositionUnitComboBox = new QComboBox(parent);

        mPositionUnitComboBox->addItems({tr("pixels"), tr("percents")});

        addEditor(mPositionUnitComboBox);
	}

	void PositionParameterDefinition::load(const ActionInstance *actionInstance)
	{
		mPositionEdit->setFromSubParameter(actionInstance->subParameter(name().original(), QStringLiteral("value")));
		mPositionUnitComboBox->setCurrentIndex(actionInstance->subParameter(name().original(), QStringLiteral("unit")).value().toInt());
	}

	void PositionParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name().original(), QStringLiteral("value"), mPositionEdit->isCode(), mPositionEdit->text());
		actionInstance->setSubParameter(name().original(), QStringLiteral("unit"), QString::number(mPositionUnitComboBox->currentIndex()));
    }

    void PositionParameterDefinition::positionChosen(QPointF position)
    {
        if(mPositionUnitComboBox->currentIndex() == 1)//Percents
        {
            QRect screenGeometry = QApplication::desktop()->screenGeometry();

            mPositionEdit->setPosition(QPointF((position.x() * 100) / screenGeometry.width(),
                                              (position.y() * 100) / screenGeometry.height()));
        }
    }
}
