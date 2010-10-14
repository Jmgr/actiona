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

#ifndef SPEAK_H
#define SPEAK_H

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
	class Speak : public QObject, public QScriptable
	{
		Q_OBJECT
		Q_CLASSINFO("type", "CodeClass")
		Q_ENUMS(Gender)

	public:
		enum Gender
		{
			NotSpecified,
			Male,
			Female
		};

		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

		Speak();
		~Speak();

	public slots:
		QString toString() const					{ return "Speak"; }
		int rate() const;
		int volume() const;
		int pitch() const;
		int range() const;
		int wordGap() const;
		int frequency() const;
		bool isPlaying() const;
		QScriptValue speak(const QString &text) const;
		QScriptValue setVoice(const QScriptValue &parameters) const;
		QScriptValue setRate(int rate) const;
		QScriptValue setVolume(int volume) const;
		QScriptValue setPitch(int pitch) const;
		QScriptValue setRange(int range) const;
		QScriptValue setWordGap(int wordGap) const;
		QScriptValue stop() const;

	private:
		void setVoicePrivate(QScriptContext *context, const QScriptValue &parameters) const;
		
		int mFrequency;
	};
}

#endif // SPEAK_H
