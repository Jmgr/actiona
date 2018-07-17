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

#include "mediaplaylist.h"

#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QVideoWidget>

namespace Code
{
	QScriptValue MediaPlaylist::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		return CodeClass::constructor(new MediaPlaylist, context, engine);
	}
	
	MediaPlaylist::MediaPlaylist()
		: CodeClass(),
		mMediaPlaylist(new QMediaPlaylist(this)),
		mMediaPlayer(new QMediaPlayer(this)),
		mVideoWidget(new QVideoWidget())
	{
		mMediaPlayer->setPlaylist(mMediaPlaylist);
		mMediaPlayer->setVideoOutput(mVideoWidget);
		mVideoWidget->setVisible(false);
		
        connect(mMediaPlayer, &QMediaPlayer::videoAvailableChanged, this, &MediaPlaylist::videoAvailableChanged);
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
		return mMediaPlayer->volume();
	}

	qint64 MediaPlaylist::position() const
	{
		return mMediaPlayer->position();
	}


	int MediaPlaylist::currentMedia() const
	{
		return mMediaPlaylist->currentIndex();
	}

	MediaPlaylist::PlaybackMode MediaPlaylist::playbackMode() const
	{
        return static_cast<MediaPlaylist::PlaybackMode>(mMediaPlaylist->playbackMode());
    }
	
	QScriptValue MediaPlaylist::setPlaybackRate(qreal rate)
	{
		mMediaPlayer->setPlaybackRate(rate);
		
		return thisObject();
	}
	
	QScriptValue MediaPlaylist::setVolume(qreal volume)
	{
		mMediaPlayer->setVolume(volume);
		
		return thisObject();
	}
	
	QScriptValue MediaPlaylist::setPosition(qint64 position)
	{
		mMediaPlayer->setPosition(position);
		
		return thisObject();
	}
	
	QScriptValue MediaPlaylist::setMuted(bool muted)
	{
		mMediaPlayer->setMuted(muted);
		
		return thisObject();
	}

	qint64 MediaPlaylist::duration() const
	{
		return mMediaPlayer->duration();
	}
	
	bool MediaPlaylist::hasAudio() const
	{
		return mMediaPlayer->isAudioAvailable();
	}
	
	bool MediaPlaylist::hasVideo() const
	{
		return mMediaPlayer->isVideoAvailable();
	}
	
	bool MediaPlaylist::isMuted() const
	{
		return mMediaPlayer->isMuted();
	}
	
	bool MediaPlaylist::isSeekable() const
	{
		return mMediaPlayer->isSeekable();
	}
	
	int MediaPlaylist::bufferStatus() const
	{
		return mMediaPlayer->bufferStatus();
	}
	
	QScriptValue MediaPlaylist::play()
	{
		mMediaPlayer->play();
		
		return thisObject();
	}
	
	QScriptValue MediaPlaylist::pause()
	{
		mMediaPlayer->pause();
		
		return thisObject();
	}
	
	QScriptValue MediaPlaylist::stop()
	{
		mMediaPlayer->stop();
		
		return thisObject();
	}
	
	QScriptValue MediaPlaylist::addLocalMedia(const QString &path)
	{
		if(!mMediaPlaylist->addMedia(QUrl::fromLocalFile(path)))
		{
			throwError(QStringLiteral("AddMediaError"), tr("Add media failed : %1").arg(mMediaPlaylist->errorString()));
			return thisObject();
		}
		
		return thisObject();
	}
	
	QScriptValue MediaPlaylist::addDistantMedia(const QString &path)
	{
		if(!mMediaPlaylist->addMedia(QUrl(path)))
		{
			throwError(QStringLiteral("AddMediaError"), tr("Add media failed : %1").arg(mMediaPlaylist->errorString()));
			return thisObject();
		}
		
		return thisObject();
	}

	QScriptValue MediaPlaylist::insertLocalMedia(int position, const QString &path)
	{
		if(!mMediaPlaylist->insertMedia(position, QUrl::fromLocalFile(path)))
		{
			throwError(QStringLiteral("InsertMediaError"), tr("Insert media failed : %1").arg(mMediaPlaylist->errorString()));
			return thisObject();
		}
		
		return thisObject();
	}
	
	QScriptValue MediaPlaylist::insertDistantMedia(int position, const QString &path)
	{
		if(!mMediaPlaylist->insertMedia(position, QUrl(path)))
		{
			throwError(QStringLiteral("InsertMediaError"), tr("Insert media failed : %1").arg(mMediaPlaylist->errorString()));
			return thisObject();
		}
		
		return thisObject();
	}

	QScriptValue MediaPlaylist::clear()
	{
		if(!mMediaPlaylist->clear())
		{
			throwError(QStringLiteral("ClearMediaError"), tr("Clear failed : %1").arg(mMediaPlaylist->errorString()));
			return thisObject();
		}
		
		return thisObject();
	}

	QScriptValue MediaPlaylist::next()
	{
		mMediaPlaylist->next();
		
		return thisObject();
	}

	QScriptValue MediaPlaylist::previous()
	{
		mMediaPlaylist->previous();
		
		return thisObject();
	}

	QScriptValue MediaPlaylist::setCurrentMedia(int mediaIndex)
	{
		mMediaPlaylist->setCurrentIndex(mediaIndex);
		
		return thisObject();
	}

	QScriptValue MediaPlaylist::shuffle()
	{
		mMediaPlaylist->shuffle();
		
		return thisObject();
	}

	QScriptValue MediaPlaylist::setPlaybackMode(PlaybackMode playbackMode)
	{
		mMediaPlaylist->setPlaybackMode(static_cast<QMediaPlaylist::PlaybackMode>(playbackMode));
		
		return thisObject();
	}

	QScriptValue MediaPlaylist::removeMedia(int position)
	{
		if(!mMediaPlaylist->removeMedia(position))
		{
			throwError(QStringLiteral("RemoveMediaError"), tr("Remove media failed : %1").arg(mMediaPlaylist->errorString()));
			return thisObject();
		}
		
		return thisObject();
	}
	
	bool MediaPlaylist::isEmpty() const
	{
		return mMediaPlaylist->isEmpty();
	}
	
	int MediaPlaylist::mediaCount() const
	{
		return mMediaPlaylist->mediaCount();
	}
	
	int MediaPlaylist::nextMedia() const
	{
		return mMediaPlaylist->nextIndex();
	}
	
	int MediaPlaylist::previousMedia() const
	{
		return mMediaPlaylist->previousIndex();
	}
	
	void MediaPlaylist::videoAvailableChanged(bool videoAvailable)
	{
		mVideoWidget->setVisible(videoAvailable);
	}
}
