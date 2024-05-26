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

#include "actiontools/actiontools_global.hpp"
#include "actiontools/windowhandle.hpp"
#include "actiontools/code/codeclass.hpp"

#include <QJSValue>
#include <QRegularExpression>

namespace Code
{
    class Point;
    class Size;

	class ACTIONTOOLSSHARED_EXPORT Window : public CodeClass
	{
		Q_OBJECT

	public:
		enum Mode
		{
            RegExp,
            Wildcard,
            WildcardUnix,
            FixedString
		};
		Q_ENUM(Mode)

        Q_INVOKABLE Window();
        Q_INVOKABLE Window(const Window &other);
		Window(const ActionTools::WindowHandle &windowHandle);
		
		Window &operator=(Window other);
		Window &operator=(ActionTools::WindowHandle windowHandle);
		
		void swap(Window &other);
		void swap(ActionTools::WindowHandle &windowHandle);
		
		const ActionTools::WindowHandle &windowHandle() const;

        Q_INVOKABLE QJSValue clone() const;
        Q_INVOKABLE bool equals(const QJSValue &other) const;
        Q_INVOKABLE QString toString() const override;
        Q_INVOKABLE bool isValid() const;
        Q_INVOKABLE QString title() const;
        Q_INVOKABLE QString className() const;
        Q_INVOKABLE bool isActive() const;
        Q_INVOKABLE QJSValue rect(bool useBorders = true) const;
        Q_INVOKABLE QJSValue process() const;
        Q_INVOKABLE Window *close();
        Q_INVOKABLE Window *killCreator();
        Q_INVOKABLE Window *setForeground();
        Q_INVOKABLE Window *minimize();
        Q_INVOKABLE Window *maximize();
        Q_INVOKABLE Window *move(const Point *point);
        Q_INVOKABLE Window *resize(const Size *size, bool useBorders = true);

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
		
	private:
		bool checkValidity() const;

		ActionTools::WindowHandle mWindowHandle;
	};

    class ACTIONTOOLSSHARED_EXPORT StaticWindow : public CodeClass
    {
        Q_OBJECT

    public:
        StaticWindow(QObject *parent): CodeClass(parent) {}

        Q_INVOKABLE QString toString() const override { return QStringLiteral("StaticWindow"); }
        Q_INVOKABLE QJSValue find(const QJSValue &parameters);
        Q_INVOKABLE QJSValue all();
        Q_INVOKABLE QJSValue foreground();
    };
}

