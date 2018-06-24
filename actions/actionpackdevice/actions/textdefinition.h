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
#include "textinstance.h"
#include "textparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "booleanparameterdefinition.h"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class TextDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT
	
	public:
		explicit TextDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			auto text = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("text"), tr("Text")), this);
			text->setTooltip(tr("The text to write"));
			addElement(text);

			auto pause = new ActionTools::NumberParameterDefinition(ActionTools::Name(QStringLiteral("pause"), tr("Pause between characters")), this);
			pause->setTooltip(tr("The pause duration between each character"));
			pause->setMinimum(0);
			pause->setMaximum(std::numeric_limits<int>::max());
			pause->setDefaultValue(QStringLiteral("0"));
			pause->setSuffix(tr(" ms", "milliseconds"));
			addElement(pause, 1);

			auto noUnicodeCharacters = new ActionTools::BooleanParameterDefinition(ActionTools::Name(QStringLiteral("noUnicodeCharacters"), tr("Do not send Unicode characters")), this);
            noUnicodeCharacters->setTooltip(tr("Prevent using Unicode characters. Enables a limited set of characters on some programs."));
			noUnicodeCharacters->setDefaultValue(QStringLiteral("false"));
            noUnicodeCharacters->setOperatingSystems(ActionTools::WorksOnWindows);
            addElement(noUnicodeCharacters, 1);

			addException(TextInstance::FailedToSendInputException, tr("Send input failure"));
		}
	
		QString name() const override													{ return QObject::tr("Write text"); }
		QString id() const override														{ return QStringLiteral("ActionWriteText"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
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

