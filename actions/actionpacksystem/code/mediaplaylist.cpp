/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#include "mediaplaylist.h"

#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QVideoWidget>

namespace Code
{
	QScriptValue MediaPlaylist::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
		
		return engine->newQObject(new MediaPlaylist, QScriptEngine::ScriptOwnership);
	}
	
	MediaPlaylist::MediaPlaylist()
		: QObject(),
		QScriptable(),
		mMediaPlaylist(new QMediaPlaylist(this)),
		mMediaPlayer(new QMediaPlayer(this)),
		mVideoWidget(new QVideoWidget())
	{
		mMediaPlayer->setPlaylist(mMediaPlaylist);
		mMediaPlayer->setVideoOutput(mVideoWidget);
		mVideoWidget->setVisible(false);
		
		connect(mMediaPlayer, SIGNAL(videoAvailableChanged(bool)), this, SLOT(videoAvailableChanged(bool)));
	}
	
	MediaPlaylist::~MediaPlaylist()
	{
		delete mVideoWidget;
	}
	
	QScriptValue MediaPlaylist::setPlaybackRate(qreal rate)
	{
		mMediaPlayer->setPlaybackRate(rate);
		
		return context()->thisObject();
	}
	
	QScriptValue MediaPlaylist::setVolume(qreal volume)
	{
		mMediaPlayer->setVolume(volume);
		
		return context()->thisObject();
	}
	
	QScriptValue MediaPlaylist::setPosition(qint64 position)
	{
		mMediaPlayer->setPosition(position);
		
		return context()->thisObject();
	}
	
	QScriptValue MediaPlaylist::setMuted(bool muted)
	{
		mMediaPlayer->setMuted(muted);
		
		return context()->thisObject();
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
		
		return context()->thisObject();
	}
	
	QScriptValue MediaPlaylist::pause()
	{
		mMediaPlayer->pause();
		
		return context()->thisObject();
	}
	
	QScriptValue MediaPlaylist::stop()
	{
		mMediaPlayer->stop();
		
		return context()->thisObject();
	}
	
	QScriptValue MediaPlaylist::addLocalMedia(const QString &path)
	{
		if(!mMediaPlaylist->addMedia(QUrl::fromLocalFile(path)))
		{
			context()->throwError(tr("Add media failed : %1").arg(mMediaPlaylist->errorString()));
			return context()->thisObject();
		}
		
		return context()->thisObject();
	}
	
	QScriptValue MediaPlaylist::addDistantMedia(const QString &path)
	{
		if(!mMediaPlaylist->addMedia(QUrl(path)))
		{
			context()->throwError(tr("Add media failed : %1").arg(mMediaPlaylist->errorString()));
			return context()->thisObject();
		}
		
		return context()->thisObject();
	}

	QScriptValue MediaPlaylist::insertLocalMedia(int position, const QString &path)
	{
		if(!mMediaPlaylist->insertMedia(position, QUrl::fromLocalFile(path)))
		{
			context()->throwError(tr("Insert media failed : %1").arg(mMediaPlaylist->errorString()));
			return context()->thisObject();
		}
		
		return context()->thisObject();
	}
	
	QScriptValue MediaPlaylist::insertDistantMedia(int position, const QString &path)
	{
		if(!mMediaPlaylist->insertMedia(position, QUrl(path)))
		{
			context()->throwError(tr("Insert media failed : %1").arg(mMediaPlaylist->errorString()));
			return context()->thisObject();
		}
		
		return context()->thisObject();
	}

	QScriptValue MediaPlaylist::clear()
	{
		if(!mMediaPlaylist->clear())
		{
			context()->throwError(tr("Clear failed : %1").arg(mMediaPlaylist->errorString()));
			return context()->thisObject();
		}
		
		return context()->thisObject();
	}

	QScriptValue MediaPlaylist::next()
	{
		mMediaPlaylist->next();
		
		return context()->thisObject();
	}

	QScriptValue MediaPlaylist::previous()
	{
		mMediaPlaylist->previous();
		
		return context()->thisObject();
	}

	QScriptValue MediaPlaylist::setCurrentMedia(int mediaIndex)
	{
		mMediaPlaylist->setCurrentIndex(mediaIndex);
		
		return context()->thisObject();
	}

	QScriptValue MediaPlaylist::shuffle()
	{
		mMediaPlaylist->shuffle();
		
		return context()->thisObject();
	}

	QScriptValue MediaPlaylist::setPlaybackMode(PlaybackMode playbackMode)
	{
		mMediaPlaylist->setPlaybackMode(static_cast<QMediaPlaylist::PlaybackMode>(playbackMode));
		
		return context()->thisObject();
	}

	QScriptValue MediaPlaylist::removeMedia(int position)
	{
		if(!mMediaPlaylist->removeMedia(position))
		{
			context()->throwError(tr("Remove media failed : %1").arg(mMediaPlaylist->errorString()));
			return context()->thisObject();
		}
		
		return context()->thisObject();
	}
	
	int MediaPlaylist::currentMedia() const
	{
		return mMediaPlaylist->currentIndex();
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
	
	MediaPlaylist::PlaybackMode MediaPlaylist::playbackMode() const
	{
		return static_cast<MediaPlaylist::PlaybackMode>(mMediaPlaylist->playbackMode());
	}
	
	void MediaPlaylist::videoAvailableChanged(bool videoAvailable)
	{
		mVideoWidget->setVisible(videoAvailable);
	}
}
