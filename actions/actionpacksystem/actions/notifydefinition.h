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

#pragma once

#include "actiondefinition.h"
#include "notifyinstance.h"
#include "textparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "fileparameterdefinition.h"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class NotifyDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit NotifyDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			auto title = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("title"), tr("Title")), this);
			title->setTooltip(tr("The notification title"));
			addElement(title);

			auto text = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("text"), tr("Text")), this);
			text->setTooltip(tr("The notification text"));
			addElement(text);

			auto timeout = new ActionTools::NumberParameterDefinition(ActionTools::Name(QStringLiteral("timeout"), tr("Timeout")), this);
			timeout->setTooltip(tr("The notification timeout"));
			timeout->setMinimum(0);
			timeout->setMaximum(std::numeric_limits<int>::max());
			timeout->setDefaultValue(QStringLiteral("3000"));
			addElement(timeout, 1);

			auto icon = new ActionTools::FileParameterDefinition(ActionTools::Name(QStringLiteral("icon"), tr("Icon")), this);
			icon->setTooltip(tr("The notification icon"));
			addElement(icon, 1);

			addException(NotifyInstance::UnableToShowNotificationException, tr("Show notification failure"));
		}

		QString name() const override													{ return QObject::tr("Notify"); }
		QString id() const override														{ return QStringLiteral("ActionNotify"); }
		ActionTools::Flag flags() const override											{ return ActionTools::WorksOnGnuLinux | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Shows a message using the notify system"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new NotifyInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::System; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/notification.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(NotifyDefinition)
	};
}

