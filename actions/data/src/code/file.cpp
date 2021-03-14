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

#include "file.hpp"
#include "actiontools/code/rawdata.hpp"
#include "backend/system.hpp"

#include <QScriptValueIterator>
#include <QProcess>
#include <QDir>
#include <QDebug>

namespace Code
{
    Backend::System::FileOperationParameters parametersFromScriptOptions(const QScriptValue &options)
    {
        QScriptValueIterator it(options);

        Backend::System::FileOperationParameters parameters;

        while(it.hasNext())
        {
            it.next();

            if(it.name() == QLatin1String("noErrorDialog"))
                parameters.noErrorDialog = it.value().toBool();
            else if(it.name() == QLatin1String("noConfirmDialog"))
                parameters.noConfirmDialog = it.value().toBool();
            else if(it.name() == QLatin1String("noProgressDialog"))
                parameters.noProgressDialog = it.value().toBool();
            else if(it.name() == QLatin1String("allowUndo"))
                parameters.allowUndo = it.value().toBool();
            else if(it.name() == QLatin1String("createDestinationDirectory"))
                parameters.createDestinationDirectory = it.value().toBool();
        }

        return parameters;
    }

    std::tuple<QString, QString, Backend::System::FileOperationParameters> parametersFromScriptContext(const QScriptValue &options, QScriptContext *context)
    {
        auto source = context->argument(0).toString();
        auto destination = context->argument(1).toString();

        return {source, destination, parametersFromScriptOptions(options)};
    }

	QScriptValue File::constructor(QScriptContext *context, QScriptEngine *engine)
    {
        return CodeClass::constructor(new File, context, engine);
	}

	QScriptValue File::copy(QScriptContext *context, QScriptEngine *engine)
	{
        if(context->argumentCount() < 2)
        {
            throwError(context, engine, QStringLiteral("ParameterCountError"), QObject::tr("Incorrect parameter count"));
            return engine->undefinedValue();
        }

        auto [source, destination, parameters] = parametersFromScriptContext(context->argument(2), context);

        try
        {
            Backend::Instance::system().copyFiles(source, destination, parameters);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(context, engine, QStringLiteral("CopyError"), QObject::tr("Failed copying files: %1").arg(e.what()));
        }

		return engine->undefinedValue();
	}

	QScriptValue File::exists(QScriptContext *context, QScriptEngine *engine)
	{
		if(context->argumentCount() != 1)
		{
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
            return engine->undefinedValue();
		}

		return QFile::exists(context->argument(0).toString());
	}

	QScriptValue File::move(QScriptContext *context, QScriptEngine *engine)
	{
        if(context->argumentCount() < 2)
        {
            throwError(context, engine, QStringLiteral("ParameterCountError"), QObject::tr("Incorrect parameter count"));
            return engine->undefinedValue();
        }

        auto [source, destination, parameters] = parametersFromScriptContext(context->argument(2), context);

        try
        {
            Backend::Instance::system().moveFiles(source, destination, parameters);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(context, engine, QStringLiteral("MoveError"), QObject::tr("Failed moving files: %1").arg(e.what()));
        }

		return engine->undefinedValue();
	}

	QScriptValue File::rename(QScriptContext *context, QScriptEngine *engine)
	{
        if(context->argumentCount() < 2)
        {
            throwError(context, engine, QStringLiteral("ParameterCountError"), QObject::tr("Incorrect parameter count"));
            return engine->undefinedValue();
        }

        auto [source, destination, parameters] = parametersFromScriptContext(context->argument(2), context);

        try
        {
            Backend::Instance::system().renameFiles(source, destination, parameters);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(context, engine, QStringLiteral("RenameError"), QObject::tr("Failed renaming files: %1").arg(e.what()));
        }

		return engine->undefinedValue();
	}

	QScriptValue File::remove(QScriptContext *context, QScriptEngine *engine)
	{
		if(context->argumentCount() < 1)
		{
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
            return engine->undefinedValue();
		}

        auto [source, _, parameters] = parametersFromScriptContext(context->argument(1), context);

        try
        {
            Backend::Instance::system().removeFiles(source, parameters);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(context, engine, QStringLiteral("RemoveError"), QObject::tr("Failed removing files: %1").arg(e.what()));
        }

		return engine->undefinedValue();
	}

	void File::registerClass(QScriptEngine *scriptEngine)
	{
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&copy, QStringLiteral("copy"), scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&move, QStringLiteral("move"), scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&rename, QStringLiteral("rename"), scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&remove, QStringLiteral("remove"), scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&exists, QStringLiteral("exists"), scriptEngine);
	}

	QScriptValue File::open(const QString &filename, OpenMode mode)
	{
		mFile.setFileName(filename);
	
		if(!mFile.open(static_cast<QIODevice::OpenMode>(mode)))
			throwError(QStringLiteral("CannotOpenFileError"), tr("Unable to open file"));
	
		return thisObject();
	}
	
	QScriptValue File::write(const QScriptValue &data)
	{
		QObject *object = data.toQObject();
		if(auto rawData = qobject_cast<RawData*>(object))
		{
			if(mFile.write(rawData->byteArray()) == -1)
				throwError(QStringLiteral("WriteFailedError"), tr("Write failed"));
		}
		else
		{
			if(mFile.write(data.toVariant().toByteArray()) == -1)
				throwError(QStringLiteral("WriteFailedError"), tr("Write failed"));
		}
	
		return thisObject();
	}
	
	QScriptValue File::writeText(const QString &value, Encoding encoding)
	{
		if(mFile.write(toEncoding(value, encoding)) == -1)
			throwError(QStringLiteral("WriteFailedError"), tr("Write failed"));
	
		return thisObject();
	}
	
	QScriptValue File::read()
	{
		return RawData::constructor(mFile.readAll(), engine());
	}
	
	QString File::readText(Encoding encoding)
	{
		return fromEncoding(mFile.readAll(), encoding);
	}
	
	QScriptValue File::close()
	{
		mFile.close();
	
		return thisObject();
	}
	
	QScriptValue File::copy(const QString &destination, const QScriptValue &options) const
	{
        auto parameters = parametersFromScriptOptions(options);

        try
        {
            Backend::Instance::system().copyFiles(mFile.fileName(), destination, parameters);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("CopyError"), QObject::tr("Failed copying files: %1").arg(e.what()));
        }

        return thisObject();
	}
	
	QScriptValue File::move(const QString &destination, const QScriptValue &options)
	{
		mFile.close();
	
        auto parameters = parametersFromScriptOptions(options);

        try
        {
            Backend::Instance::system().moveFiles(mFile.fileName(), destination, parameters);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("MoveError"), QObject::tr("Failed moving files: %1").arg(e.what()));
        }

        return thisObject();
	}
	
	QScriptValue File::rename(const QString &destination, const QScriptValue &options)
	{
        auto parameters = parametersFromScriptOptions(options);

        try
        {
            Backend::Instance::system().renameFiles(mFile.fileName(), destination, parameters);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("RenameError"), QObject::tr("Failed moving files: %1").arg(e.what()));
        }

        return thisObject();
	}
	
	QScriptValue File::remove(const QScriptValue &options)
	{
		mFile.close();

        auto parameters = parametersFromScriptOptions(options);

        try
        {
            Backend::Instance::system().removeFiles(mFile.fileName(), parameters);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("RemoveError"), QObject::tr("Failed removing files: %1").arg(e.what()));
        }

        return thisObject();
	}
}
