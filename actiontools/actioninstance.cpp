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
				element->setDefaultValues(this);
		}
	}

	QDataStream &operator << (QDataStream &s, const ActionInstance &actionInstance)
	{
		s << actionInstance.label();
		s << actionInstance.comment();
		s << actionInstance.parametersData();
		s << actionInstance.color();
		s << actionInstance.isEnabled();
		s << actionInstance.isSelected();
		s << actionInstance.exceptionActionInstances();

		return s;
	}

	QDataStream &operator >> (QDataStream &s, ActionInstance &actionInstance)
	{
		QString label;
		QString comment;
		ParametersData parametersData;
		QColor color;
		bool enabled;
		bool selected;
		ExceptionActionInstancesHash exceptionActionInstances;

		s >> label;
		s >> comment;
		s >> parametersData;
		s >> color;
		s >> enabled;
		s >> selected;
		s >> exceptionActionInstances;

		actionInstance.setLabel(label);
		actionInstance.setComment(comment);
		actionInstance.setParametersData(parametersData);
		actionInstance.setColor(color);
		actionInstance.setEnabled(enabled);
		actionInstance.setSelected(selected);
		actionInstance.setExceptionActionInstances(exceptionActionInstances);

		return s;
	}

	QDebug &operator << (QDebug &dbg, const ActionInstance &actionInstance)
	{
		dbg.space() << "Id:" << actionInstance.definition()->id();
		dbg.space() << "Label:" << actionInstance.label();
		dbg.space() << "Comment:" << actionInstance.comment();
		dbg.space() << "Color:" << actionInstance.color();
		dbg.space() << "Enabled:" << actionInstance.isEnabled();
		dbg.space() << "Selected:" << actionInstance.isSelected();
		dbg.space() << "Exception action instances:" << actionInstance.exceptionActionInstances();
		dbg.space() << "Data:" << actionInstance.parametersData();

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
	
	QDebug &operator << (QDebug &dbg, const ExceptionActionInstancesHash &exceptionActionInstancesHash)
	{
		foreach(ActionException::Exception exception, exceptionActionInstancesHash.keys())
		{
			dbg.space() << exception << "=" << exceptionActionInstancesHash.value(exception);
		}

		return dbg.maybeSpace();
	}
}