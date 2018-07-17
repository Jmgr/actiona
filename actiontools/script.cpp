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

#include "script.h"
#include "actioninstance.h"
#include "actiondefinition.h"
#include "elementdefinition.h"
#include "actionfactory.h"
#include "parameter.h"
#include "subparameter.h"
#include "messagehandler.h"
#include "variableparameterdefinition.h"
#include "groupdefinition.h"
#include "scriptlinemodel.h"

#ifdef ACT_PROFILE
#include "highresolutiontimer.h"
#endif

#include <QIODevice>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QBuffer>

namespace ActionTools
{
	const QRegExp Script::CodeVariableDeclarationRegExp(QStringLiteral("^[ \t]*var ([A-Za-z_][A-Za-z0-9_]*)"), Qt::CaseSensitive, QRegExp::RegExp2);

	Script::Script(ActionFactory *actionFactory, QObject *parent)
		: QObject(parent),
		mActionFactory(actionFactory),
        mLineModel{new ScriptLineModel(*this, this)}
	{
	}

	Script::~Script()
	{
		qDeleteAll(mActionInstances);
    }

    void Script::appendAction(ActionInstance *actionInstance)
    {
        mActionInstances.append(actionInstance);

        mRebuildLabelList = true;
    }

    ActionInstance *Script::appendAction(const QString &actionDefinitionId)
    {
        ActionInstance *actionInstance = mActionFactory->newActionInstance(actionDefinitionId);
		if(!actionInstance)
			return nullptr;

		appendAction(actionInstance);

		return actionInstance;
	}

	ActionInstance *Script::actionAt(int line) const
	{
		if(line < 0 || line >= mActionInstances.size())
			return nullptr;

		return mActionInstances.at(line);
	}

	void Script::insertAction(int line, ActionInstance *actionInstance)
	{
		mActionInstances.insert(line, actionInstance);

        mRebuildLabelList = true;
	}

	void Script::setAction(int line, ActionInstance *actionInstance)
	{
		if(line < 0 || line >= mActionInstances.size())
			return;

		delete mActionInstances.at(line);

		mActionInstances[line] = actionInstance;

        mRebuildLabelList = true;
	}

	void Script::removeActions(int line, int count)
	{
		if(count <= 0 || line < 0)
			return;

		for(int r = line; r <= line + count - 1; ++r)
			removeAction(r);

        mRebuildLabelList = true;
	}

	void Script::removeAction(int line)
	{
		if(line < 0 || line >= mActionInstances.size())
			return;

		delete mActionInstances.takeAt(line);

        mRebuildLabelList = true;
	}

	void Script::removeAction(ActionInstance *actionInstance)
	{
		int index = mActionInstances.indexOf(actionInstance);

		if(index == -1)
			return;

		removeAction(index);
	}

	void Script::removeAll()
	{
		qDeleteAll(mActionInstances);
		mActionInstances.clear();
        mRebuildLabelList = true;
	}

	void Script::moveAction(int startLine, int endLine)
	{
		if(startLine < 0 || startLine >= mActionInstances.count() ||
		   endLine < 0 || startLine == endLine)
		return;

		if(endLine >= mActionInstances.count())
			mActionInstances.append(mActionInstances.takeAt(startLine));
		else
			mActionInstances.move(startLine, endLine);
	}

	int Script::labelLine(const QString &label) const
	{
		for(int i = 0; i < mActionInstances.count(); ++i)
		{
			if(mActionInstances.at(i)->label() == label)
				return i;
		}

		return -1;
	}

	bool Script::hasEnabledActions() const
	{
        for(ActionInstance *actionInstance: mActionInstances)
		{
			if(actionInstance->isEnabled() && actionInstance->definition()->worksUnderThisOS())
				return true;
		}

		return false;
	}

	QSet<int> Script::usedActions() const
	{
		QSet<int> result;
		int actionCount = mActionFactory->actionDefinitionCount();
		QStringList actions;

		for(int actionIndex = 0; actionIndex < actionCount; ++actionIndex)
			actions << mActionFactory->actionDefinition(actionIndex)->id();

		//First add all the actions contained in the script, then search in all data fields that are in code mode
        for(ActionInstance *actionInstance: mActionInstances)
		{
			result << actionInstance->definition()->index();

			const ParametersData &parametersData = actionInstance->parametersData();
            for(const Parameter &parameter: parametersData)
			{
                for(const SubParameter &subParameter: parameter.subParameters())
				{
					if(subParameter.isCode())
					{
                        const QString &value = subParameter.value();

						for(int actionIdIndex = 0; actionIdIndex < actions.count(); ++actionIdIndex)
						{
							if(value.contains(actions.at(actionIdIndex)))
								result << actionIdIndex;
						}
					}
				}
			}
		}

		return result;
	}

