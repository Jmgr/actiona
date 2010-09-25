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
#include <espeak/speak_lib.h>
}

namespace Code
{
	QScriptValue Speak::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
	
		return engine->newQObject(new Speak, QScriptEngine::ScriptOwnership);
	}
	
	Speak::Speak()
	{
		espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 500, 0, 0);
		espeak_VOICE voice;
		voice.name = 0;
		voice.languages = "fr";
		voice.gender = 0;
		voice.age = 0;
		voice.variant = 0;
		espeak_SetVoiceByProperties(&voice);
	}
	
	Speak::~Speak()
	{
		espeak_Terminate();
	}
	
	QScriptValue Speak::speak(const QString &text) const
	{
		espeak_Synth(text.toUtf8().data(), text.toUtf8().size(), 0, POS_CHARACTER, 0, espeakCHARS_UTF8 | espeakENDPAUSE, 0, 0);
		
		return context()->thisObject();
	}
	
	bool Speak::isPlaying() const
	{
		return espeak_IsPlaying();
	}
	
	QScriptValue Speak::stop() const
	{
		espeak_Cancel();
		
		return context()->thisObject();
	}
	
	QScriptValue Speak::synchronize() const
	{
		espeak_Synchronize();
		
		return context()->thisObject();
	}
}
