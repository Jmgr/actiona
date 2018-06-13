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
#include "playsoundinstance.h"
#include "fileparameterdefinition.h"
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
	class PlaySoundDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit PlaySoundDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::FileParameterDefinition *file = new ActionTools::FileParameterDefinition(ActionTools::Name(QStringLiteral("file"), tr("Sound file/URL")), this);
			file->setTooltip(tr("The sound file or URL to play"));
			file->setMode(ActionTools::FileEdit::FileOpen);
			file->setCaption(tr("Choose the sound file"));
			file->setFilter(tr("All files (*.*)"));
			addElement(file);

			ActionTools::BooleanParameterDefinition *url = new ActionTools::BooleanParameterDefinition(ActionTools::Name(QStringLiteral("url"), tr("URL")), this);
			url->setTooltip(tr("Is the sound resource an URL"));
			url->setDefaultValue(QStringLiteral("false"));
			addElement(url);

			ActionTools::NumberParameterDefinition *volume = new ActionTools::NumberParameterDefinition(ActionTools::Name(QStringLiteral("volume"), tr("Volume")), this);
			volume->setTooltip(tr("The volume to play at"));
			volume->setMinimum(0);
			volume->setMaximum(100);
			volume->setSuffix(tr("%", "percent"));
			volume->setDefaultValue(QStringLiteral("100"));
			addElement(volume);

			ActionTools::BooleanParameterDefinition *blocking = new ActionTools::BooleanParameterDefinition(ActionTools::Name(QStringLiteral("blocking"), tr("Wait until played")), this);
			blocking->setTooltip(tr("Should the action end only when the sound has finished playing"));
			blocking->setDefaultValue(QStringLiteral("true"));
			addElement(blocking);

			ActionTools::BooleanParameterDefinition *loop = new ActionTools::BooleanParameterDefinition(ActionTools::Name(QStringLiteral("looping"), tr("Looping")), this);
			loop->setTooltip(tr("Should the sound loop"));
			loop->setDefaultValue(QStringLiteral("false"));
			addElement(loop, 1);

			ActionTools::NumberParameterDefinition *playbackrate = new ActionTools::NumberParameterDefinition(ActionTools::Name(QStringLiteral("playbackRate"), tr("Playback rate")), this);
			playbackrate->setTooltip(tr("The playback rate"));
			playbackrate->setMinimum(std::numeric_limits<int>::min());
			playbackrate->setMaximum(std::numeric_limits<int>::max());
			playbackrate->setSuffix(tr("%", "percent"));
			playbackrate->setDefaultValue(QStringLiteral("100"));
			addElement(playbackrate, 1);
		}

		QString name() const override													{ return QObject::tr("Play sound"); }
		QString id() const override														{ return QStringLiteral("ActionPlaySound"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Plays a sound"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new PlaySoundInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::System; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/playsound.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(PlaySoundDefinition)
	};
}