    bool Script::write(QIODevice *device, const Tools::Version &programVersion, const Tools::Version &scriptVersion, std::function<void(int, int, QString)> *progressCallback)
	{
#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("Script::write");
#endif
        if(progressCallback)
        {
            progressCallback->operator()(0, 0, tr("Writing..."));
        }

		QXmlStreamWriter stream(device);
		stream.setAutoFormatting(true);

		stream.writeStartDocument();
		stream.writeStartElement(QStringLiteral("scriptfile"));

		QString osName = tr("Unknown");
    #ifdef Q_OS_LINUX
		osName = tr("GNU/Linux");
	#endif
    #ifdef Q_OS_WIN
		osName = tr("Windows");
	#endif
    #ifdef Q_OS_MAC
		osName = tr("Mac");
	#endif

		stream.writeStartElement(QStringLiteral("settings"));

		stream.writeAttribute(QStringLiteral("program"), QStringLiteral("actiona"));
		stream.writeAttribute(QStringLiteral("version"), programVersion.toString());
		stream.writeAttribute(QStringLiteral("scriptVersion"), scriptVersion.toString());
		stream.writeAttribute(QStringLiteral("os"), osName);

		stream.writeEndElement();

		stream.writeStartElement(QStringLiteral("actions"));

        for(int actionIndex: usedActions())
		{
			ActionDefinition *actionDefinition = mActionFactory->actionDefinition(actionIndex);

			stream.writeStartElement(QStringLiteral("action"));
			stream.writeAttribute(QStringLiteral("name"), actionDefinition->id());
			stream.writeAttribute(QStringLiteral("version"), actionDefinition->version().toString());
			stream.writeEndElement();
		}

		stream.writeEndElement();

		stream.writeStartElement(QStringLiteral("parameters"));

        int parameterIndex = 0;
        for(const ScriptParameter &parameter: mParameters)
		{
            if(progressCallback)
            {
                progressCallback->operator()(parameterIndex, mParameters.size() - 1, tr("Writing parameters..."));
            }

			stream.writeStartElement(QStringLiteral("parameter"));
			stream.writeAttribute(QStringLiteral("name"), parameter.name());
			stream.writeAttribute(QStringLiteral("code"), QStringLiteral("%1").arg(parameter.isCode()));
			stream.writeAttribute(QStringLiteral("type"), QString::number(parameter.type()));
			stream.writeCharacters(parameter.value());
			stream.writeEndElement();

            ++parameterIndex;
		}

		stream.writeEndElement();

		stream.writeStartElement(QStringLiteral("resources"));

		QMap<QString, Resource>::const_iterator resourceIt = mResources.constBegin();
        int resourceIndex = 0;
        while(resourceIt != mResources.constEnd())
        {
            if(progressCallback)
            {
                progressCallback->operator()(resourceIndex , mResources.size() - 1, tr("Writing resources..."));
            }

			stream.writeStartElement(QStringLiteral("resource"));
			stream.writeAttribute(QStringLiteral("id"), resourceIt.key());
			stream.writeAttribute(QStringLiteral("type"), QString::number(resourceIt.value().type()));
			stream.writeCharacters(QLatin1String(qCompress(resourceIt.value().data()).toBase64()));
            stream.writeEndElement();

            ++resourceIt;
            ++resourceIndex;
        }

        stream.writeEndElement();

		stream.writeStartElement(QStringLiteral("script"));
		stream.writeAttribute(QStringLiteral("pauseBefore"), QString::number(pauseBefore()));
		stream.writeAttribute(QStringLiteral("pauseAfter"), QString::number(pauseAfter()));

        int actionIndex = 0;
        for(ActionInstance *actionInstance: mActionInstances)
		{
            if(progressCallback)
            {
                progressCallback->operator()(actionIndex, mActionInstances.size() - 1, tr("Writing actions..."));
            }

			stream.writeStartElement(QStringLiteral("action"));
			stream.writeAttribute(QStringLiteral("name"), actionInstance->definition()->id());

			if(!actionInstance->label().isEmpty())
				stream.writeAttribute(QStringLiteral("label"), actionInstance->label());
			if(!actionInstance->comment().isEmpty())
				stream.writeAttribute(QStringLiteral("comment"), actionInstance->comment());
			if(actionInstance->color().isValid() && actionInstance->color() != Qt::transparent)
				stream.writeAttribute(QStringLiteral("color"), actionInstance->color().name());
			if(!actionInstance->isEnabled())
				stream.writeAttribute(QStringLiteral("enabled"), QVariant(actionInstance->isEnabled()).toString());
			if(actionInstance->pauseBefore() != 0)
				stream.writeAttribute(QStringLiteral("pauseBefore"), QVariant(actionInstance->pauseBefore()).toString());
			if(actionInstance->pauseAfter() != 0)
				stream.writeAttribute(QStringLiteral("pauseAfter"), QVariant(actionInstance->pauseAfter()).toString());
			if(actionInstance->timeout() != 0)
				stream.writeAttribute(QStringLiteral("timeout"), QVariant(actionInstance->timeout()).toString());

			const ExceptionActionInstancesHash &exceptionActionsHash = actionInstance->exceptionActionInstances();
            for(ActionException::Exception exception: exceptionActionsHash.keys())
			{
				ActionException::ExceptionActionInstance exceptionActionInstance = exceptionActionsHash.value(exception);
				stream.writeStartElement(QStringLiteral("exception"));
				stream.writeAttribute(QStringLiteral("id"), QString::number(static_cast<int>(exception)));
				stream.writeAttribute(QStringLiteral("action"), QString::number(static_cast<int>(exceptionActionInstance.action())));
				stream.writeAttribute(QStringLiteral("line"), exceptionActionInstance.line());
				stream.writeEndElement();
			}

			const ParametersData &parametersData = actionInstance->parametersData();
            for(const QString &parameter: parametersData.keys())
			{
				const Parameter &parameterData = parametersData.value(parameter);

				stream.writeStartElement(QStringLiteral("parameter"));
				stream.writeAttribute(QStringLiteral("name"), parameter);

                for(const QString &subParameter: parameterData.subParameters().keys())
				{
					const SubParameter &subParameterData = parameterData.subParameters().value(subParameter);

					stream.writeStartElement(QStringLiteral("subParameter"));
					stream.writeAttribute(QStringLiteral("name"), subParameter);

					stream.writeAttribute(QStringLiteral("code"), QStringLiteral("%1").arg(subParameterData.isCode()));
                    stream.writeCharacters(subParameterData.value());

					stream.writeEndElement();
				}

				stream.writeEndElement();
			}

			stream.writeEndElement();

            ++actionIndex;
		}

		stream.writeEndElement();

		stream.writeEndElement();
		stream.writeEndDocument();

		return true;
	}

