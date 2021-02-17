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

#include "actiontools/multitextparameterdefinition.hpp"
#include "actiontools/subparameter.hpp"
#include "actiontools/itemlistwidget.hpp"
#include "actiontools/actioninstance.hpp"

namespace ActionTools
{
    MultiTextParameterDefinition::MultiTextParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
		mItemListWidget(nullptr)
	{
	}

	void MultiTextParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mItemListWidget = new ItemListWidget(parent);

		addEditor(mItemListWidget);
	}

	void MultiTextParameterDefinition::load(const ActionInstance *actionInstance)
	{
		QString textBuffer = actionInstance->subParameter(name().original(), QStringLiteral("value")).value();

        mItemListWidget->setItems(textBuffer.split(QLatin1Char('\n'), Qt::SkipEmptyParts));
	}

	void MultiTextParameterDefinition::save(ActionInstance *actionInstance)
	{
		QString textBuffer;
        const auto items = mItemListWidget->items();
        for(const QString &text: items)
			textBuffer += QStringLiteral("%1\n").arg(text);

		actionInstance->setSubParameter(name().original(), QStringLiteral("value"), textBuffer);
	}
}
