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

#include "code/codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>

class QMediaPlaylist;
class QMediaPlayer;
class QVideoWidget;

namespace Code
{
	class MediaPlaylist : public CodeClass
	{
		Q_OBJECT
		Q_ENUMS(PlaybackMode)
		Q_PROPERTY(qreal playbackRate READ playbackRate WRITE setPlaybackRate)
		Q_PROPERTY(qreal volume READ volume WRITE setVolume)
		Q_PROPERTY(qint64 position READ position WRITE setPosition)
		Q_PROPERTY(bool muted READ isMuted WRITE setMuted)
		Q_PROPERTY(int currentMedia READ currentMedia WRITE setCurrentMedia)
		Q_PROPERTY(PlaybackMode playbackMode READ playbackMode WRITE setPlaybackMode)

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
		~MediaPlaylist() override;

		qreal playbackRate() const;
		qreal volume() const;
		qint64 position() const;
		int currentMedia() const;
		PlaybackMode playbackMode() const;

	public slots:
		QString toString() const override                                { return QStringLiteral("MediaPlaylist"); }
        bool equals(const QScriptValue &other) const override    { return defaultEqualsImplementation<MediaPlaylist>(other); }
		//Player
		QScriptValue setPlaybackRate(qreal rate);
		QScriptValue setVolume(qreal volume);
		QScriptValue setPosition(qint64 position);
		QScriptValue setMuted(bool muted);
		qint64 duration() const;
		bool hasAudio() const;
		bool hasVideo() const;
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
		bool isEmpty() const;
		int mediaCount() const;
		int nextMedia() const;
		int previousMedia() const;

	private slots:
		void videoAvailableChanged(bool videoAvailable);

	private:
		QMediaPlaylist *mMediaPlaylist;
		QMediaPlayer *mMediaPlayer;
		QVideoWidget *mVideoWidget;
	};
}

