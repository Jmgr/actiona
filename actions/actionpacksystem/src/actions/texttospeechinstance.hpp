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

#include "actiontools/actioninstance.hpp"
#include "actiontools/script.hpp"

#include <QTextToSpeech>

namespace Actions
{
    class TextToSpeechInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
        TextToSpeechInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
            : ActionTools::ActionInstance(definition, parent),
              mTextToSpeech(new QTextToSpeech(this))
		{
		}

        ~TextToSpeechInstance() override = default;

        void startExecution() override
		{
			bool ok = true;

            QString text = evaluateString(ok, QStringLiteral("text"));
            int volume = evaluateInteger(ok, QStringLiteral("volume"));
            QString language = evaluateString(ok, QStringLiteral("language"));
            bool blocking = evaluateBoolean(ok, QStringLiteral("blocking"));
            int playbackRate = evaluateInteger(ok, QStringLiteral("playbackRate"));
            int pitch = evaluateInteger(ok, QStringLiteral("pitch"));

			if(!ok)
				return;

            QLocale locale{language};

            if(language.isEmpty() || locale == QLocale::c())
                locale = QLocale::system();

            mTextToSpeech->setVolume(volume != -1 ? volume / 100. : 1.);
            mTextToSpeech->setRate((playbackRate - 100) / 100.);
            mTextToSpeech->setPitch((pitch - 100) / 100.);
            mTextToSpeech->setLocale(locale);
            mTextToSpeech->say(text);

            if(blocking)
                connect(mTextToSpeech, &QTextToSpeech::stateChanged, this, &TextToSpeechInstance::stateChanged);
            else
                executionEnded();
		}

        void stopLongTermExecution() override
        {
            mTextToSpeech->stop();
        }

        void stopExecution() override
        {
            mTextToSpeech->stop();
		}

        void pauseExecution() override
        {
            mTextToSpeech->pause();
        }

        void resumeExecution() override
        {
            mTextToSpeech->resume();
        }

	private slots:
        void stateChanged(QTextToSpeech::State state)
        {
            if(state == QTextToSpeech::Ready)
            {
                mTextToSpeech->disconnect();

                emit executionEnded();
            }
        }

	private:
        QTextToSpeech *mTextToSpeech;

        Q_DISABLE_COPY(TextToSpeechInstance)
	};
}

