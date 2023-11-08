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
#include "playsoundinstance.hpp"
#include "actiontools/fileparameterdefinition.hpp"
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
	class PlaySoundDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit PlaySoundDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            auto &file = addParameter<ActionTools::FileParameterDefinition>({QStringLiteral("file"), tr("Sound file/URL")});
            file.setTooltip(tr("The sound file or URL to play"));
            file.setMode(ActionTools::FileEdit::FileOpen);
            file.setCaption(tr("Choose the sound file"));
            file.setFilter(tr("All files (*.*)"));

            auto &url = addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("url"), tr("URL")});
            url.setTooltip(tr("Is the sound resource an URL"));
            url.setDefaultValue(QStringLiteral("false"));

            auto &volume = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("volume"), tr("Volume")});
            volume.setTooltip(tr("The volume to play at"));
            volume.setMinimum(0);
            volume.setMaximum(100);
            volume.setSuffix(tr("%", "percent"));
            volume.setDefaultValue(QStringLiteral("100"));

            auto &blocking = addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("blocking"), tr("Wait until played")});
            blocking.setTooltip(tr("Should the action end only when the sound has finished playing"));
            blocking.setDefaultValue(QStringLiteral("true"));

            auto &loop = addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("looping"), tr("Looping")}, 1);
            loop.setTooltip(tr("Should the sound loop"));
            loop.setDefaultValue(QStringLiteral("false"));

            auto &playbackrate = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("playbackRate"), tr("Playback rate")}, 1);
            playbackrate.setTooltip(tr("The playback rate"));
            playbackrate.setMinimum(std::numeric_limits<int>::min());
            playbackrate.setMaximum(std::numeric_limits<int>::max());
            playbackrate.setSuffix(tr("%", "percent"));
            playbackrate.setDefaultValue(QStringLiteral("100"));
		}

		QString name() const override													{ return QObject::tr("Play sound"); }
		QString id() const override														{ return QStringLiteral("ActionPlaySound"); }
		QString description() const override												{ return QObject::tr("Plays a sound"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new PlaySoundInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::System; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/playsound.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(PlaySoundDefinition)
	};
}

