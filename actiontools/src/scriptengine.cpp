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

#include "actiontools/scriptengine.hpp"
#include "qtjsapi/RJSApi.h"

#include <private/qv4engine_p.h>

namespace ActionTools
{
    QJSEngine *ScriptEngine::mCurrent = nullptr;

    ScriptEngine::ScriptEngine(QObject *parent):
        QObject(parent),
        mEngine(std::make_unique<QJSEngine>()),
        mQJsApi(new RJSApi(mEngine.get()))
    {
        mQJsApi->init();
    }

    ScriptEngine::~ScriptEngine()
    {
    }

    QJSValue ScriptEngine::evaluate(const QString &program, const QString &fileName, int lineNumber, QStringList *exceptionStackTrace)
    {
        mIsEvaluating = true;
        mCurrent = mEngine.get();
        auto result = mEngine->evaluate(program, fileName, lineNumber, exceptionStackTrace);
        mCurrent = nullptr;
        mIsEvaluating = false;

        return result;
    }

    std::tuple<int, int> ScriptEngine::currentLineAndColumn() const
    {
        if(!mIsEvaluating)
            return {-1, -1};

        auto handle = mEngine->handle();
        if(!handle)
            return {-1, -1};

        auto trace = handle->stackTrace(1);
        if(trace.isEmpty())
            return {-1, -1};

        auto first = trace.first();

        return {first.line, first.column};
    }

    QJSValue ScriptEngine::globalObject() const
    {
        return mEngine->globalObject();
    }

    void ScriptEngine::setInterrupted(bool interrupted)
    {
        mEngine->setInterrupted(interrupted);
    }
}
