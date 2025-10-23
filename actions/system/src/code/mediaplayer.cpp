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

#include "mediaplayer.hpp"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVideoWidget>

namespace Code
{
    MediaPlayer::MediaPlayer()
		: CodeClass(),
		mMediaPlayer(new QMediaPlayer(this)),
        mVideoWidget(new QVideoWidget()),
        mAudioOutput(new QAudioOutput(this))
	{
		mMediaPlayer->setVideoOutput(mVideoWidget);
        mMediaPlayer->setAudioOutput(mAudioOutput);
		mVideoWidget->setVisible(false);
    }
	
    MediaPlayer::~MediaPlayer()
	{
		delete mVideoWidget;
	}

    qreal MediaPlayer::playbackRate() const
	{
		return mMediaPlayer->playbackRate();
	}

    qreal MediaPlayer::volume() const
    {
        return mMediaPlayer->audioOutput()->volume();
	}

    qint64 MediaPlayer::position() const
	{
		return mMediaPlayer->position();
	}
	
    MediaPlayer *MediaPlayer::setPlaybackRate(qreal rate)
	{
		mMediaPlayer->setPlaybackRate(rate);
		
        return this;
	}
	
    MediaPlayer *MediaPlayer::setVolume(qreal volume)
	{
        mMediaPlayer->audioOutput()->setVolume(volume);
		
        return this;
	}
	
    MediaPlayer *MediaPlayer::setPosition(qint64 position)
	{
		mMediaPlayer->setPosition(position);
		
        return this;
	}
	
    MediaPlayer *MediaPlayer::setMuted(bool muted)
	{
        mMediaPlayer->audioOutput()->setMuted(muted);
		
        return this;
	}

    qint64 MediaPlayer::duration() const
	{
		return mMediaPlayer->duration();
	}

    bool MediaPlayer::hasAudio() const
    {
        return mMediaPlayer->hasAudio();
    }

    bool MediaPlayer::hasVideo() const
    {
        return mMediaPlayer->hasVideo();
    }
	
    bool MediaPlayer::isMuted() const
	{
        return mMediaPlayer->audioOutput()->isMuted();
	}
	
    bool MediaPlayer::isSeekable() const
	{
		return mMediaPlayer->isSeekable();
	}
	
    int MediaPlayer::bufferStatus() const
	{
        return static_cast<int>(mMediaPlayer->bufferProgress() * 100.f);
	}
	
    MediaPlayer *MediaPlayer::play()
	{
		mMediaPlayer->play();
		
        return this;
	}
	
    MediaPlayer *MediaPlayer::pause()
	{
		mMediaPlayer->pause();
		
        return this;
	}
	
    MediaPlayer *MediaPlayer::stop()
	{
		mMediaPlayer->stop();
		
        return this;
	}

    MediaPlayer *MediaPlayer::setLocalMedia(const QString &path)
    {
        mMediaPlayer->setSource(QUrl::fromLocalFile(path));

        return this;
    }

    MediaPlayer *MediaPlayer::setDistantMedia(const QString &url)
    {
        mMediaPlayer->setSource(QUrl(url));

        return this;
    }

    void MediaPlayer::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<MediaPlayer*>("const MediaPlayer *");

        CodeClass::registerClass<MediaPlayer>(QStringLiteral("MediaPlayer"), scriptEngine);
    }
}
