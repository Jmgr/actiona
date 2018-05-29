/*
	Actiona
	Copyright (C) 2005-2017 Jonathan Mercier-Ganady

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

#include "actioninstance.h"
#include "script.h"

#include <QTextToSpeech>

#include <QDebug>//TMP

namespace Actions
{
    class TextToSpeechInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
        /*
		enum Exceptions
		{
			FailedToStartException = ActionTools::ActionException::UserException
		};
        */

        TextToSpeechInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
            : ActionTools::ActionInstance(definition, parent),
              mTextToSpeech(new QTextToSpeech)
		{
		}

        virtual ~TextToSpeechInstance()
        {
            //HACK: deleting this here seems to crash the application
            mTextToSpeech->deleteLater();
        }

		void startExecution()
		{
			bool ok = true;

            QString text = evaluateString(ok, "text");

			if(!ok)
				return;

            for(auto engine: QTextToSpeech::availableEngines())
            {
                qDebug() << engine;
            }

            for(auto voice: mTextToSpeech->availableVoices())
            {
                qDebug() << voice.ageName(voice.age()) << voice.genderName(voice.gender()) << voice.name();
            }

            for(auto locale: mTextToSpeech->availableLocales())
            {
                qDebug() << locale;
            }

            connect(mTextToSpeech, SIGNAL(stateChanged(QTextToSpeech::State)), this, SLOT(stateChanged(QTextToSpeech::State)));

            mTextToSpeech->say(text);

            /*
			mProcess->setWorkingDirectory(workingDirectory);

			connect(mProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
			connect(mProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

			QStringList parameterList = parameters.split(QChar(' '));
			mProcess->start(command, parameters.isEmpty() ? QStringList() : parameterList);
			setVariable(mOutputVariable, QString());
			setVariable(mErrorOutputVariable, QString());
            */
		}

		void stopExecution()
        {
            mTextToSpeech->stop();
		}

        void pauseExecution()
        {
            mTextToSpeech->pause();
        }

        void resumeExecution()
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

