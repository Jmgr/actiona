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

#pragma once

#include "actiontools/actiondefinition.hpp"
#include "textinstance.hpp"
#include "actiontools/textparameterdefinition.hpp"
#include "actiontools/numberparameterdefinition.hpp"
#include "actiontools/booleanparameterdefinition.hpp"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class TextDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT
	
	public:
		explicit TextDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			auto &text = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("text"), tr("Text")});
            text.setTooltip(tr("The text to write"));

            auto &pause = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("pause"), tr("Pause between characters")}, 1);
            pause.setTooltip(tr("The pause duration between each character"));
            pause.setMinimum(0);
            pause.setMaximum(std::numeric_limits<int>::max());
            pause.setDefaultValue(QStringLiteral("0"));
            pause.setSuffix(tr(" ms", "milliseconds"));

            auto &noUnicodeCharacters = addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("noUnicodeCharacters"), tr("Do not send Unicode characters")}, 1);
            noUnicodeCharacters.setTooltip(tr("Prevent using Unicode characters. Enables a limited set of characters on some programs."));
            noUnicodeCharacters.setDefaultValue(QStringLiteral("false"));
            noUnicodeCharacters.setOperatingSystems(ActionTools::WorksOnWindows);

			addException(TextInstance::FailedToSendInputException, tr("Send input failure"));
		}
	
		QString name() const override													{ return QObject::tr("Write text"); }
		QString id() const override														{ return QStringLiteral("ActionWriteText"); }
		QString description() const override												{ return QObject::tr("Write some text"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new TextInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Device; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/text.png")); }
		bool requirementCheck(QStringList &missingRequirements) const override			{ return requirementCheckXTest(missingRequirements); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }
	
	private:
		Q_DISABLE_COPY(TextDefinition)
	};
}

