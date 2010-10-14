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

#ifndef WINDOW_H
#define WINDOW_H

#include "actiontools_global.h"
#include "windowhandle.h"

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Window : public QObject, public QScriptable
	{
		Q_OBJECT
		Q_CLASSINFO("type", "CodeType")
		Q_ENUMS(Mode)
		
	public:
		enum Mode
		{
			RegExp = QRegExp::RegExp2,
			Wildcard = QRegExp::Wildcard,
			WildcardUnix = QRegExp::WildcardUnix,
			FixedString = QRegExp::FixedString
		};

		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue constructor(const ActionTools::WindowHandle &windowHandle, QScriptContext *context, QScriptEngine *engine);
		static ActionTools::WindowHandle parameter(QScriptContext *context);

		static QScriptValue find(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue all(QScriptContext *context, QScriptEngine *engine);
		
		Window();
		Window(const Window &other);
		Window(const ActionTools::WindowHandle &windowHandle);
		
		Window &operator=(Window other);
		Window &operator=(ActionTools::WindowHandle windowHandle);
		
		void swap(Window &other);
		void swap(ActionTools::WindowHandle &windowHandle);
		
		const ActionTools::WindowHandle &windowHandle() const;
		
	public slots:
		QScriptValue clone() const;
		bool equals(const QScriptValue &other) const;
		QString toString() const;
		bool isValid() const;
		QString title() const;
		QString className() const;
		QScriptValue rect() const;
		QScriptValue process() const;
		QScriptValue close() const;
		QScriptValue killCreator() const;
		QScriptValue setForeground() const;
		QScriptValue minimize() const;
		QScriptValue maximize() const;
		QScriptValue move() const;
		QScriptValue resize() const;
		
	private:
		bool checkValidity() const;

		ActionTools::WindowHandle mWindowHandle;
	};
}

#endif // WINDOW_H