    Script::ReadResult Script::read(
            QIODevice *device,
            const Tools::Version &scriptVersion,
            std::function<void(int, int, QString)> *progressCallback,
            std::function<void()> *resetCallback,
            std::function<void(QList<ActionTools::ActionInstance *>)> *addActionsCallback)
	{
#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("Script::read");
#endif
		mMissingActions.clear();

        if(progressCallback)
        {
            progressCallback->operator()(0, 0, tr("Reading schema..."));
        }

        ReadResult result = validateSchema(device, scriptVersion);
        if(result != ReadSuccess)
            return result;

        if(progressCallback)
        {
            progressCallback->operator()(0, 0, tr("Listing file content..."));
        }

        //List the script content
        device->reset();

        int parameterCount = 0;
        int resourceCount = 0;
        int actionCount = 0;

        {
#ifdef ACT_PROFILE
            Tools::HighResolutionTimer timer("Listing script content");
#endif

            QXmlStreamReader stream(device);
            while(!stream.atEnd() && !stream.hasError())
            {
                stream.readNext();

                if(stream.isStartDocument() || !stream.isStartElement())
                    continue;

				if(stream.name() == QLatin1String("parameters"))
                {
                    stream.readNext();

					for(;!stream.isEndElement() || stream.name() != QLatin1String("parameters");stream.readNext())
                    {
                        if(!stream.isStartElement())
                            continue;

                        ++parameterCount;
                    }
                }
				else if(stream.name() == QLatin1String("resources"))
                {
                    stream.readNext();

					for(;!stream.isEndElement() || stream.name() != QLatin1String("resources");stream.readNext())
                    {
                        if(!stream.isStartElement())
                            continue;

                        ++resourceCount;
                    }
                }
				else if(stream.name() == QLatin1String("script"))
                {
                    stream.readNext();

					for(;!stream.isEndElement() || stream.name() != QLatin1String("script");stream.readNext())
                    {
                        if(!stream.isStartElement())
                            continue;

                        ++actionCount;
                    }
                }
            }
        }

		if(resetCallback)
			(*resetCallback)();
		else
			removeAll();

		mParameters.clear();
        mResources.clear();

		device->reset();

#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer2("Reading content");
#endif

        if(progressCallback)
        {
            progressCallback->operator()(0, 0, tr("Reading content..."));
        }

        QHash<ActionDefinition *, Tools::Version> updatableActionDefinitions;
		QList<ActionInstance *> newActions;

		QXmlStreamReader stream(device);
		while(!stream.atEnd() && !stream.hasError())
		{
			stream.readNext();

			if(stream.isStartDocument())
				continue;

			if(!stream.isStartElement())
				continue;

			if(stream.name() == QLatin1String("settings"))
			{
				const QXmlStreamAttributes &attributes = stream.attributes();
				mProgramName = attributes.value(QStringLiteral("program")).toString();
#if (QT_VERSION >= 0x050600)
				mProgramVersion = QVersionNumber::fromString(attributes.value(QStringLiteral("version")).toString());
				mScriptVersion = QVersionNumber::fromString(attributes.value(QStringLiteral("scriptVersion")).toString());
#else
                mProgramVersion = Tools::Version(attributes.value("version").toString());
                mScriptVersion = Tools::Version(attributes.value("scriptVersion").toString());
#endif
				mOs = attributes.value(QStringLiteral("os")).toString();

				if(mScriptVersion > scriptVersion)
					return ReadInvalidScriptVersion;
			}
			else if(stream.name() == QLatin1String("actions"))
			{
				stream.readNext();

				for(;!stream.isEndElement() || stream.name() != QLatin1String("actions");stream.readNext())
				{
					if(!stream.isStartElement())
						continue;

					const QXmlStreamAttributes &attributes = stream.attributes();
					QString name = attributes.value(QStringLiteral("name")).toString();
#if (QT_VERSION >= 0x050600)
					Tools::Version version = QVersionNumber::fromString(attributes.value(QStringLiteral("version")).toString());
#else
                    Tools::Version version(attributes.value("version").toString());
#endif

					ActionDefinition *actionDefinition = mActionFactory->actionDefinition(name);
					if(!actionDefinition)
						mMissingActions << name;
                    else if(actionDefinition->version() > version)
                        updatableActionDefinitions[actionDefinition] = version;
				}
			}
			else if(stream.name() == QLatin1String("parameters"))
			{
				stream.readNext();

				for(;!stream.isEndElement() || stream.name() != QLatin1String("parameters");stream.readNext())
				{
					if(!stream.isStartElement())
						continue;

                    if(progressCallback)
                    {
                        progressCallback->operator()(mParameters.size(), parameterCount - 1, tr("Reading parameters..."));
                    }

					const QXmlStreamAttributes &attributes = stream.attributes();
					ScriptParameter scriptParameter(	attributes.value(QStringLiteral("name")).toString(),
														stream.readElementText(),
														QVariant(attributes.value(QStringLiteral("code")).toString()).toBool(),
														static_cast<ScriptParameter::ParameterType>(attributes.value(QStringLiteral("type")).toString().toInt()));

					mParameters.append(scriptParameter);
				}
			}
			else if(stream.name() == QLatin1String("resources"))
            {
                stream.readNext();

				for(;!stream.isEndElement() || stream.name() != QLatin1String("resources");stream.readNext())
                {
                    if(!stream.isStartElement())
                        continue;

                    if(progressCallback)
                    {
                        progressCallback->operator()(mResources.size(), resourceCount - 1, tr("Reading resources..."));
                    }

                    const QXmlStreamAttributes &attributes = stream.attributes();
					QString id = attributes.value(QStringLiteral("id")).toString();
                    QString base64Data = stream.readElementText();
                    QByteArray data = qUncompress(QByteArray::fromBase64(base64Data.toLatin1()));
					Resource resource(data, static_cast<Resource::Type>(attributes.value(QStringLiteral("type")).toString().toInt()));

                    mResources.insert(id, resource);
                }
            }
			else if(stream.name() == QLatin1String("script"))
			{
				const QXmlStreamAttributes &attributes = stream.attributes();
				setPauseBefore(attributes.value(QStringLiteral("pauseBefore")).toString().toInt());
				setPauseAfter(attributes.value(QStringLiteral("pauseAfter")).toString().toInt());

				stream.readNext();

				for(;!stream.isEndElement() || stream.name() != QLatin1String("script");stream.readNext())
				{
					if(!stream.isStartElement())
						continue;

					const QXmlStreamAttributes &attributes = stream.attributes();
					QString name = attributes.value(QStringLiteral("name")).toString();
					QString label = attributes.value(QStringLiteral("label")).toString();
					QString comment = attributes.value(QStringLiteral("comment")).toString();
					QColor color = QColor(attributes.value(QStringLiteral("color")).toString());
					bool enabled = (attributes.hasAttribute(QStringLiteral("enabled")) ? QVariant(attributes.value(QStringLiteral("enabled")).toString()).toBool() : true);
					int pauseBefore = attributes.value(QStringLiteral("pauseBefore")).toString().toInt();
					int pauseAfter = attributes.value(QStringLiteral("pauseAfter")).toString().toInt();
					int timeout = attributes.value(QStringLiteral("timeout")).toString().toInt();

					//Add a new action
					ActionInstance *actionInstance = mActionFactory->newActionInstance(name);
					if(!actionInstance)
						continue;

					ParametersData parametersData;
					ExceptionActionInstancesHash exceptionActionsHash;

					stream.readNext();

					for(;!stream.isEndElement() || stream.name() != QLatin1String("action");stream.readNext())
					{
						if(!stream.isStartElement())
							continue;

                        if(progressCallback)
                        {
                            progressCallback->operator()(mActionInstances.size(), actionCount - 1, tr("Reading actions..."));
                        }

						if(stream.name() == QLatin1String("exception"))
						{
							const QXmlStreamAttributes &attributes = stream.attributes();
							ActionException::Exception exceptionId = static_cast<ActionException::Exception>(attributes.value(QStringLiteral("id")).toString().toInt());
							ActionException::ExceptionActionInstance exceptionActionInstance(static_cast<ActionException::ExceptionAction>(attributes.value(QStringLiteral("action")).toString().toInt()),
																							 attributes.value(QStringLiteral("line")).toString());
							exceptionActionsHash.insert(exceptionId, exceptionActionInstance);
						}
						else if(stream.name() == QLatin1String("parameter"))
						{
							const QXmlStreamAttributes &attributes = stream.attributes();
							const QString &parameterName = attributes.value(QStringLiteral("name")).toString();

							Parameter parameterData;

							stream.readNext();

							for(;!stream.isEndElement() || stream.name() != QLatin1String("parameter");stream.readNext())
							{
								if(!stream.isStartElement())
									continue;

								const QXmlStreamAttributes &attributes = stream.attributes();
								QString subParameterName = attributes.value(QStringLiteral("name")).toString();
								SubParameter subParameterData;

								subParameterData.setCode(QVariant(stream.attributes().value(QStringLiteral("code")).toString()).toBool());
								subParameterData.setValue(stream.readElementText());

								parameterData.subParameters().insert(subParameterName, subParameterData);
							}

							parametersData.insert(parameterName, parameterData);
						}
					}

                    //Set default values, will be overwritten afterwards, but this is done to make sure we have valid parameters everywhere
                    for(ElementDefinition *element: actionInstance->definition()->elements())
                        element->setDefaultValues(actionInstance);

					actionInstance->setLabel(label);
					actionInstance->setComment(comment);

                    ParametersData defaultParametersData = actionInstance->parametersData();
                    for(const QString &parameterKey: parametersData.keys())
                        defaultParametersData[parameterKey] = parametersData.value(parameterKey);

                    actionInstance->setParametersData(defaultParametersData);
					actionInstance->setColor(color);
					actionInstance->setEnabled(enabled);
					actionInstance->setExceptionActionInstances(exceptionActionsHash);
					actionInstance->setPauseBefore(pauseBefore);
					actionInstance->setPauseAfter(pauseAfter);
					actionInstance->setTimeout(timeout);

					newActions.append(actionInstance);
				}
			}
		}

		if(addActionsCallback)
			(*addActionsCallback)(newActions);
		else
			mActionInstances.append(newActions);

        for(ActionDefinition *actionDefinition: updatableActionDefinitions.keys())
        {
            for(ActionInstance *actionInstance: mActionInstances)
            {
                if(actionInstance->definition() == actionDefinition)
                    actionDefinition->updateAction(actionInstance, updatableActionDefinitions.value(actionDefinition));
            }
        }

		return ReadSuccess;
	}

