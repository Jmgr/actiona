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

#include "mediaplaylist.hpp"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVideoWidget>

namespace Code
{
	MediaPlaylist::MediaPlaylist()
		: CodeClass(),
		mMediaPlayer(new QMediaPlayer(this)),
		mVideoWidget(new QVideoWidget())
	{
		mMediaPlayer->setVideoOutput(mVideoWidget);
		mVideoWidget->setVisible(false);
    }
	
	MediaPlaylist::~MediaPlaylist()
	{
		delete mVideoWidget;
	}

	qreal MediaPlaylist::playbackRate() const
	{
		return mMediaPlayer->playbackRate();
	}

	qreal MediaPlaylist::volume() const
    {
        return mMediaPlayer->audioOutput()->volume();
	}

	qint64 MediaPlaylist::position() const
	{
		return mMediaPlayer->position();
	}
	
    MediaPlaylist *MediaPlaylist::setPlaybackRate(qreal rate)
	{
		mMediaPlayer->setPlaybackRate(rate);
		
        return this;
	}
	
    MediaPlaylist *MediaPlaylist::setVolume(qreal volume)
	{
        mMediaPlayer->audioOutput()->setVolume(volume);
		
        return this;
	}
	
    MediaPlaylist *MediaPlaylist::setPosition(qint64 position)
	{
		mMediaPlayer->setPosition(position);
		
        return this;
	}
	
    MediaPlaylist *MediaPlaylist::setMuted(bool muted)
	{
        mMediaPlayer->audioOutput()->setMuted(muted);
		
        return this;
	}

	qint64 MediaPlaylist::duration() const
	{
		return mMediaPlayer->duration();
	}

    bool MediaPlaylist::hasAudio() const
    {
        return mMediaPlayer->hasAudio();
    }

    bool MediaPlaylist::hasVideo() const
    {
        return mMediaPlayer->hasVideo();
    }
	
	bool MediaPlaylist::isMuted() const
	{
        return mMediaPlayer->audioOutput()->isMuted();
	}
	
	bool MediaPlaylist::isSeekable() const
	{
		return mMediaPlayer->isSeekable();
	}
	
	int MediaPlaylist::bufferStatus() const
	{
        return static_cast<int>(mMediaPlayer->bufferProgress() * 100.f);
	}
	
    MediaPlaylist *MediaPlaylist::play()
	{
		mMediaPlayer->play();
		
        return this;
	}
	
    MediaPlaylist *MediaPlaylist::pause()
	{
		mMediaPlayer->pause();
		
        return this;
	}
	
    MediaPlaylist *MediaPlaylist::stop()
	{
		mMediaPlayer->stop();
		
        return this;
	}

    void MediaPlaylist::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<MediaPlaylist*>("const MediaPlaylist *");

        CodeClass::registerClass<MediaPlaylist>(QStringLiteral("MediaPlaylist"), scriptEngine);
    }
}
