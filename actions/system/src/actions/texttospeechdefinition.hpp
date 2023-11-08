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
#include "texttospeechinstance.hpp"
#include "actiontools/textparameterdefinition.hpp"
#include "actiontools/numberparameterdefinition.hpp"
#include "actiontools/localeparameterdefinition.hpp"
#include "actiontools/booleanparameterdefinition.hpp"

#include <QTextToSpeech>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
    class TextToSpeechDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
        explicit TextToSpeechDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            auto &text = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("text"), tr("Text")});
            text.setTooltip(tr("The text to say"));

            auto &volume = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("volume"), tr("Volume")});
            volume.setTooltip(tr("The speech volume"));
            volume.setMinimum(-1);
            volume.setMaximum(100);
            volume.setSuffix(tr("%", "percent"));
            volume.setDefaultValue(QStringLiteral("-1"));
            volume.setSpecialValueText(tr("Default volume"));

            auto &language = addParameter<ActionTools::LocaleParameterDefinition>({QStringLiteral("language"), tr("Language")});
            language.setTooltip(tr("The language to use"));
            language.setLocales(QTextToSpeech{}.availableLocales());

            auto &blocking = addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("blocking"), tr("Wait until speech finished")});
            blocking.setTooltip(tr("Should the action end only when the speech is finished"));
            blocking.setDefaultValue(QStringLiteral("true"));

            auto &playbackrate = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("playbackRate"), tr("Playback rate")}, 1);
            playbackrate.setTooltip(tr("The playback rate"));
            playbackrate.setMinimum(0);
            playbackrate.setMaximum(200);
            playbackrate.setSuffix(tr("%", "percent"));
            playbackrate.setDefaultValue(QStringLiteral("100"));

            auto &pitch = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("pitch"), tr("Pitch")}, 1);
            pitch.setTooltip(tr("The sound frequency of the voice"));
            pitch.setMinimum(0);
            pitch.setMaximum(200);
            pitch.setSuffix(tr("%", "percent"));
            pitch.setDefaultValue(QStringLiteral("100"));
		}

        QString name() const override													{ return QObject::tr("Text to speech"); }
        QString id() const override														{ return QStringLiteral("ActionTextToSpeech"); }
        QString description() const override												{ return QObject::tr("Reads some text using text to speech"); }
        ActionTools::ActionInstance *newActionInstance() const override					{ return new TextToSpeechInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::System; }
        QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/texttospeech.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
        Q_DISABLE_COPY(TextToSpeechDefinition)
	};
}

