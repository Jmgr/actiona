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

#include "codeinitializer.hpp"
#include "actiontools/code/codeclass.hpp"
#include "actiontools/code/image.hpp"
#include "actiontools/code/rawdata.hpp"
#include "actiontools/code/algorithms.hpp"
#include "actiontools/code/color.hpp"
#include "actiontools/code/point.hpp"
#include "actiontools/code/size.hpp"
#include "actiontools/code/rect.hpp"
#include "actiontools/code/processhandle.hpp"
#include "actiontools/code/window.hpp"
#include "actiontools/actionpack.hpp"
#include "actiontools/actionfactory.hpp"
#include "actiontools/scriptengine.hpp"
#include "codeexecution.hpp"
#include "codestdio.hpp"

#include <QFile>
#include <QUiLoader>
#include <QDir>
#include <QFileInfo>

namespace
{
    QString prefixFilenameWithCurrentPath(const QString &filename, QJSEngine *engine)
    {
        if(QDir::isAbsolutePath(filename))
            return filename;

		QJSValue executionObject = engine->globalObject().property(QStringLiteral("Execution"));
        if(executionObject.isNull())
            return filename;

		QString scriptOrCodeFilename = executionObject.property(QStringLiteral("filename")).toString();
        if(scriptOrCodeFilename.isEmpty())
            return filename;

        QDir scriptOrCodeFilenameDir = QFileInfo(scriptOrCodeFilename).absoluteDir();
        if(!scriptOrCodeFilenameDir.isReadable())
            return filename;

        QString newFilename = scriptOrCodeFilenameDir.absoluteFilePath(filename);
        if(!QFileInfo(newFilename).isReadable())
            return filename;

        return newFilename;
    }
}

namespace Execution
{
    QJSValue CodeGlobal::includeFunction(const QString &filepath)
    {
        QString filename = prefixFilenameWithCurrentPath(filepath, ActionTools::ScriptEngine::current());
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly))
        {
            Code::CodeClass::throwError(QStringLiteral("IncludeFileError"), QObject::tr("Unable to include file %1").arg(filename));
            return {};
        }

		QString fileContent = QString::fromUtf8(file.readAll());
        file.close();

        return ActionTools::ScriptEngine::current()->evaluate(fileContent, filename);
	}

    void CodeGlobal::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        auto class_ = scriptEngine.engine().newQObject(new CodeGlobal(&scriptEngine));
        scriptEngine.globalObject().setProperty(QStringLiteral("include"), class_.property(QStringLiteral("includeFunction")));
    }

    void CodeInitializer::initialize(ActionTools::ScriptEngine &scriptEngine, ActionTools::ActionFactory *actionFactory, const QString &filename)
	{
		Code::Window::registerClass(scriptEngine);
		Code::RawData::registerClass(scriptEngine);
		Code::Image::registerClass(scriptEngine);
		Code::Algorithms::registerClass(scriptEngine);
		Code::Color::registerClass(scriptEngine);
		Code::Point::registerClass(scriptEngine);
		Code::Size::registerClass(scriptEngine);
		Code::Rect::registerClass(scriptEngine);
		Code::ProcessHandle::registerClass(scriptEngine);

        CodeExecution::registerClass(scriptEngine);
        QJSValue executionObject = scriptEngine.globalObject().property(QStringLiteral("Execution"));
        executionObject.setProperty(QStringLiteral("filename"), filename);

        CodeStdio::registerClass(scriptEngine);
        CodeGlobal::registerClass(scriptEngine);

		int actionPackCount = actionFactory->actionPackCount();
		for(int actionPackIndex = 0; actionPackIndex < actionPackCount; ++actionPackIndex)
		{
			ActionTools::ActionPack *actionPack = actionFactory->actionPack(actionPackIndex);

			actionPack->codeInit(scriptEngine);
		}
	}
}
