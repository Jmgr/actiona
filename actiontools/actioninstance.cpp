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

#include "actioninstance.h"
#include "actiondefinition.h"
#include "actionfactory.h"
#include "elementdefinition.h"
#include "parameterdefinition.h"
#include "groupdefinition.h"

namespace ActionTools
{
	ActionInstance::ActionInstance(const ActionDefinition *definition, QObject *parent)
		: QObject(parent)
	{
		d = new ActionInstanceData();

		d->definition = definition;

		//Set the default values
		if(definition)
		{
			foreach(ElementDefinition *element, definition->elements())
			{
				if(ParameterDefinition *parameter = dynamic_cast<ParameterDefinition *>(element))
					setParameterDefaultValue(parameter);
				else if(GroupDefinition *group = dynamic_cast<GroupDefinition *>(element))
				{
					foreach(ParameterDefinition *member, group->members())
						setParameterDefaultValue(member);
				}
			}
		}
	}

	void ActionInstance::setParameterDefaultValue(ParameterDefinition *parameter)
	{
		parameter->setDefaultValues(d->parametersData[parameter->name()]);
	}

	QDataStream &operator << (QDataStream &s, const ActionInstance &action)
	{
		s << action.label();
		s << action.comment();
		s << action.parametersData();
		s << action.color();
		s << action.isEnabled();
		s << action.isSelected();

		return s;
	}

	QDataStream &operator >> (QDataStream &s, ActionInstance &action)
	{
		QString label;
		QString comment;
		ParametersData parametersData;
		QColor color;
		bool enabled;
		bool selected;

		s >> label;
		s >> comment;
		s >> parametersData;
		s >> color;
		s >> enabled;
		s >> selected;

		action.setLabel(label);
		action.setComment(comment);
		action.setParametersData(parametersData);
		action.setColor(color);
		action.setEnabled(enabled);
		action.setSelected(selected);

		return s;
	}

	QDebug &operator << (QDebug &dbg, const ActionInstance &action)
	{
		dbg.space() << "Id:" << action.definition()->id();
		dbg.space() << "Label:" << action.label();
		dbg.space() << "Comment:" << action.comment();
		dbg.space() << "Color:" << action.color();
		dbg.space() << "Enabled:" << action.isEnabled();
		dbg.space() << "Selected:" << action.isSelected();
		dbg.space() << "Data:" << action.parametersData();

		return dbg.maybeSpace();
	}

	QDebug &operator << (QDebug &dbg, const ParametersData &parametersData)
	{
		foreach(const QString &parameterName, parametersData.keys())
		{
			dbg.space() << parameterName << "=" << parametersData.value(parameterName);
		}

		return dbg.maybeSpace();
	}
}
