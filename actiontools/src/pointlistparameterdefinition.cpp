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

#include "actiontools/pointlistparameterdefinition.hpp"
#include "actiontools/subparameter.hpp"
#include "actiontools/pointlistwidget.hpp"
#include "actiontools/actioninstance.hpp"

namespace ActionTools
{
    PointListParameterDefinition::PointListParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent),
		mPointListWidget(nullptr)
	{
	}

	void PointListParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mPointListWidget = new PointListWidget(parent);

		addEditor(mPointListWidget);
	}

	void PointListParameterDefinition::load(const ActionInstance *actionInstance)
	{
		QString pointBuffer = actionInstance->subParameter(name().original(), QStringLiteral("value")).value();
		QPolygon polygon;
        QStringList pointStrings = pointBuffer.split(QLatin1Char(';'), Qt::SkipEmptyParts);

		for(const QString &pointString: qAsConst(pointStrings))
		{
            QStringList pointComponents = pointString.split(QLatin1Char(':'), Qt::SkipEmptyParts);
			if(pointComponents.size() != 2)
				continue;

			polygon << QPoint(pointComponents.at(0).toInt(), pointComponents.at(1).toInt());
		}

		mPointListWidget->setPoints(polygon);
	}

	void PointListParameterDefinition::save(ActionInstance *actionInstance)
	{
		QString pointBuffer;
        const auto points = mPointListWidget->points();
        for(const QPoint &point: points)
			pointBuffer += QStringLiteral("%1:%2;").arg(point.x()).arg(point.y());

		actionInstance->setSubParameter(name().original(), QStringLiteral("value"), pointBuffer);
	}
}
