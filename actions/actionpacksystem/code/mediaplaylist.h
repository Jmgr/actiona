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

#ifndef MEDIAPLAYLIST_H
#define MEDIAPLAYLIST_H

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>

class QMediaPlaylist;
class QMediaPlayer;
class QVideoWidget;

namespace Code
{
	class MediaPlaylist : public QObject, public QScriptable
	{
		Q_OBJECT
		Q_ENUMS(PlaybackMode)
		
	public:
		enum PlaybackMode
		{
			CurrentItemOnce,
			CurrentItemInLoop,
			Sequential,
			Loop,
			Random
		};
		
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		
		MediaPlaylist();
		~MediaPlaylist();
		
	public slots:
		//Player
		QScriptValue setPlaybackRate(qreal rate);
		QScriptValue setVolume(qreal volume);
		QScriptValue setPosition(qint64 position);
		QScriptValue setMuted(bool muted);
		qint64 duration() const;
		bool hasAudio() const;
		bool hasVideo() const;
		qreal playbackRate() const;
		qreal volume() const;
		qint64 position() const;
		bool isMuted() const;
		bool isSeekable() const;
		int bufferStatus() const;
		QScriptValue play();
		QScriptValue pause();
		QScriptValue stop();
		
		//Playlist
		QScriptValue addLocalMedia(const QString &path);
		QScriptValue addDistantMedia(const QString &path);
		QScriptValue insertLocalMedia(int position, const QString &path);
		QScriptValue insertDistantMedia(int position, const QString &path);
		QScriptValue clear();
		QScriptValue next();
		QScriptValue previous();
		QScriptValue setCurrentMedia(int mediaIndex);
		QScriptValue shuffle();
		QScriptValue setPlaybackMode(PlaybackMode playbackMode);
		QScriptValue removeMedia(int position);
		int currentMedia() const;
		bool isEmpty() const;
		int mediaCount() const;
		int nextMedia() const;
		int previousMedia() const;
		PlaybackMode playbackMode() const;
		qint64 playlistDuration() const;
		
	private slots:
		void videoAvailableChanged(bool videoAvailable);
		
	private:
		QMediaPlaylist *mMediaPlaylist;
		QMediaPlayer *mMediaPlayer;
		QVideoWidget *mVideoWidget;
	};
}

#endif // MEDIAPLAYLIST_H
