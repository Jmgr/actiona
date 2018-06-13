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

	Contact : jmgr@jmgr.info
*/

#pragma once

#include "actiontools_global.h"
#include "windowhandle.h"
#include "code/codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Window : public CodeClass
	{
		Q_OBJECT
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
		static QScriptValue constructor(const ActionTools::WindowHandle &windowHandle, QScriptEngine *engine);
		static ActionTools::WindowHandle parameter(QScriptContext *context, QScriptEngine *engine);

		static QScriptValue find(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue all(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue foreground(QScriptContext *context, QScriptEngine *engine);

		static void registerClass(QScriptEngine *scriptEngine);

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
		bool equals(const QScriptValue &other) const override;
		QString toString() const override;
		bool isValid() const;
		QString title() const;
		QString className() const;
		bool isActive() const;
		QScriptValue rect(bool useBorders = true) const;
		QScriptValue process() const;
		QScriptValue close() const;
		QScriptValue killCreator() const;
		QScriptValue setForeground() const;
		QScriptValue minimize() const;
		QScriptValue maximize() const;
		QScriptValue move() const;
		QScriptValue resize(bool useBorders = true) const;
		
	private:
		bool checkValidity() const;

		ActionTools::WindowHandle mWindowHandle;
	};
}

