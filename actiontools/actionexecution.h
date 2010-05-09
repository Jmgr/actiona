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

#ifndef ACTIONEXECUTION_H
#define ACTIONEXECUTION_H

#include "actiontools_global.h"
#include "subparameter.h"
#include "action.h"
#include "stringlistpair.h"

#include <QObject>

class QScriptEngine;

namespace ActionTools
{
	class Script;
	
	class ACTIONTOOLSSHARED_EXPORT ActionExecution : public QObject
	{
		Q_OBJECT

	public:
		ActionExecution(Action *action, Script *script, QScriptEngine *scriptEngine);
		~ActionExecution();

		bool evaluateString(QString &buffer,
							const QString &parameterName,
							const QString &subParameterName = "value");
		bool evaluateVariable(QString &buffer,
							const QString &parameterName,
							const QString &subParameterName = "value");
		bool evaluateInteger(int &buffer,
							 const QString &parameterName,
							 const QString &subParameterName = "value");
		bool evaluateFloat(float &buffer,
						   const QString &parameterName,
						   const QString &subParameterName = "value");
		bool evaluateListElement(int &buffer,
								 const QString &parameterName,
								 const QString &subParameterName,
								 const StringListPair &listElements);
		
		QString nextLine() const;
		void setNextLine(const QString &nextLine);

		template<typename T>
		bool checkRange(T value, T min, T max)		{ return (value >= min && value <= max); }
		template<typename T>
		bool checkMin(T value, T min)				{ return (value >= min); }
		template<typename T>
		bool checkMax(T value, T max)				{ return (value <= max); }

		QString errorMessage()						{ return mErrorMessage; }

	signals:
		void evaluationError(	ActionTools::Action::ExecutionException exceptionType,
								const QString &message);

	private:
		Action *mAction;
		Script *mScript;
		QScriptEngine *mScriptEngine;
		QString mErrorMessage;
		QVariant mResult;
		QString mParameterName;
		QString mSubParameterName;

		bool evaluate(const SubParameter &toEvaluate);
	};
}

#endif // ACTIONEXECUTION_H
