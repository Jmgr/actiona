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

#ifndef ACTIONINSTANCE_H
#define ACTIONINSTANCE_H

#include "actiontools_global.h"
#include "parameter.h"
#include "actionexception.h"
#include "ifactionvalue.h"
#include "stringlistpair.h"

#include <QObject>
#include <QSharedData>
#include <QColor>
#include <QScriptValue>
#include <QVariant>

class QScriptEngine;
class QDataStream;

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
		ActionInstanceData() : definition(0), enabled(true), selected(false), pauseBefore(0), pauseAfter(0), timeout(0), script(0), scriptEngine(0), scriptLine(0)
			{}
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
			scriptEngine(other.scriptEngine),
			scriptLine(other.scriptLine),
			runtimeParameters(other.runtimeParameters)
			{}

		bool operator==(const ActionInstanceData &other) const;
		
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
		int scriptLine;
		QVariantHash runtimeParameters;
	};

	class ACTIONTOOLSSHARED_EXPORT ActionInstance : public QObject
	{
		Q_OBJECT

	public:
		ActionInstance(const ActionDefinition *definition = 0, QObject *parent = 0);
		ActionInstance(const ActionInstance &other);

		virtual ~ActionInstance()											{}
		
		bool operator==(const ActionInstance &other) const					{ return ((*d) == (*other.d)); }
		bool operator!=(const ActionInstance &other) const					{ return !((*d) == (*other.d)); }

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
		void setRuntimeParameter(const QString &parameterName, const QVariant &value)
																			{ d->runtimeParameters.insert(parameterName, value); }
		QVariant runtimeParameter(const QString &parameterName) const		{ return d->runtimeParameters.value(parameterName); }
		void clearRuntimeParameters()										{ d->runtimeParameters.clear(); }

		qint64 runtimeId() const											{ return mRuntimeId; }

		virtual void reset()												{}//This is called when this action should reset its counter (for loops)
		virtual void startExecution()										{}//This is called when the action should start its execution
		virtual void stopExecution()										{}//This is called when the action should break its execution
		virtual void stopLongTermExecution()								{}//This is called on script execution end, the action should stop its long term actions (ie continuous press of a key)
		virtual void pauseExecution()										{}//This is called when the action should pause its execution
		virtual void resumeExecution()										{}//This is called when the action should resume its execution

		void setupExecution(QScriptEngine *scriptEngine, Script *script, int scriptLine)
		{
			d->scriptEngine = scriptEngine;
			d->script = script;
			d->scriptLine = scriptLine;
		}

		void copyActionDataFrom(const ActionInstance &other);

	signals:
		void showProgressDialog(const QString &title, int maximum);
		void updateProgressDialog(const QString &caption);
		void updateProgressDialog(int value);
		void hideProgressDialog();
		void executionException(int exception, const QString &message);
		void executionEnded();
		void disableAction(bool disable = true);
		void consolePrint(const QString &text);
		void consolePrintWarning(const QString &text);
		void consolePrintError(const QString &text);

	protected:
		QScriptEngine *scriptEngine() const									{ return d->scriptEngine; }
		Script *script() const												{ return d->script; }
		int scriptLine() const												{ return d->scriptLine; }

		/** Parameter management **/
        QScriptValue evaluateValue(bool &ok,
                            const QString &parameterName,
                            const QString &subParameterName = "value");
		QVariant evaluateVariant(bool &ok,
							const QString &parameterName,
							const QString &subParameterName = "value");
		QString evaluateString(bool &ok,
							const QString &parameterName,
							const QString &subParameterName = "value");
		QString evaluateVariable(bool &ok,
							const QString &parameterName,
							const QString &subParameterName = "value");
		int evaluateInteger(bool &ok,
							 const QString &parameterName,
							 const QString &subParameterName = "value");
		bool evaluateBoolean(bool &ok,
							 const QString &parameterName,
							 const QString &subParameterName = "value");
		double evaluateDouble(bool &ok,
						   const QString &parameterName,
						   const QString &subParameterName = "value");
		IfActionValue evaluateIfAction(bool &ok,
						   const QString &parameterName);
		QString evaluateSubParameter(bool &ok,
								  const SubParameter &subParameter);

		template<typename T>
		T evaluateListElement(bool &ok,
								 const StringListPair &listElements,
								 const QString &parameterName,
								 const QString &subParameterName = "value")
		{
			if(!ok)
				return T();

			const SubParameter &subParameter = retreiveSubParameter(parameterName, subParameterName);
			QString result;

			if(subParameter.isCode())
				result = evaluateCode(ok, subParameter).toString();
			else
				result = evaluateText(ok, subParameter);

			if(!ok)
				return T();

			//Search in the non-translated items
			for(int i=0;i<listElements.first.size();++i)
			{
				if(listElements.first.at(i) == result)
					return static_cast<T>(i);
			}

			//Then search in the translated items
			for(int i=0;i<listElements.second.size();++i)
			{
				if(listElements.second.at(i) == result)
					return static_cast<T>(i);
			}

			T back = static_cast<T>(result.toInt(&ok));

			if(!ok || back < 0 || back >= listElements.first.count())
			{
				ok = false;

				emit executionException(ActionException::BadParameterException, tr("\"%1\" is an invalid value.").arg(result));

				return T();
			}

			return back;
		}

		QPoint evaluatePoint(bool &ok,
						   const QString &parameterName,
						   const QString &subParameterName = "value");
		QStringList evaluateItemList(bool &ok,
						   const QString &parameterName,
						   const QString &subParameterName = "value");
		QPolygon evaluatePolygon(bool &ok,
						   const QString &parameterName,
						   const QString &subParameterName = "value");
		QColor evaluateColor(bool &ok,
						   const QString &parameterName,
						   const QString &subParameterName = "value");

		QString nextLine() const;
		void setNextLine(const QString &nextLine);
		void setNextLine(int nextLine);

		void setArray(const QString &name, const QStringList &stringList);
		void setArrayKeyValue(const QString &name, const QStringList &Keys, const QStringList &Values);
		QScriptValue arrayElement(const QString &name, int index);

		void setVariable(const QString &name, const QScriptValue &value);
		QScriptValue variable(const QString &name);

		void setCurrentParameter(const QString &parameterName, const QString &subParameterName = "value");

		bool callProcedure(const QString &procedureName);

	private:
		SubParameter retreiveSubParameter(const QString &parameterName, const QString &subParameterName);
		QScriptValue evaluateCode(bool &ok, const SubParameter &toEvaluate);

		QString evaluateVariableArray(bool &ok, const QScriptValue &scriptValue);

		QString evaluateText(bool &ok, const SubParameter &toEvaluate);
		QString evaluateTextString(bool &ok, const QString &toEvaluate, int &pos);

		static const QRegExp mNumericalIndex;
		static const QRegExp mNameRegExp;
		static const QRegExp mVariableRegExp;
		static qint64 mCurrentRuntimeId;

		qint64 mRuntimeId;

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
