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

#ifndef ACTIONINSTANCE_H
#define ACTIONINSTANCE_H

#include "actiontools_global.h"
#include "parameter.h"
#include "actionexception.h"

#include <QObject>
#include <QSharedData>
#include <QVariant>
#include <QDataStream>
#include <QHash>
#include <QColor>
#include <QScriptValue>
#include <QScriptContext>
#include <QScriptEngine>
#include <QMetaEnum>

class QScriptEngine;

namespace ActionTools
{
	class ActionDefinition;
	class ActionFactory;
	class ParameterDefinition;
	class Script;

	typedef QHash<QString, Parameter> ParametersData;
	typedef QHash<ActionException::Exception, ActionException::ExceptionActionInstance> ExceptionActionInstancesHash;

	class ActionInstanceData : public QSharedData
	{
	public:
		ActionInstanceData() : definition(0), enabled(true), selected(false), pauseBefore(0), pauseAfter(0), timeout(0), script(0), scriptEngine(0)		{}
		ActionInstanceData(const ActionInstanceData &other)
			: QSharedData(other),
			parametersData(other.parametersData),
			definition(other.definition),
			comment(other.comment),
			label(other.label),
			color(other.color),
			enabled(other.enabled),
			selected(other.selected),
			exceptionActionInstances(other.exceptionActionInstances),
			pauseBefore(other.pauseBefore),
			pauseAfter(other.pauseAfter),
			timeout(other.timeout),
			script(other.script),
			scriptEngine(other.scriptEngine)																											{}

		ParametersData parametersData;
		const ActionDefinition *definition;
		QString comment;
		QString label;
		QColor color;
		bool enabled;
		bool selected;
		ExceptionActionInstancesHash exceptionActionInstances;
		int pauseBefore;
		int pauseAfter;
		int timeout;
		Script *script;
		QScriptEngine *scriptEngine;
	};

	class ACTIONTOOLSSHARED_EXPORT ActionInstance : public QObject
	{
		Q_OBJECT

	public:
		ActionInstance(const ActionDefinition *definition = 0, QObject *parent = 0);
		ActionInstance(const ActionInstance &other) : QObject(), d(other.d)	{}
		virtual ~ActionInstance()											{}

		const ActionDefinition *definition() const							{ return d->definition; }

		QString comment() const												{ return d->comment; }
		QString label() const												{ return d->label; }
		ParametersData parametersData() const								{ return d->parametersData; }
		QColor color() const												{ return d->color; }
		bool isEnabled() const												{ return d->enabled; }
		bool isSelected() const												{ return d->selected; }
		ExceptionActionInstancesHash exceptionActionInstances() const		{ return d->exceptionActionInstances; }
		ActionException::ExceptionActionInstance exceptionActionInstance(ActionException::Exception exception) const
																			{ return d->exceptionActionInstances.value(exception); }
		int pauseBefore() const												{ return d->pauseBefore; }
		int pauseAfter() const												{ return d->pauseAfter; }
		int timeout() const													{ return d->timeout; }

		void setComment(const QString &comment)								{ d->comment = comment; }
		void setLabel(const QString &label)									{ d->label = label; }
		void setParametersData(const ParametersData &parametersData)		{ d->parametersData = parametersData; }
		void setColor(const QColor &color)									{ d->color = color; }
		void setEnabled(bool enabled)										{ d->enabled = enabled; }
		void setSelected(bool selected)										{ d->selected = selected; }
		void setExceptionActionInstances(const ExceptionActionInstancesHash &exceptionActions)
																			{ d->exceptionActionInstances = exceptionActions; }
		void setExceptionActionInstance(ActionException::Exception exception, ActionException::ExceptionActionInstance exceptionActionInstance)
																			{ d->exceptionActionInstances.insert(exception, exceptionActionInstance); }
		void setPauseBefore(int pauseBefore)								{ d->pauseBefore = pauseBefore; }
		void setPauseAfter(int pauseAfter)									{ d->pauseAfter = pauseAfter; }
		void setTimeout(int timeout)										{ d->timeout = timeout; }
		
		void setParameter(const QString &name, const Parameter &parameter)	{ d->parametersData.insert(name, parameter); }
		void setSubParameter(const QString &parameterName, const QString &subParameterName, const SubParameter &subParameter)
																			{ d->parametersData[parameterName].setSubParameter(subParameterName, subParameter); }
		void setSubParameter(const QString &parameterName, const QString &subParameterName, bool code, const QVariant &value)
																			{ setSubParameter(parameterName, subParameterName, SubParameter(code, value)); }
		void setSubParameter(const QString &parameterName, const QString &subParameterName, const QVariant &value)
																			{ setSubParameter(parameterName, subParameterName, SubParameter(false, value)); }
		Parameter parameter(const QString &name) const						{ return d->parametersData.value(name); }
		SubParameter subParameter(const QString &parameterName, const QString &subParameterName) const
																			{ return parameter(parameterName).subParameter(subParameterName); }
		virtual void reset()												{}
		virtual void scriptExecutionStopped()										{}

		void setupExecution(QScriptEngine *scriptEngine, Script *script)	{ d->scriptEngine = scriptEngine; d->script = script; }

		void copyActionDataFrom(const ActionInstance &other)
		{
			setComment(other.comment());
			setLabel(other.label());
			setParametersData(other.parametersData());
			setColor(other.color());
			setEnabled(other.isEnabled());
			setSelected(other.isSelected());
			setExceptionActionInstances(other.exceptionActionInstances());
			setPauseBefore(other.pauseBefore());
			setPauseAfter(other.pauseAfter());
			setTimeout(other.timeout());
		}

	protected:
		QScriptEngine *scriptEngine() const									{ return d->scriptEngine; }
		Script *script() const												{ return d->script; }

	public slots:
		virtual void startExecution()										{}
		virtual void stopExecution()										{}

	signals:
		void executionException(int exception, const QString &message);
		void executionEnded();
		void disableAction(bool disable = true);

	private:
		QSharedDataPointer<ActionInstanceData> d;
	};

	ACTIONTOOLSSHARED_EXPORT QDataStream &operator << (QDataStream &s, const ActionInstance &actionInstance);
	ACTIONTOOLSSHARED_EXPORT QDataStream &operator >> (QDataStream &s, ActionInstance &actionInstance);
	ACTIONTOOLSSHARED_EXPORT QDebug &operator << (QDebug &dbg, const ActionInstance &actionInstance);
	ACTIONTOOLSSHARED_EXPORT QDebug &operator << (QDebug &dbg, const ParametersData &parametersData);
	ACTIONTOOLSSHARED_EXPORT QDebug &operator << (QDebug &dbg, const ExceptionActionInstancesHash &exceptionActionInstancesHash);
}

Q_DECLARE_METATYPE(ActionTools::ActionInstance)

#endif // ACTIONINSTANCE_H
