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

#include "script.h"
#include "action.h"
#include "actionfactory.h"
#include "parameter.h"
#include "subparameter.h"

#include <QIODevice>
#include <QXmlStreamReader>

namespace ActionTools
{
	Script::Script(ActionFactory *actionFactory, QObject *parent)
		: QObject(parent),
		mActionFactory(actionFactory)
	{
	}

	Script::~Script()
	{
		qDeleteAll(mActions);
	}

	Action *Script::appendAction(const QString &actionId)
	{
		Action *action = mActionFactory->newAction(actionId);
		if(!action)
			return 0;

		appendAction(action);

		return action;
	}

	Action *Script::actionAt(int line) const
	{
		if(line < 0 || line >= mActions.size())
			return 0;

		return mActions.at(line);
	}

	void Script::insertAction(int line, Action *action)
	{
		mActions.insert(line, action);
	}

	void Script::setAction(int line, Action *action)
	{
		if(line < 0 || line >= mActions.size())
			return;

		delete mActions.at(line);

		mActions[line] = action;
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
		if(line < 0 || line >= mActions.size())
			return;

		delete mActions.takeAt(line);
	}

	void Script::removeAction(Action *action)
	{
		int index = mActions.indexOf(action);

		if(index == -1)
			return;

		removeAction(index);
	}

	void Script::removeAll()
	{
		qDeleteAll(mActions);
		mActions.clear();
	}

	void Script::moveAction(int startLine, int endLine)
	{
		if(startLine < 0 || startLine >= mActions.count() ||
		   endLine < 0 || startLine == endLine)
		return;

		if(endLine >= mActions.count())
			mActions.append(mActions.takeAt(startLine));
		else
			mActions.move(startLine, endLine);
	}

	int Script::labelLine(const QString &label) const
	{
		for(int i = 0; i < mActions.count(); ++i)
		{
			if(mActions.at(i)->label() == label)
				return i;
		}

		return -1;
	}

	bool Script::isActionUsed(const QString &actionId) const
	{
		foreach(Action *action, mActions)
		{
			if(action->interface()->id() == actionId)
				return true;
		}

		return false;
	}

	bool Script::hasEnabledActions() const
	{
		foreach(Action *action, mActions)
		{
			if(action->isEnabled())
				return true;
		}

		return false;
	}

	bool Script::isActionUsed(int actionIndex) const
	{
		if(actionIndex < 0 || actionIndex >= mActionFactory->actionCount())
			return false;

		foreach(Action *action, mActions)
		{
			if(action->interface() == mActionFactory->actionInterface(actionIndex))
				return true;
		}

		return false;
	}

