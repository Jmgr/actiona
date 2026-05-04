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

#pragma once

#include "actiontools/code/codeclass.hpp"

#include <QJSValue>
#include <QMediaPlayer>

class QVideoWidget;
class QAudioOutput;

namespace Code
{
    class MediaPlayer : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(qreal playbackRate READ playbackRate WRITE setPlaybackRate)
		Q_PROPERTY(qreal volume READ volume WRITE setVolume)
		Q_PROPERTY(qint64 position READ position WRITE setPosition)
		Q_PROPERTY(bool muted READ isMuted WRITE setMuted)
        Q_PROPERTY(QJSValue onError READ onError WRITE setOnError)
        Q_PROPERTY(QJSValue onFinished READ onFinished WRITE setOnFinished)
        Q_PROPERTY(QJSValue onMediaStatusChanged READ onMediaStatusChanged WRITE setOnMediaStatusChanged)
        Q_PROPERTY(QJSValue onPlaybackStateChanged READ onPlaybackStateChanged WRITE setOnPlaybackStateChanged)

	public:
        enum Error
        {
            NoError = QMediaPlayer::NoError,
            ResourceError = QMediaPlayer::ResourceError,
            FormatError = QMediaPlayer::FormatError,
            NetworkError = QMediaPlayer::NetworkError,
            AccessDeniedError = QMediaPlayer::AccessDeniedError
        };
        Q_ENUM(Error)

        enum MediaStatus
        {
            NoMedia = QMediaPlayer::NoMedia,
            LoadingMedia = QMediaPlayer::LoadingMedia,
            LoadedMedia = QMediaPlayer::LoadedMedia,
            StalledMedia = QMediaPlayer::StalledMedia,
            BufferingMedia = QMediaPlayer::BufferingMedia,
            BufferedMedia = QMediaPlayer::BufferedMedia,
            EndOfMedia = QMediaPlayer::EndOfMedia,
            InvalidMedia = QMediaPlayer::InvalidMedia
        };
        Q_ENUM(MediaStatus)

        enum PlaybackState
        {
            StoppedState = QMediaPlayer::StoppedState,
            PlayingState = QMediaPlayer::PlayingState,
            PausedState = QMediaPlayer::PausedState
        };
        Q_ENUM(PlaybackState)

        Q_INVOKABLE MediaPlayer();
        ~MediaPlayer() override;

		qreal playbackRate() const;
		qreal volume() const;
		qint64 position() const;

        void setOnError(const QJSValue &onError) { mOnError = onError; }
        void setOnFinished(const QJSValue &onFinished) { mOnFinished = onFinished; }
        void setOnMediaStatusChanged(const QJSValue &onMediaStatusChanged) { mOnMediaStatusChanged = onMediaStatusChanged; }
        void setOnPlaybackStateChanged(const QJSValue &onPlaybackStateChanged) { mOnPlaybackStateChanged = onPlaybackStateChanged; }

        QJSValue onError() const { return mOnError; }
        QJSValue onFinished() const { return mOnFinished; }
        QJSValue onMediaStatusChanged() const { return mOnMediaStatusChanged; }
        QJSValue onPlaybackStateChanged() const { return mOnPlaybackStateChanged; }

        Q_INVOKABLE QString toString() const override                                { return QStringLiteral("MediaPlaylist"); }
        Q_INVOKABLE MediaPlayer *setPlaybackRate(qreal rate);
        Q_INVOKABLE MediaPlayer *setVolume(qreal volume);
        Q_INVOKABLE MediaPlayer *setPosition(qint64 position);
        Q_INVOKABLE MediaPlayer *setMuted(bool muted);
        Q_INVOKABLE qint64 duration() const;
        Q_INVOKABLE bool hasAudio() const;
        Q_INVOKABLE bool hasVideo() const;
        Q_INVOKABLE bool isMuted() const;
        Q_INVOKABLE bool isSeekable() const;
        Q_INVOKABLE int bufferStatus() const;
        Q_INVOKABLE Error error() const;
        Q_INVOKABLE QString errorString() const;
        Q_INVOKABLE MediaStatus mediaStatus() const;
        Q_INVOKABLE PlaybackState playbackState() const;
        Q_INVOKABLE MediaPlayer *play();
        Q_INVOKABLE MediaPlayer *pause();
        Q_INVOKABLE MediaPlayer *stop();
        Q_INVOKABLE MediaPlayer *setLocalMedia(const QString &path);
        Q_INVOKABLE MediaPlayer *setDistantMedia(const QString &url);

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

    private slots:
        void mediaErrorOccurred(QMediaPlayer::Error error, const QString &errorString);
        void mediaStatusChanged(QMediaPlayer::MediaStatus status);
        void playbackStateChanged(QMediaPlayer::PlaybackState state);

	private:
        QString playbackErrorString(const QString &errorString) const;

		QMediaPlayer *mMediaPlayer;
		QVideoWidget *mVideoWidget;
        QAudioOutput *mAudioOutput;
        QJSValue mOnError;
        QJSValue mOnFinished;
        QJSValue mOnMediaStatusChanged;
        QJSValue mOnPlaybackStateChanged;
	};
}