    bool Script::validateContent(const QString &content, const Tools::Version &scriptVersion)
	{
		QByteArray byteArray(content.toUtf8());
		QBuffer buffer(&byteArray);
		buffer.open(QIODevice::ReadOnly);

		mStatusMessage.clear();

		MessageHandler messageHandler;

		QFile schemaFile(QStringLiteral(":/script%1.xsd").arg(scriptVersion.toString()));
		if(!schemaFile.open(QIODevice::ReadOnly))
			return false;

		QXmlSchema schema;
		schema.setMessageHandler(&messageHandler);

		if(!schema.load(&schemaFile))
			return false;

		QXmlSchemaValidator validator(schema);
		if(!validator.validate(&buffer))
		{
			mStatusMessage = messageHandler.statusMessage();
			mLine = messageHandler.line();
			mColumn = messageHandler.column();

			return false;
		}

		return true;
	}

	int Script::actionIndexFromRuntimeId(qint64 runtimeId) const
	{
		for(int actionIndex = 0; actionIndex < actionCount(); ++actionIndex)
		{
			const ActionInstance *actionInstance = mActionInstances.at(actionIndex);

			if(actionInstance->runtimeId() == runtimeId)
				return actionIndex;
		}

		return -1;
	}

	QStringList Script::procedureNames() const
	{
		QStringList back;

        for(const ActionInstance *actionInstance: mActionInstances)
		{
			if(actionInstance->definition()->id() == QLatin1String("ActionBeginProcedure"))
			{
				const ActionTools::SubParameter &nameParameter = actionInstance->subParameter(QStringLiteral("name"), QStringLiteral("value"));
                const QString &procedureName = nameParameter.value();

				back.append(procedureName);
			}
		}

		return back;
	}

