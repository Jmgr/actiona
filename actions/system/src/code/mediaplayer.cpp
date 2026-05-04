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

        connect(mMediaPlayer, &QMediaPlayer::errorOccurred, this, &MediaPlayer::mediaErrorOccurred);
        connect(mMediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &MediaPlayer::mediaStatusChanged);
        connect(mMediaPlayer, &QMediaPlayer::playbackStateChanged, this, &MediaPlayer::playbackStateChanged);
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

    MediaPlayer::Error MediaPlayer::error() const
    {
        return static_cast<Error>(mMediaPlayer->error());
    }

    QString MediaPlayer::errorString() const
    {
        return mMediaPlayer->errorString();
    }

    MediaPlayer::MediaStatus MediaPlayer::mediaStatus() const
    {
        return static_cast<MediaStatus>(mMediaPlayer->mediaStatus());
    }

    MediaPlayer::PlaybackState MediaPlayer::playbackState() const
    {
        return static_cast<PlaybackState>(mMediaPlayer->playbackState());
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

    void MediaPlayer::mediaErrorOccurred(QMediaPlayer::Error error, const QString &errorString)
    {
        if(!mOnError.isUndefined())
            mOnError.call(QJSValueList() << static_cast<Error>(error) << playbackErrorString(errorString));
    }

    void MediaPlayer::mediaStatusChanged(QMediaPlayer::MediaStatus status)
    {
        if(!mOnMediaStatusChanged.isUndefined())
            mOnMediaStatusChanged.call(QJSValueList() << static_cast<MediaStatus>(status));

        if(status == QMediaPlayer::EndOfMedia && !mOnFinished.isUndefined())
            mOnFinished.call();

        if(status == QMediaPlayer::InvalidMedia && !mOnError.isUndefined())
            mOnError.call(QJSValueList() << error() << playbackErrorString(mMediaPlayer->errorString()));
    }

    void MediaPlayer::playbackStateChanged(QMediaPlayer::PlaybackState state)
    {
        if(!mOnPlaybackStateChanged.isUndefined())
            mOnPlaybackStateChanged.call(QJSValueList() << static_cast<PlaybackState>(state));
    }

    QString MediaPlayer::playbackErrorString(const QString &errorString) const
    {
        if(!errorString.isEmpty())
            return errorString;

        if(mMediaPlayer->mediaStatus() == QMediaPlayer::InvalidMedia)
            return tr("Invalid media");

        if(mMediaPlayer->mediaStatus() == QMediaPlayer::NoMedia)
            return tr("No media has been set");

        return tr("Unknown media error");
    }
}
