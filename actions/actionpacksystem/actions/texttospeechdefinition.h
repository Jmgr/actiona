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
#include "texttospeechinstance.h"
#include "textparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "localeparameterdefinition.h"
#include "booleanparameterdefinition.h"

#include <QTextToSpeech>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
    class TextToSpeechDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
        explicit TextToSpeechDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            ActionTools::TextParameterDefinition *text = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("text"), tr("Text")), this);
            text->setTooltip(tr("The text to say"));
            addElement(text);

            ActionTools::NumberParameterDefinition *volume = new ActionTools::NumberParameterDefinition(ActionTools::Name(QStringLiteral("volume"), tr("Volume")), this);
            volume->setTooltip(tr("The speech volume"));
            volume->setMinimum(-1);
            volume->setMaximum(100);
            volume->setSuffix(tr("%", "percent"));
            volume->setDefaultValue(QStringLiteral("-1"));
            volume->setSpecialValueText(tr("Default volume"));
            addElement(volume);

            ActionTools::LocaleParameterDefinition *language = new ActionTools::LocaleParameterDefinition(ActionTools::Name(QStringLiteral("language"), tr("Language")), this);
            language->setTooltip(tr("The language to use"));
            language->setLocales(QTextToSpeech{}.availableLocales());
            addElement(language);

            ActionTools::BooleanParameterDefinition *blocking = new ActionTools::BooleanParameterDefinition(ActionTools::Name(QStringLiteral("blocking"), tr("Wait until speech finished")), this);
            blocking->setTooltip(tr("Should the action end only when the speech is finished"));
            blocking->setDefaultValue(QStringLiteral("true"));
            addElement(blocking);

            ActionTools::NumberParameterDefinition *playbackrate = new ActionTools::NumberParameterDefinition(ActionTools::Name(QStringLiteral("playbackRate"), tr("Playback rate")), this);
            playbackrate->setTooltip(tr("The playback rate"));
            playbackrate->setMinimum(0);
            playbackrate->setMaximum(200);
            playbackrate->setSuffix(tr("%", "percent"));
            playbackrate->setDefaultValue(QStringLiteral("100"));
            addElement(playbackrate, 1);

            ActionTools::NumberParameterDefinition *pitch = new ActionTools::NumberParameterDefinition(ActionTools::Name(QStringLiteral("pitch"), tr("Pitch")), this);
            pitch->setTooltip(tr("The sound frequency of the voice"));
            pitch->setMinimum(0);
            pitch->setMaximum(200);
            pitch->setSuffix(tr("%", "percent"));
            pitch->setDefaultValue(QStringLiteral("100"));
            addElement(pitch, 1);
		}

        QString name() const													{ return QObject::tr("Text to speech"); }
        QString id() const														{ return QStringLiteral("ActionTextToSpeech"); }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
        QString description() const												{ return QObject::tr("Reads some text using text to speech"); }
        ActionTools::ActionInstance *newActionInstance() const					{ return new TextToSpeechInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::System; }
        QPixmap icon() const													{ return QPixmap(QStringLiteral(":/icons/texttospeech.png")); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
        Q_DISABLE_COPY(TextToSpeechDefinition)
	};
}