	QStringList Script::labels() const
	{
        if(!mRebuildLabelList)
            return mLabels;
        //TODO: cache this

        mLabels.clear();

        for(const ActionInstance *actionInstance: mActionInstances)
		{
			if(!actionInstance->label().isEmpty())
                mLabels << actionInstance->label();
		}

        mRebuildLabelList = false;

        return mLabels;
    }

    Script::ReadResult Script::validateSchema(QIODevice *device, const Tools::Version &scriptVersion, bool tryOlderVersions)
    {
        MessageHandler messageHandler;

		QFile schemaFile(QStringLiteral(":/script%1.xsd").arg(scriptVersion.toString()));
        if(!schemaFile.open(QIODevice::ReadOnly))
            return ReadInternal;

        QXmlSchema schema;
        schema.setMessageHandler(&messageHandler);

        {
#ifdef ACT_PROFILE
            Tools::HighResolutionTimer timer("loading schema file");
#endif
            if(!schema.load(&schemaFile))
                return ReadInternal;
        }

        {
#ifdef ACT_PROFILE
            Tools::HighResolutionTimer timer("validating file");
#endif
            QXmlSchemaValidator validator(schema);
            if(!validator.validate(device))
            {
                mStatusMessage = messageHandler.statusMessage();
                mLine = messageHandler.line();
                mColumn = messageHandler.column();

                if(!tryOlderVersions)
                    return ReadInvalidSchema;

                //If we could not validate, try to read the settings value to get the version
                device->reset();

                QXmlStreamReader stream(device);
                while(!stream.atEnd() && !stream.hasError())
                {
                    stream.readNext();

                    if(stream.isStartDocument())
                        continue;

                    if(!stream.isStartElement())
                        continue;

					if(stream.name() == QLatin1String("settings"))
                    {
                        const QXmlStreamAttributes &attributes = stream.attributes();
						mProgramName = attributes.value(QStringLiteral("program")).toString();
#if (QT_VERSION >= 0x050600)
						mProgramVersion = QVersionNumber::fromString(attributes.value(QStringLiteral("version")).toString());
						mScriptVersion = QVersionNumber::fromString(attributes.value(QStringLiteral("scriptVersion")).toString());
#else
						mProgramVersion = Tools::Version(attributes.value(QStringLiteral("version")).toString());
						mScriptVersion = Tools::Version(attributes.value(QStringLiteral("scriptVersion")).toString());
#endif
						mOs = attributes.value(QStringLiteral("os")).toString();

                        device->reset();

                        if(mScriptVersion == scriptVersion)
                            return ReadInvalidSchema;

                        if(validateSchema(device, mScriptVersion, false) != ReadSuccess)
                            return ReadInvalidSchema;

                        return ReadSuccess;
                    }
                }

                return ReadInvalidSchema;
            }
        }

        return ReadSuccess;
    }

