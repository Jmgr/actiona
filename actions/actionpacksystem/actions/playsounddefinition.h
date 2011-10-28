/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#ifndef PLAYSOUNDDEFINITION_H
#define PLAYSOUNDDEFINITION_H

#include "actiondefinition.h"
#include "playsoundinstance.h"
#include "fileparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "booleanparameterdefinition.h"

#include <climits>

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
			ActionTools::FileParameterDefinition *file = new ActionTools::FileParameterDefinition(ActionTools::Name("file", tr("Sound file/URL")), this);
			file->setTooltip(tr("The sound file or URL to play"));
			file->setMode(ActionTools::FileEdit::FileOpen);
			file->setCaption(tr("Choose the sound file"));
			file->setFilter(tr("All files (*.*)"));
			addElement(file);

			ActionTools::BooleanParameterDefinition *url = new ActionTools::BooleanParameterDefinition(ActionTools::Name("url", tr("URL")), this);
			url->setTooltip(tr("Is the sound resource an URL"));
			url->setDefaultValue(false);
			addElement(url);

			ActionTools::NumberParameterDefinition *volume = new ActionTools::NumberParameterDefinition(ActionTools::Name("volume", tr("Volume")), this);
			volume->setTooltip(tr("The volume to play at"));
			volume->setMinimum(0);
			volume->setMaximum(100);
			volume->setSuffix(tr("%", "percent"));
			volume->setDefaultValue(100);
			addElement(volume);

			ActionTools::BooleanParameterDefinition *blocking = new ActionTools::BooleanParameterDefinition(ActionTools::Name("blocking", tr("Wait until played")), this);
			blocking->setTooltip(tr("Should the action end only when the sound has finished playing"));
			blocking->setDefaultValue(true);
			addElement(blocking);

			ActionTools::BooleanParameterDefinition *loop = new ActionTools::BooleanParameterDefinition(ActionTools::Name("looping", tr("Looping")), this);
			loop->setTooltip(tr("Should the sound loop"));
			loop->setDefaultValue(false);
			addElement(loop, 1);

			ActionTools::NumberParameterDefinition *playbackrate = new ActionTools::NumberParameterDefinition(ActionTools::Name("playbackRate", tr("Playback rate")), this);
			playbackrate->setTooltip(tr("The playback rate"));
			playbackrate->setMinimum(INT_MIN);
			playbackrate->setMaximum(INT_MAX);
			playbackrate->setSuffix(tr("%", "percent"));
			playbackrate->setDefaultValue(100);
			addElement(playbackrate, 1);
		}

		QString name() const													{ return QObject::tr("Play sound"); }
		QString id() const														{ return "ActionPlaySound"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Plays a sound"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new PlaySoundInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::System; }
		QPixmap icon() const													{ return QPixmap(":/icons/playsound.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(PlaySoundDefinition)
	};
}

#endif // PLAYSOUNDDEFINITION_H
