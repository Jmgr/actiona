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

#ifndef ACTIONINSTANCEEXECUTIONHELPER_H
#define ACTIONINSTANCEEXECUTIONHELPER_H

#include "actiontools_global.h"
#include "subparameter.h"
#include "actioninstance.h"
#include "stringlistpair.h"

#include <QObject>

class QScriptEngine;
class QRegExp;

namespace ActionTools
{
	class Script;
	class IfActionValue;

	class ACTIONTOOLSSHARED_EXPORT ActionInstanceExecutionHelper : public QObject
	{
		Q_OBJECT

	public:
		ActionInstanceExecutionHelper(ActionInstance *actionInstance, Script *script, QScriptEngine *scriptEngine);
		~ActionInstanceExecutionHelper();

		bool evaluateVariant(QVariant &buffer,
							const QString &parameterName,
							const QString &subParameterName = "value");
		bool evaluateString(QString &buffer,
							const QString &parameterName,
							const QString &subParameterName = "value");
		bool evaluateVariable(QString &buffer,
							const QString &parameterName,
							const QString &subParameterName = "value");
		bool evaluateInteger(int &buffer,
							 const QString &parameterName,
							 const QString &subParameterName = "value");
		bool evaluateBoolean(bool &buffer,
							 const QString &parameterName,
							 const QString &subParameterName = "value");
		bool evaluateFloat(float &buffer,
						   const QString &parameterName,
						   const QString &subParameterName = "value");
		bool evaluateIfAction(IfActionValue &buffer,
						   const QString &parameterName);

		//I have to put this method here because it's a template method
		template<typename T>
		bool evaluateListElement(T &buffer,
								 const StringListPair &listElements,
								 const QString &parameterName,
								 const QString &subParameterName = "value")
		{
			mParameterName = parameterName;
			mSubParameterName = subParameterName;

			const SubParameter &toEvaluate = mActionInstance->subParameter(parameterName, subParameterName);
			if(!evaluate(toEvaluate))
				return false;

			QString selectedItem = mResult.toString();

			for(int i=0;i<listElements.first.size();++i)//Search in the non-translated items
			{
				if(listElements.first.at(i) == selectedItem)
				{
					buffer = static_cast<T>(i);
					return true;
				}
			}

			for(int i=0;i<listElements.second.size();++i)//Then search in the translated items
			{
				if(listElements.second.at(i) == selectedItem)
				{
					buffer = static_cast<T>(i);
					return true;
				}
			}

			bool success;

			buffer = static_cast<T>(mResult.toInt(&success));

			if(!success || buffer < 0 || buffer >= listElements.first.count())
			{
				mErrorMessage = tr("\"%1\" is incorrect.").arg(toEvaluate.value().toString());
				emit evaluationException(ActionException::BadParameterException, mErrorMessage);
				return false;
			}

			return true;
		}

		bool evaluatePoint(QPoint &buffer,
						   const QString &parameterName,
						   const QString &subParameterName = "value");
		bool evaluateColor(QColor &buffer,
						   const QString &parameterName,
						   const QString &subParameterName = "value");

		QString nextLine() const;
		void setNextLine(const QString &nextLine);

		void setVariable(const QString &name, const QVariant &value);
		QVariant variable(const QString &name);

		void setCurrentParameter(const QString &parameterName, const QString &subParameterName = "value");

		template<typename T>
		bool checkRange(T value, T min, T max)		{ return (value >= min && value <= max); }
		template<typename T>
		bool checkMin(T value, T min)				{ return (value >= min); }
		template<typename T>
		bool checkMax(T value, T max)				{ return (value <= max); }

		QString errorMessage()						{ return mErrorMessage; }

	signals:
		void evaluationException(	int exception,
									const QString &message);

	private:
		ActionInstance *mActionInstance;
		Script *mScript;
		QScriptEngine *mScriptEngine;
		QString mErrorMessage;
		QVariant mResult;
		QString mParameterName;
		QString mSubParameterName;
		const QRegExp mNameRegExp;

		bool evaluate(const SubParameter &toEvaluate);
	};
}

#endif // ACTIONINSTANCEEXECUTIONHELPER_H