    QSet<QString> Script::findVariables(ActionInstance *actionInstance, ActionInstance *excludedActionInstance) const
    {
        QSet<QString> back;

        if(actionInstance)
        {
            if(actionInstance != excludedActionInstance)
                findVariablesInAction(actionInstance, back);
        }
        else
        {
            for(const ScriptParameter &scriptParameter: mParameters)
            {
                if(!scriptParameter.name().isEmpty())
                    back << scriptParameter.name();
            }

            for(ActionInstance *currentActionInstance: mActionInstances)
            {
                if(currentActionInstance != excludedActionInstance)
                    findVariablesInAction(currentActionInstance, back);
            }
        }

        return back;
    }

    void Script::executionStopped()
    {
        for(auto actionInstance: mActionInstances)
            actionInstance->stopLongTermExecution();

        mMinMaxExecutionCounter = {std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};
        mExecutionDuration = 0;

        for(auto actionInstance: mActionInstances)
        {
            auto counter = actionInstance->executionCounter();

            if(counter < mMinMaxExecutionCounter.first)
                mMinMaxExecutionCounter.first = counter;
            if(counter > mMinMaxExecutionCounter.second)
                mMinMaxExecutionCounter.second = counter;

            mExecutionDuration += actionInstance->executionDuration();
        }
    }