	bool Script::write(QIODevice *device, const Tools::Version &programVersion, const Tools::Version &scriptVersion)
	{
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

		stream.writeStartElement("options");

		foreach(const QString &key, mOptions.keys())
		{
			stream.writeTextElement(key, mOptions.value(key).toString());
		}

		stream.writeEndElement();

		stream.writeEndElement();

		stream.writeStartElement("actions");

		int actionInterfaceCount = mActionFactory->actionCount();
		for(int actionIndex = 0; actionIndex < actionInterfaceCount; ++actionIndex)
		{
			ActionInterface *actionInterface = mActionFactory->actionInterface(actionIndex);

			stream.writeStartElement(actionInterface->id());
			stream.writeAttribute("version", actionInterface->version().toString());
			stream.writeEndElement();
		}

		stream.writeEndElement();

		stream.writeStartElement("parameters");

		foreach(const ScriptParameter &parameter, mParameters)
		{
			stream.writeStartElement(parameter.name());
			stream.writeAttribute("code", QString("%1").arg(parameter.isCode()));
			stream.writeCharacters(parameter.value());
			stream.writeEndElement();
		}

		stream.writeEndElement();

		stream.writeStartElement("script");

		foreach(Action *action, mActions)
		{
			stream.writeStartElement(action->interface()->id());

			if(!action->label().isEmpty())
				stream.writeAttribute("label", action->label());
			if(!action->comment().isEmpty())
				stream.writeAttribute("comment", action->comment());
			if(action->color().isValid() && action->color() != Qt::transparent)
				stream.writeAttribute("color", action->color().name());
			if(!action->isEnabled())
				stream.writeAttribute("enabled", QVariant(action->isEnabled()).toString());

			const ParametersData &parametersData = action->parametersData();
			foreach(const QString &parameter, parametersData.keys())
			{
				const Parameter &parameterData = parametersData.value(parameter);

				stream.writeStartElement(parameter);

				foreach(const QString &subParameter, parameterData.subParameters().keys())
				{
					const SubParameter &subParameterData = parameterData.subParameters().value(subParameter);

					stream.writeStartElement(subParameter);

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

	bool Script::read(QIODevice *device)
	{
		qDeleteAll(mActions);
		mActions.clear();
		mParameters.clear();

		QXmlStreamReader stream(device);
		QXmlStreamAttributes attributes;

		while(!stream.atEnd())
		{
			stream.readNext();

			if(stream.isStartElement())
			{
				if(stream.name() == "settings")
				{
					attributes = stream.attributes();
					QString programName = attributes.value("program").toString();
					Tools::Version version = Tools::Version(attributes.value("version").toString());
					Tools::Version scriptVersion = Tools::Version(attributes.value("scriptVersion").toString());
					QString osName = attributes.value("os").toString();
					//TODO : Check versions here
					//if(!MainLogic::instance()->checkVersions(version, executerVersion, scriptVersion))
					//stream.raiseError();

					while(!stream.atEnd())
					{
						stream.readNext();

						if(stream.isEndElement() && stream.name() == "options")
							break;

						if(stream.isStartElement())
							mOptions.insert(stream.name().toString(), stream.text().toString());
					}
				}
				else if(stream.name() == "actions")
				{
					while(!stream.atEnd())
					{
						stream.readNext();

						if(stream.isEndElement() && stream.name() == "actions")
							break;

						if(stream.isStartElement())
						{
							attributes = stream.attributes();
							QString name = stream.name().toString();
							QString version = attributes.value("version").toString();
							//TODO : Check if we have that action here
							//if(!MainLogic::instance()->checkAction(name, version))
							//	stream.raiseError();
						}
					}
				}
				else if(stream.name() == "parameters")
				{
					while(!stream.atEnd())
					{
						stream.readNext();

						if(stream.isEndElement() && stream.name() == "parameters")
							break;

						if(stream.isStartElement())
						{
							attributes = stream.attributes();
							ScriptParameter scriptParameter(	stream.name().toString(),
																stream.readElementText(),
																QVariant(attributes.value("code").toString()).toBool());

							mParameters.append(scriptParameter);
						}
					}
				}
				else if(stream.name() == "script")
				{
					while(!stream.atEnd())
					{
						stream.readNext();

						if(stream.isEndElement() && stream.name() == "script")
							break;

						if(stream.isStartElement())
						{
							attributes = stream.attributes();
							QString name = stream.name().toString();
							QString label = attributes.value("label").toString();
							QString comment = attributes.value("comment").toString();
							ParametersData parametersData;
							QColor color = QColor(attributes.value("color").toString());
							bool enabled = (attributes.hasAttribute("enabled") ? QVariant(attributes.value("enabled").toString()).toBool() : true);

							//Add a new action
							Action *newAction = mActionFactory->newAction(name);
							if(!newAction)
							{
								//TODO
								//QMessageBox::warning(this, tr("Open script"), tr("Unable to create an action of type %1").arg(name));
								continue;
							}

							while(!stream.atEnd())
							{
								stream.readNext();

								if(stream.isEndElement() && stream.name() == name)
									break;

								if(stream.isStartElement())
								{
									const QStringRef &parameterName = stream.name();
									Parameter parameterData;

									while(!stream.atEnd())
									{
										stream.readNext();

										if(stream.isEndElement() && stream.name() == parameterName)
											break;

										if(stream.isStartElement())
										{
											QString subParameterName = stream.name().toString();
											SubParameter subParameterData;

											subParameterData.setCode(QVariant(stream.attributes().value("code").toString()).toBool());
											subParameterData.setValue(stream.readElementText());

											parameterData.subParameters().insert(subParameterName, subParameterData);
										}
									}

									parametersData.insert(parameterName.toString(), parameterData);
								}
							}

							newAction->setLabel(label);
							newAction->setComment(comment);
							newAction->setParametersData(parametersData);
							newAction->setColor(color);
							newAction->setEnabled(enabled);

							appendAction(newAction);
						}
					}
				}
			}
		}

		return true;
	}

	QStringList Script::labels() const
	{
		QStringList back;

		foreach(const Action *action, mActions)
		{
			if(!action->label().isEmpty())
				back << action->label();
		}

		return back;
	}
}
