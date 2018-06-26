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
	class NotifyDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit NotifyDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            auto &title = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("title"), tr("Title")});
            title.setTooltip(tr("The notification title"));

            auto &text = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("text"), tr("Text")});
            text.setTooltip(tr("The notification text"));

            auto &timeout = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("timeout"), tr("Timeout")}, 1);
            timeout.setTooltip(tr("The notification timeout"));
            timeout.setMinimum(0);
            timeout.setMaximum(std::numeric_limits<int>::max());
            timeout.setDefaultValue(QStringLiteral("3000"));

            auto &icon = addParameter<ActionTools::FileParameterDefinition>({QStringLiteral("icon"), tr("Icon")}, 1);
            icon.setTooltip(tr("The notification icon"));

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

