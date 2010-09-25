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
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		
		Speak();
		~Speak();
		
	public slots:
		QScriptValue speak(const QString &text) const;
		bool isPlaying() const;
		QScriptValue stop() const;
		QScriptValue synchronize() const;
	};
}

#endif // SPEAK_H
