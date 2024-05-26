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
#include "actiontools/scriptengine.hpp"

#include <QObject>
#include <QByteArray>

class QJSValue;

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT CodeClass : public QObject
	{
		Q_OBJECT

        Q_INVOKABLE virtual QString toString() const = 0;

    public:
        void throwError(const QString &errorType, const QString &message) const;

        template<class T, typename U>
        static QJSValue construct(U &&param, ActionTools::ScriptEngine &engine)
        {
            return engine.engine().newQObject(new T(param));
        }

	protected:
		enum Encoding
		{
			Native,
			Ascii,
			Latin1,
			UTF8
		};

        explicit CodeClass(QObject *parent = nullptr);
		~CodeClass() override = default;

        template<class T, class... Args>
        static void registerStaticClass(const QString &name, ActionTools::ScriptEngine &engine, Args&&... args)
        {
            engine.globalObject().setProperty(name, engine.engine().newQObject(new T(&engine, std::forward<Args>(args)...)));
        }
        template<class T>
        static void registerClass(const QString &name, ActionTools::ScriptEngine &engine)
        {
            engine.globalObject().setProperty(name, engine.engine().newQMetaObject(&T::staticMetaObject));
        }
        template<class T, class ST>
        static void registerClassWithStaticFunctions(const QString &name, const QStringList &staticFunctions, ActionTools::ScriptEngine &engine)
        {
            // Ugly hack to get static functions.
            auto class_ = engine.engine().newQMetaObject(&T::staticMetaObject);
            auto staticClass_ = engine.engine().newQObject(new ST(&engine));
            for(const auto &staticFunction: staticFunctions)
            {
                class_.setProperty(staticFunction, staticClass_.property(staticFunction));
            }
            engine.globalObject().setProperty(name, class_);
        }

        template<class T>
        QJSValue clone() const
        {
            return ActionTools::ScriptEngine::current()->newQObject(new T(*static_cast<const T*>(this)));
        }
        template<class T, typename U>
        QJSValue construct(U &&param) const
        {
            return ActionTools::ScriptEngine::current()->newQObject(new T(param));
        }

		static QByteArray toEncoding(const QString &string, Encoding encoding);
		static QString fromEncoding(const QByteArray &byteArray, Encoding encoding);
		static QStringList arrayParameterToStringList(const QJSValue &scriptValue);
	};

    std::tuple<bool, QString, int> ACTIONTOOLSSHARED_EXPORT checkSyntax(const QString &program);
}
