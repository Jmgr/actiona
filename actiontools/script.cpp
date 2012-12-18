/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#include "script.h"
#include "actioninstance.h"
#include "actiondefinition.h"
#include "actionfactory.h"
#include "parameter.h"
#include "subparameter.h"
#include "messagehandler.h"

#include <QIODevice>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QBuffer>

namespace ActionTools
{
	Script::Script(ActionFactory *actionFactory, QObject *parent)
		: QObject(parent),
		mActionFactory(actionFactory),
		mLine(-1),
		mColumn(-1),
		mPauseBefore(0),
		mPauseAfter(0)
	{
	}

	Script::~Script()
	{
		qDeleteAll(mActionInstances);
	}

	ActionInstance *Script::appendAction(const QString &actionDefinitionId)
	{
		ActionInstance *actionInstance = mActionFactory->newActionInstance(actionDefinitionId);
		if(!actionInstance)
			return 0;

		appendAction(actionInstance);

		return actionInstance;
	}

	ActionInstance *Script::actionAt(int line) const
	{
		if(line < 0 || line >= mActionInstances.size())
			return 0;

		return mActionInstances.at(line);
	}

	void Script::insertAction(int line, ActionInstance *actionInstance)
	{
		mActionInstances.insert(line, actionInstance);
	}

	void Script::setAction(int line, ActionInstance *actionInstance)
	{
		if(line < 0 || line >= mActionInstances.size())
			return;

		delete mActionInstances.at(line);

		mActionInstances[line] = actionInstance;
	}

	void Script::removeActions(int line, int count)
	{
		if(count <= 0 || line < 0)
			return;

		for(int r = line; r <= line + count - 1; ++r)
			removeAction(r);
	}

