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
		ActionData() : interface(0), enabled(true), selected(false), script(0), scriptEngine(0)		{}
		ActionData(const ActionData &other)
			: QSharedData(other),
			parametersData(other.parametersData),
			interface(other.interface),
			comment(other.comment),
			label(other.label),
			color(other.color),
			enabled(other.enabled),
			selected(other.selected),
			script(other.script),
			scriptEngine(other.scriptEngine)														{}

		ParametersData parametersData;
		ActionInterface *interface;
		QString comment;
		QString label;
		QColor color;
		bool enabled;
		bool selected;
		Script *script;
		QScriptEngine *scriptEngine;
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
		/*
		enum ExecutionEventType
		{
			BadParameterEvent,			//Bad parameter (parameter not found or invalid value)
			CodeErrorEvent,				//Error while evaluating the code
			
			UserEvent = 32				//Action defined event
		};
		enum ExecutionEventActionType
		{
			ContinueAction,				//Continue script execution
			FailAction,					//Show an error message and stop execution (default)
			DeactivateAction,			//Deactivate the failing action and continue
			GotoLineAction,				//Goto a line
			AskToContinueAction,		//Show a message box asking to continue, if not, fail
			
			UserAction = 32				//Action defined event action
		};
		
		struct UserEvent
		{
			int type;
			
		};
		
		void addUserEvent(const QString &id, const QString &name);
		*/
		//TODO
		
		/*
		 EventDefinition
		 {
			QString name;				//Translated name, if user event
			int event;
			int action;
			QVariant actionParameter;
		 }
		 Event
		 {
			int event;
			int action;
			QVariant message;
		 }
		 */

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

		void setupExecution(QScriptEngine *scriptEngine, Script *script)	{ d->scriptEngine = scriptEngine; d->script = script; }

		void copyActionDataFrom(const Action &other)
		{
			setComment(other.comment());
			setLabel(other.label());
			setParametersData(other.parametersData());
			setColor(other.color());
			setEnabled(other.isEnabled());
			setSelected(other.isSelected());
		}

	protected:
		QScriptEngine *scriptEngine() const									{ return d->scriptEngine; }
		Script *script() const												{ return d->script; }

	public slots:
		virtual void startExecution()										{}
		virtual void stopExecution()										{}

	signals:
		void executionException(ActionTools::Action::ExecutionException exceptionType,
								const QString &message);
		void executionEnded();
		void disableAction(bool disable = true);

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

#define SCRIPT_INIT(CLASSNAME) \
{ \
	ActionTools::Action *object = new CLASSNAME##Instance(); \
	QScriptValue scriptObject = scriptEngine->newQObject(object, QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater); \
	scriptEngine->globalObject().setProperty(#CLASSNAME, scriptObject); \
	for(int enumeratorIndex = 0; enumeratorIndex < object->metaObject()->enumeratorCount(); ++enumeratorIndex) \
	{ \
		const QMetaEnum &metaEnum = object->metaObject()->enumerator(enumeratorIndex); \
		QScriptValue enumObject = scriptEngine->newObject(); \
		scriptObject.setProperty(metaEnum.name(), enumObject); \
		for(int keyIndex = 0; keyIndex < metaEnum.keyCount(); ++keyIndex) \
		{ \
			enumObject.setProperty(metaEnum.key(keyIndex), metaEnum.value(keyIndex)); \
		} \
	} \
	return object; \
}

#endif // ACTION_H