    void Script::updateLineModel()
    {
        mLineModel->update();
    }

    void Script::parametersFromDefinition(QSet<QString> &variables, const ActionInstance *actionInstance, const ElementDefinition *elementDefinition) const
    {
        const Parameter &parameter = actionInstance->parameter(elementDefinition->name().original());
        const SubParameterHash &subParameters = parameter.subParameters();
		QRegExp newLineRegExp(QStringLiteral("[\n\r;]"));

        SubParameterHash::ConstIterator it = subParameters.constBegin();
        for(;it != subParameters.constEnd();++it)
        {
            const SubParameter &subParameter = it.value();

            if(subParameter.isCode())
            {
                //Add every variable in any parameter type that is in code mode
                const QString &code = subParameter.value();

                for(const QString &codeLine: code.split(newLineRegExp, QString::SkipEmptyParts))
                {
                    int position = 0;

                    while((position = CodeVariableDeclarationRegExp.indexIn(codeLine, position)) != -1)
                    {
                        QString foundVariableName = CodeVariableDeclarationRegExp.cap(1);

                        position += CodeVariableDeclarationRegExp.cap(1).length();

                        if(!foundVariableName.isEmpty())
                            variables << foundVariableName;
                    }
                }
            }
            else
            {
                //Add every variable in a variable parameter that is not in code mode
                if(qobject_cast<const VariableParameterDefinition *>(elementDefinition))
                {
                    const QString &foundVariableName = subParameter.value();

                    if(!foundVariableName.isEmpty())
                        variables << foundVariableName;

                    continue;
                }

                //Add every variable in any parameter type that is not in code mode
                const QString &text = subParameter.value();

                int position = 0;

                while((position = ActionInstance::VariableRegExp.indexIn(text, position)) != -1)
                {
                    QString foundVariableName = ActionInstance::VariableRegExp.cap(1);

                    position += ActionInstance::VariableRegExp.cap(0).length();

                    if(!foundVariableName.isEmpty())
                        variables << foundVariableName;
                }
            }
        }
    }

    void Script::findVariablesInAction(ActionInstance *actionInstance, QSet<QString> &result) const
    {
        const ActionDefinition *actionDefinition = actionInstance->definition();

        for(const ElementDefinition *elementDefinition: actionDefinition->elements())
        {
            if(const auto groupDefinition = qobject_cast<const GroupDefinition *>(elementDefinition))
            {
                for(const ParameterDefinition *parameterDefinition: groupDefinition->members())
                    parametersFromDefinition(result, actionInstance, parameterDefinition);
            }
            else
                parametersFromDefinition(result, actionInstance, elementDefinition);
        }
    }
}
