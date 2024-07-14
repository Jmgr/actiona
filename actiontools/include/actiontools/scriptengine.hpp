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

#include "actiontools_global.hpp"

#include <QJSEngine>

class RJSApi;

namespace ActionTools
{
    class ACTIONTOOLSSHARED_EXPORT ScriptEngine : public QObject
    {
        Q_OBJECT

    public:
        enum Context
        {
            Unknown,
            ActionInit,
            Parameters,
            Actions
        };

        explicit ScriptEngine(QObject *parent = nullptr);
        virtual ~ScriptEngine();

        QJSValue evaluate(const QString &program, const QString &fileName = QString(), int lineNumber = 1, QStringList *exceptionStackTrace = nullptr);
        bool isEvaluating() const { return mIsEvaluating; }
        std::tuple<int, int> currentLineAndColumn() const;
        QJSEngine &engine() { return *mEngine.get(); }
        QJSValue globalObject() const;
        void setInterrupted(bool interrupted);

        void setContext(Context context) { mContext = context; }
        Context context() const { return mContext; }

        static QJSEngine *current() { return mCurrent; }

    private:
        static QJSEngine *mCurrent;
        std::unique_ptr<QJSEngine> mEngine;
        std::unique_ptr<RJSApi> mQJsApi;
        bool mIsEvaluating{false};
        Context mContext{Unknown};
    };
}
