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

#include "speak.h"

extern "C"
{
#include "espeak/speak_lib.h"
}

#include <QScriptValueIterator>
#include <QDebug>

namespace Code
{
	QScriptValue Speak::constructor(QScriptContext *context, QScriptEngine *engine)
	{
#ifdef Q_WS_WIN
		QString dataDirectory = context->argumentCount() > 0 ? context->argument(0).toString() : "data";
#else
		QString dataDirectory = context->argumentCount() > 0 ? context->argument(0).toString() : QString();
#endif

		return engine->newQObject(new Speak(dataDirectory), QScriptEngine::ScriptOwnership);
	}

	Speak::Speak(const QString &dataDirectory)
		: QObject(),
		QScriptable(),
		mFrequency(-1)
	{
		if((mFrequency = espeak_Initialize(AUDIO_OUTPUT_SYNCH_PLAYBACK, 500, dataDirectory.isEmpty() ? 0 : dataDirectory.toUtf8(), 0)) == -1)
		{
			context()->throwError(tr("Initialize failed"));
			return;
		}
	}

	Speak::~Speak()
	{
		espeak_Terminate();
	}

	int Speak::rate() const
	{
		return espeak_GetParameter(espeakRATE, 1);
	}

	int Speak::volume() const
	{
		return espeak_GetParameter(espeakVOLUME, 1);
	}

	int Speak::pitch() const
	{
		return espeak_GetParameter(espeakPITCH, 1);
	}

	int Speak::range() const
	{
		return espeak_GetParameter(espeakRANGE, 1);
	}

	int Speak::wordGap() const
	{
		return espeak_GetParameter(espeakWORDGAP, 1);
	}

	int Speak::frequency() const
	{
		return mFrequency;
	}

	bool Speak::isPlaying() const
	{
		return espeak_IsPlaying();
	}

	QScriptValue Speak::speak(const QString &text) const
	{
		int result = espeak_Synth(text.toUtf8().data(), text.toUtf8().size(), 0, POS_CHARACTER, 0, espeakCHARS_UTF8 | espeakENDPAUSE, 0, 0);
		switch(result)
		{
		case EE_BUFFER_FULL:
			context()->throwError(tr("Speak failed : buffer is full"));
			return context()->thisObject();
		case EE_INTERNAL_ERROR:
			context()->throwError(tr("Speak failed"));
			return context()->thisObject();
		default:
			break;
		}

		return context()->thisObject();
	}

	QScriptValue Speak::setVoice(const QScriptValue &parameters) const
	{
		QScriptValueIterator it(parameters);
		espeak_VOICE voice;
		std::memset(&voice, 0, sizeof(espeak_VOICE));

		std::string name;
		std::string language;

		while(it.hasNext())
		{
			it.next();

			if(it.name() == "name" && !it.value().toString().isEmpty())
				name = it.value().toString().toStdString();
			else if(it.name() == "language" && !it.value().toString().isEmpty())
				language = it.value().toString().toStdString();
			else if(it.name() == "gender")
				voice.gender = it.value().toInt32();
			else if(it.name() == "age")
				voice.age = it.value().toInt32();
			else if(it.name() == "variant")
				voice.variant = it.value().toInt32();
		}

		if(name.size() > 0)
			voice.name = name.c_str();
		if(language.size() > 0)
			voice.languages = language.c_str();

		if(espeak_SetVoiceByProperties(&voice) != EE_OK)
		{
			context()->throwError(tr("Unable to set the voice"));
			return context()->thisObject();
		}

		return context()->thisObject();
	}

	QScriptValue Speak::setRate(int rate) const
	{
		if(espeak_SetParameter(espeakRATE, rate, 0) != EE_OK)
		{
			context()->throwError(tr("Failed to set the rate"));
			return context()->thisObject();
		}

		return context()->thisObject();
	}

	QScriptValue Speak::setVolume(int volume) const
	{
		if(volume < 0 || volume > 100 || espeak_SetParameter(espeakVOLUME, volume, 0) != EE_OK)
		{
			context()->throwError(tr("Failed to set the volume"));
			return context()->thisObject();
		}

		return context()->thisObject();
	}

	QScriptValue Speak::setPitch(int pitch) const
	{
		if(pitch < 0 || pitch > 100 || espeak_SetParameter(espeakPITCH, pitch, 0) != EE_OK)
		{
			context()->throwError(tr("Failed to set the pitch"));
			return context()->thisObject();
		}

		return context()->thisObject();
	}

	QScriptValue Speak::setRange(int range) const
	{
		if(range < 0 || range > 100 || espeak_SetParameter(espeakRANGE, range, 0) != EE_OK)
		{
			context()->throwError(tr("Failed to set the range"));
			return context()->thisObject();
		}

		return context()->thisObject();
	}

	QScriptValue Speak::setWordGap(int wordGap) const
	{
		if(espeak_SetParameter(espeakWORDGAP, wordGap, 0) != EE_OK)
		{
			context()->throwError(tr("Failed to set the word gap"));
			return context()->thisObject();
		}

		return context()->thisObject();
	}

	QScriptValue Speak::stop() const
	{
		if(espeak_Cancel() != EE_OK)
		{
			context()->throwError(tr("Stop failed"));
			return context()->thisObject();
		}

		return context()->thisObject();
	}
}
