/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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

#ifndef NOTIFY_H
#define NOTIFY_H

#include "code/codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QStringList>

struct _NotifyNotification;

namespace Code
{
	class Notify : public CodeClass
	{
		Q_OBJECT
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		
		Notify();
		~Notify();
		
	public slots:
		QString toString() const					{ return "Notify"; }
        virtual bool equals(const QScriptValue &other) const { Q_UNUSED(other) return false; }
		QScriptValue show();
		
	private:
		_NotifyNotification *mNotification;
		QString mTitle;
		QString mText;
		QString mIcon;
		int mTimeout;
	};
}

#endif // NOTIFY_H
