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

#ifndef ACTION_H
#define ACTION_H

#include "actiontools_global.h"
#include "parameter.h"

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
	class ActionInterface;
	class ActionFactory;
	class ParameterDefinition;
	class Script;

	typedef QHash<QString, Parameter> ParametersData;

	class ActionData : public QSharedData
	{
	public:
		ActionData() : interface(0), enabled(true), selected(false)			{}
		ActionData(const ActionData &other)
			: QSharedData(other),
			parametersData(other.parametersData),
			interface(other.interface),
			comment(other.comment),
			label(other.label),
			color(other.color),
			enabled(other.enabled),
			selected(other.selected)										{}

		ParametersData parametersData;
		ActionInterface *interface;
		QString comment;
		QString label;
		QColor color;
		bool enabled;
		bool selected;
	};

	class ACTIONTOOLSSHARED_EXPORT Action : public QObject
	{
		Q_OBJECT

	public:
		enum ExecutionException
		{
			Warning,
			Error
		};

		Action(ActionInterface *interface = 0, QObject *parent = 0);
		Action(const Action &other) : QObject(), d(other.d)					{}
		virtual ~Action()													{}

		ActionInterface *interface() const									{ return d->interface; }

		QString comment() const												{ return d->comment; }
		QString label() const												{ return d->label; }
		ParametersData parametersData() const								{ return d->parametersData; }
		QColor color() const												{ return d->color; }
		bool isEnabled() const												{ return d->enabled; }
		bool isSelected() const												{ return d->selected; }

		void setComment(const QString &comment)								{ d->comment = comment; }
		void setLabel(const QString &label)									{ d->label = label; }
		void setParametersData(const ParametersData &parametersData)		{ d->parametersData = parametersData; }
		void setColor(const QColor &color)									{ d->color = color; }
		void setEnabled(bool enabled)										{ d->enabled = enabled; }
		void setSelected(bool selected)										{ d->selected = selected; }
		void setParameter(const QString &name, const Parameter &parameter)	{ d->parametersData.insert(name, parameter); }
		void setSubParameter(const QString &parameterName, const QString &subParameterName, const SubParameter &subParameter)
																			{ d->parametersData[parameterName].setSubParameter(subParameterName, subParameter); }
		void setSubParameter(const QString &parameterName, const QString &subParameterName, bool code, const QVariant &value)
																			{ setSubParameter(parameterName, subParameterName, SubParameter(code, value)); }
		Parameter parameter(const QString &name) const						{ return d->parametersData.value(name); }
		SubParameter subParameter(const QString &parameterName, const QString &subParameterName) const
																			{ return parameter(parameterName).subParameter(subParameterName); }
		virtual void reset()												{}

		void copyActionDataFrom(const Action &other)
		{
			setComment(other.comment());
			setLabel(other.label());
			setParametersData(other.parametersData());
			setColor(other.color());
			setEnabled(other.isEnabled());
			setSelected(other.isSelected());
		}

	public slots:
		virtual void startExecution(Script *script, QScriptEngine *scriptEngine)	{ Q_UNUSED(script); Q_UNUSED(scriptEngine); }
		virtual void stopExecution()										{}

	signals:
		void executionException(ActionTools::Action::ExecutionException exceptionType,
								const QString &message);
		void executionEnded();

	private:
		void setParameterDefaultValue(ParameterDefinition *parameter);

		QSharedDataPointer<ActionData> d;
	};

	ACTIONTOOLSSHARED_EXPORT QDataStream &operator << (QDataStream &s, const Action &action);
	ACTIONTOOLSSHARED_EXPORT QDataStream &operator >> (QDataStream &s, Action &action);
	ACTIONTOOLSSHARED_EXPORT QDebug &operator << (QDebug &dbg, const Action &action);
	ACTIONTOOLSSHARED_EXPORT QDebug &operator << (QDebug &dbg, const ParametersData &parametersData);
}

Q_DECLARE_METATYPE(ActionTools::Action)
Q_DECLARE_METATYPE(ActionTools::Action::ExecutionException)

/*
#define SCRIPT_CONSTRUCTOR(CLASSNAME) \
static QScriptValue ScriptConstructor(QScriptContext *context, QScriptEngine *engine) \
{ return engine->newQObject(new CLASSNAME##Instance(context->argument(0).toQObject()), QScriptEngine::ScriptOwnership); }
*/
#define SCRIPT_CONSTRUCTOR(CLASSNAME)

/*
#define SCRIPT_INIT(CLASSNAME) { \
QScriptValue ctor = scriptEngine->newFunction(&CLASSNAME##Instance::ScriptConstructor); \
QScriptValue metaObject = scriptEngine->newQMetaObject(&CLASSNAME##Instance::staticMetaObject, ctor); \
scriptEngine->globalObject().setProperty(#CLASSNAME, metaObject); }
*/
/*
#define SCRIPT_INIT(CLASSNAME) { \
QScriptValue ctor = scriptEngine->newFunction(&CLASSNAME##Instance::ScriptConstructor); \
QScriptValue metaObject = scriptEngine->newQMetaObject(&CLASSNAME##Instance::staticMetaObject, ctor); \
scriptEngine->globalObject().setProperty(#CLASSNAME "Class", metaObject); \
scriptEngine->globalObject().setProperty(#CLASSNAME, scriptEngine->evaluate("new " #CLASSNAME "Class()")); \
}
*/

#define SCRIPT_INIT(CLASSNAME) \
{ \
	QObject *object = new CLASSNAME##Instance(); \
	QScriptValue scriptObject = scriptEngine->newQObject(object, QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater); \
	scriptEngine->globalObject().setProperty(#CLASSNAME, scriptObject); \
	for(int enumeratorIndex = 0; enumeratorIndex < object->metaObject()->enumeratorCount(); ++enumeratorIndex) \
	{ \
		const QMetaEnum &metaEnum = object->metaObject()->enumerator(enumeratorIndex); \
		for(int keyIndex = 0; keyIndex < metaEnum.keyCount(); ++keyIndex) \
		{ \
			scriptObject.setProperty(metaEnum.key(keyIndex), metaEnum.value(keyIndex)); \
		} \
	} \
}

#endif // ACTION_H
