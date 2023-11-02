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

#include <QtGlobal>
#include <QAudioOutput>

#include "playsoundinstance.hpp"

namespace Actions
{
	PlaySoundInstance::PlaySoundInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		  mMediaPlayer(new QMediaPlayer(this)),
		  mBlocking(false)
	{
        connect(mMediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &PlaySoundInstance::mediaStatusChanged);
	}

    PlaySoundInstance::~PlaySoundInstance() = default;

	void PlaySoundInstance::startExecution()
	{
		bool ok = true;

		QString file = evaluateString(ok, QStringLiteral("file"));
		int volume = evaluateInteger(ok, QStringLiteral("volume"));
		mBlocking = evaluateBoolean(ok, QStringLiteral("blocking"));
		bool looping = evaluateBoolean(ok, QStringLiteral("looping"));
		bool isUrl = evaluateBoolean(ok, QStringLiteral("url"));
		int playbackRate = evaluateInteger(ok, QStringLiteral("playbackRate"));

		if(!ok)
			return;

        mMediaPlayer->stop();

		QUrl url;

		if(isUrl)
			url = QUrl(file);
		else
			url = QUrl::fromLocalFile(file);

        mMediaPlayer->setSource(url);

        mMediaPlayer->setLoops(looping ? QMediaPlayer::Infinite : QMediaPlayer::Once);

        mMediaPlayer->setPlaybackRate(playbackRate / 100.0f);
        mMediaPlayer->audioOutput()->setVolume(volume);
		mMediaPlayer->play();

		if(mMediaPlayer->error() != QMediaPlayer::NoError)
		{
			emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Unable to play file %1: %2").arg(file).arg(mMediaPlayer->errorString()));
			return;
		}

		if(!mBlocking)
			executionEnded();
	}

	void PlaySoundInstance::pauseExecution()
	{
		mMediaPlayer->pause();
	}

	void PlaySoundInstance::resumeExecution()
	{
		mMediaPlayer->play();
	}

	void PlaySoundInstance::stopExecution()
	{
		mMediaPlayer->stop();
	}

	void PlaySoundInstance::stopLongTermExecution()
	{
		mMediaPlayer->stop();
	}

    void PlaySoundInstance::mediaStatusChanged(QMediaPlayer::MediaStatus status)
	{
        if(status == QMediaPlayer::EndOfMedia)
			executionEnded();
	}
}