	void Script::removeAction(int line)
	{
		if(line < 0 || line >= mActionInstances.size())
			return;

		delete mActionInstances.takeAt(line);
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
		foreach(ActionInstance *actionInstance, mActionInstances)
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
		foreach(ActionInstance *actionInstance, mActionInstances)
		{
			result << actionInstance->definition()->index();

			const ParametersData &parametersData = actionInstance->parametersData();
			foreach(const Parameter &parameter, parametersData)
			{
				foreach(const SubParameter &subParameter, parameter.subParameters())
				{
					if(subParameter.isCode())
					{
						const QString &value = subParameter.value().toString();

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

	bool Script::write(QIODevice *device, const Tools::Version &programVersion, const Tools::Version &scriptVersion)
	{
#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("Script::write");
#endif
		QXmlStreamWriter stream(device);
		stream.setAutoFormatting(true);

		stream.writeStartDocument();
		stream.writeStartElement("scriptfile");

		QString osName = tr("Unknown");
	#ifdef Q_WS_X11
		osName = tr("GNU/Linux");
	#endif
	#ifdef Q_WS_WIN
		osName = tr("Windows");
	#endif
	#ifdef Q_WS_MAC
		osName = tr("Mac");
	#endif

		stream.writeStartElement("settings");

		stream.writeAttribute("program", "actionaz");
		stream.writeAttribute("version", programVersion.toString());
		stream.writeAttribute("scriptVersion", scriptVersion.toString());
		stream.writeAttribute("os", osName);

		stream.writeEndElement();

		stream.writeStartElement("actions");

		foreach(int actionIndex, usedActions())
		{
			ActionDefinition *actionDefinition = mActionFactory->actionDefinition(actionIndex);

			stream.writeStartElement("action");
			stream.writeAttribute("name", actionDefinition->id());
			stream.writeAttribute("version", actionDefinition->version().toString());
			stream.writeEndElement();
		}

		stream.writeEndElement();

		stream.writeStartElement("parameters");

		foreach(const ScriptParameter &parameter, mParameters)
		{
			stream.writeStartElement("parameter");
			stream.writeAttribute("name", parameter.name());
			stream.writeAttribute("code", QString("%1").arg(parameter.isCode()));
			stream.writeAttribute("type", QString::number(parameter.type()));
			stream.writeCharacters(parameter.value());
			stream.writeEndElement();
		}

		stream.writeEndElement();

        stream.writeStartElement("resources");

        QHash<QString, Resource>::const_iterator resourceIt = mResources.constBegin();
        while(resourceIt != mResources.constEnd())
        {
            stream.writeStartElement("resource");
            stream.writeAttribute("id", resourceIt.key());
            stream.writeAttribute("type", QString::number(resourceIt.value().type()));
            stream.writeCharacters(resourceIt.value().data().toBase64());
            stream.writeEndElement();

            ++resourceIt;
        }

        stream.writeEndElement();

		stream.writeStartElement("script");
		stream.writeAttribute("pauseBefore", QString::number(pauseBefore()));
		stream.writeAttribute("pauseAfter", QString::number(pauseAfter()));

		foreach(ActionInstance *actionInstance, mActionInstances)
		{
			stream.writeStartElement("action");
			stream.writeAttribute("name", actionInstance->definition()->id());

			if(!actionInstance->label().isEmpty())
				stream.writeAttribute("label", actionInstance->label());
			if(!actionInstance->comment().isEmpty())
				stream.writeAttribute("comment", actionInstance->comment());
			if(actionInstance->color().isValid() && actionInstance->color() != Qt::transparent)
				stream.writeAttribute("color", actionInstance->color().name());
			if(!actionInstance->isEnabled())
				stream.writeAttribute("enabled", QVariant(actionInstance->isEnabled()).toString());
			if(actionInstance->pauseBefore() != 0)
				stream.writeAttribute("pauseBefore", QVariant(actionInstance->pauseBefore()).toString());
			if(actionInstance->pauseAfter() != 0)
				stream.writeAttribute("pauseAfter", QVariant(actionInstance->pauseAfter()).toString());
			if(actionInstance->timeout() != 0)
				stream.writeAttribute("timeout", QVariant(actionInstance->timeout()).toString());

			const ExceptionActionInstancesHash &exceptionActionsHash = actionInstance->exceptionActionInstances();
			foreach(ActionException::Exception exception, exceptionActionsHash.keys())
			{
				ActionException::ExceptionActionInstance exceptionActionInstance = exceptionActionsHash.value(exception);
				stream.writeStartElement("exception");
				stream.writeAttribute("id", QString::number(static_cast<int>(exception)));
				stream.writeAttribute("action", QString::number(static_cast<int>(exceptionActionInstance.action())));
				stream.writeAttribute("line", exceptionActionInstance.line());
				stream.writeEndElement();
			}

			const ParametersData &parametersData = actionInstance->parametersData();
			foreach(const QString &parameter, parametersData.keys())
			{
				const Parameter &parameterData = parametersData.value(parameter);

				stream.writeStartElement("parameter");
				stream.writeAttribute("name", parameter);

				foreach(const QString &subParameter, parameterData.subParameters().keys())
				{
					const SubParameter &subParameterData = parameterData.subParameters().value(subParameter);

					stream.writeStartElement("subParameter");
					stream.writeAttribute("name", subParameter);

					stream.writeAttribute("code", QString("%1").arg(subParameterData.isCode()));
					stream.writeCharacters(subParameterData.value().toString());

					stream.writeEndElement();
				}

				stream.writeEndElement();
			}

			stream.writeEndElement();
		}

		stream.writeEndElement();

		stream.writeEndElement();
		stream.writeEndDocument();

		return true;
	}

    Script::ReadResult Script::read(QIODevice *device, const Tools::Version &scriptVersion)
	{
#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("Script::read");
#endif
		mMissingActions.clear();

        ReadResult result = validateSchema(device, scriptVersion);
        if(result != ReadSuccess)
            return result;

		qDeleteAll(mActionInstances);
		mActionInstances.clear();
		mParameters.clear();
        mResources.clear();

		device->reset();

#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer2("Reading content");
#endif

		QXmlStreamReader stream(device);
		while(!stream.atEnd() && !stream.hasError())
		{
			stream.readNext();

			if(stream.isStartDocument())
				continue;

			if(!stream.isStartElement())
				continue;

			if(stream.name() == "settings")
			{
				const QXmlStreamAttributes &attributes = stream.attributes();
				mProgramName = attributes.value("program").toString();
				mProgramVersion = Tools::Version(attributes.value("version").toString());
				mScriptVersion = Tools::Version(attributes.value("scriptVersion").toString());
				mOs = attributes.value("os").toString();

				if(mScriptVersion > scriptVersion)
					return ReadBadScriptVersion;
			}
			else if(stream.name() == "actions")
			{
				stream.readNext();

				for(;!stream.isEndElement() || stream.name() != "actions";stream.readNext())
				{
					if(!stream.isStartElement())
						continue;

					const QXmlStreamAttributes &attributes = stream.attributes();
					QString name = attributes.value("name").toString();
					//TODO : Do something with the action version

					ActionDefinition *actionDefinition = mActionFactory->actionDefinition(name);
					if(!actionDefinition)
						mMissingActions << name;
				}
			}
			else if(stream.name() == "parameters")
			{
				stream.readNext();

				for(;!stream.isEndElement() || stream.name() != "parameters";stream.readNext())
				{
					if(!stream.isStartElement())
						continue;

					const QXmlStreamAttributes &attributes = stream.attributes();
					ScriptParameter scriptParameter(	attributes.value("name").toString(),
														stream.readElementText(),
														QVariant(attributes.value("code").toString()).toBool(),
														static_cast<ScriptParameter::ParameterType>(attributes.value("type").toString().toInt()));

					mParameters.append(scriptParameter);
				}
			}
            else if(stream.name() == "resources")
            {
                stream.readNext();

                for(;!stream.isEndElement() || stream.name() != "resources";stream.readNext())
                {
                    if(!stream.isStartElement())
                        continue;

                    const QXmlStreamAttributes &attributes = stream.attributes();
                    QString id = attributes.value("id").toString();
                    QString base64Data = stream.readElementText();
                    QByteArray data = QByteArray::fromBase64(base64Data.toAscii());
                    Resource resource(data, static_cast<Resource::Type>(attributes.value("type").toString().toInt()));

                    mResources.insert(id, resource);
                }
            }
			else if(stream.name() == "script")
			{
				const QXmlStreamAttributes &attributes = stream.attributes();
				setPauseBefore(attributes.value("pauseBefore").toString().toInt());
				setPauseAfter(attributes.value("pauseAfter").toString().toInt());

				stream.readNext();

				for(;!stream.isEndElement() || stream.name() != "script";stream.readNext())
				{
					if(!stream.isStartElement())
						continue;

					const QXmlStreamAttributes &attributes = stream.attributes();
					QString name = attributes.value("name").toString();
					QString label = attributes.value("label").toString();
					QString comment = attributes.value("comment").toString();
					QColor color = QColor(attributes.value("color").toString());
					bool enabled = (attributes.hasAttribute("enabled") ? QVariant(attributes.value("enabled").toString()).toBool() : true);
					int pauseBefore = attributes.value("pauseBefore").toString().toInt();
					int pauseAfter = attributes.value("pauseAfter").toString().toInt();
					int timeout = attributes.value("timeout").toString().toInt();

					//Add a new action
					ActionInstance *actionInstance = mActionFactory->newActionInstance(name);
					if(!actionInstance)
						continue;

					ParametersData parametersData;
					ExceptionActionInstancesHash exceptionActionsHash;

					stream.readNext();

					for(;!stream.isEndElement() || stream.name() != "action";stream.readNext())
					{
						if(!stream.isStartElement())
							continue;

						if(stream.name() == "exception")
						{
							const QXmlStreamAttributes &attributes = stream.attributes();
							ActionException::Exception exceptionId = static_cast<ActionException::Exception>(attributes.value("id").toString().toInt());
							ActionException::ExceptionActionInstance exceptionActionInstance(static_cast<ActionException::ExceptionAction>(attributes.value("action").toString().toInt()),
																							 attributes.value("line").toString());
							exceptionActionsHash.insert(exceptionId, exceptionActionInstance);
						}
						else if(stream.name() == "parameter")
						{
							const QXmlStreamAttributes &attributes = stream.attributes();
							const QString &parameterName = attributes.value("name").toString();

							Parameter parameterData;

							stream.readNext();

							for(;!stream.isEndElement() || stream.name() != "parameter";stream.readNext())
							{
								if(!stream.isStartElement())
									continue;

								const QXmlStreamAttributes &attributes = stream.attributes();
								QString subParameterName = attributes.value("name").toString();
								SubParameter subParameterData;

								subParameterData.setCode(QVariant(stream.attributes().value("code").toString()).toBool());
								subParameterData.setValue(stream.readElementText());

								parameterData.subParameters().insert(subParameterName, subParameterData);
							}

							parametersData.insert(parameterName, parameterData);
						}
					}

					actionInstance->setLabel(label);
					actionInstance->setComment(comment);
					actionInstance->setParametersData(parametersData);
					actionInstance->setColor(color);
					actionInstance->setEnabled(enabled);
					actionInstance->setExceptionActionInstances(exceptionActionsHash);
					actionInstance->setPauseBefore(pauseBefore);
					actionInstance->setPauseAfter(pauseAfter);
					actionInstance->setTimeout(timeout);

					appendAction(actionInstance);
				}
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

        QFile schemaFile(QString(":/script%1.xsd").arg(scriptVersion.toString()));
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

		foreach(const ActionInstance *actionInstance, mActionInstances)
		{
			if(actionInstance->definition()->id() == "ActionBeginProcedure")
			{
				const ActionTools::SubParameter &nameParameter = actionInstance->subParameter("name", "value");
				const QString &procedureName = nameParameter.value().toString();

				back.append(procedureName);
			}
		}

		return back;
	}

	QStringList Script::labels() const
	{
		QStringList back;

		foreach(const ActionInstance *actionInstance, mActionInstances)
		{
			if(!actionInstance->label().isEmpty())
				back << actionInstance->label();
		}

        return back;
    }

    Script::ReadResult Script::validateSchema(QIODevice *device, const Tools::Version &scriptVersion, bool tryOlderVersions)
    {
        MessageHandler messageHandler;

        QFile schemaFile(QString(":/script%1.xsd").arg(scriptVersion.toString()));
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
                    return ReadBadSchema;

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

                    if(stream.name() == "settings")
                    {
                        const QXmlStreamAttributes &attributes = stream.attributes();
                        mProgramName = attributes.value("program").toString();
                        mProgramVersion = Tools::Version(attributes.value("version").toString());
                        mScriptVersion = Tools::Version(attributes.value("scriptVersion").toString());
                        mOs = attributes.value("os").toString();

                        device->reset();

                        if(mScriptVersion == scriptVersion)
                            return ReadBadSchema;

                        if(validateSchema(device, mScriptVersion, false) != ReadSuccess)
                            return ReadBadSchema;

                        return ReadSuccess;
                    }
                }

                return ReadBadSchema;
            }
        }

        return ReadSuccess;
    }
}
