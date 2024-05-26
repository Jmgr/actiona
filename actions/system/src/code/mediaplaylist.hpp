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

class QMediaPlayer;
class QVideoWidget;

namespace Code
{
	class MediaPlaylist : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(qreal playbackRate READ playbackRate WRITE setPlaybackRate)
		Q_PROPERTY(qreal volume READ volume WRITE setVolume)
		Q_PROPERTY(qint64 position READ position WRITE setPosition)
		Q_PROPERTY(bool muted READ isMuted WRITE setMuted)

	public:
        Q_INVOKABLE MediaPlaylist();
		~MediaPlaylist() override;

		qreal playbackRate() const;
		qreal volume() const;
		qint64 position() const;

        Q_INVOKABLE QString toString() const override                                { return QStringLiteral("MediaPlaylist"); }
        Q_INVOKABLE MediaPlaylist *setPlaybackRate(qreal rate);
        Q_INVOKABLE MediaPlaylist *setVolume(qreal volume);
        Q_INVOKABLE MediaPlaylist *setPosition(qint64 position);
        Q_INVOKABLE MediaPlaylist *setMuted(bool muted);
        Q_INVOKABLE qint64 duration() const;
        Q_INVOKABLE bool hasAudio() const;
        Q_INVOKABLE bool hasVideo() const;
        Q_INVOKABLE bool isMuted() const;
        Q_INVOKABLE bool isSeekable() const;
        Q_INVOKABLE int bufferStatus() const;
        Q_INVOKABLE MediaPlaylist *play();
        Q_INVOKABLE MediaPlaylist *pause();
        Q_INVOKABLE MediaPlaylist *stop();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

	private:
		QMediaPlayer *mMediaPlayer;
		QVideoWidget *mVideoWidget;
	};
}

