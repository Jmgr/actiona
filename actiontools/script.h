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

#ifndef SCRIPT_H
#define SCRIPT_H

#include <QObject>
#include "actiontools_global.h"
#include "version.h"
#include "scriptparameter.h"

class QIODevice;

#include <QVariant>

namespace ActionTools
{
	class Action;
	class ActionFactory;

	class ACTIONTOOLSSHARED_EXPORT Script : public QObject
	{
		Q_OBJECT

	public:
		Script(ActionFactory *actionFactory, QObject *parent = 0);
		~Script();

		void appendAction(Action *action)									{ mActions.append(action); }
		Action *appendAction(const QString &actionId);
		Action *actionAt(int line) const;
		void insertAction(int line, Action *action);
		void setAction(int line, Action *action);
		void removeActions(int line, int count);
		void removeAction(int line);
		void removeAction(Action *action);
		void removeAll();
		void moveAction(int startLine, int endLine);
		int actionCount() const												{ return mActions.count(); }
		int labelLine(const QString &label) const;
		bool isActionUsed(const QString &actionId) const;
		bool isActionUsed(int actionIndex) const;
		bool hasEnabledActions() const;

		bool write(QIODevice *device, const Tools::Version &programVersion, const Tools::Version &scriptVersion);
		bool read(QIODevice *device);

		void addParameter(const ScriptParameter &parameter)					{ mParameters.append(parameter); }
		int parameterCount() const											{ return mParameters.count(); }
		ScriptParameter parameter(int index) const							{ return mParameters.at(index); }
		void removeAllParameters()											{ mParameters.clear(); }
		QList<ScriptParameter> parameters() const							{ return mParameters; }

		QStringList labels() const;

		void setOption(const QString &name, const QVariant &value)			{ mOptions.insert(name, value); }
		QVariant option(const QString &name) const							{ return mOptions.value(name); }
		
		void setVariable(const QString &name, const QVariant &value)		{ mVariables.insert(name, value); }
		const QHash<QString, QVariant> &variables() const					{ return mVariables;  }
		QVariant variable(const QString &name) const						{ return mVariables.value(name); }
		void clearVariables()												{ mVariables.clear(); }
		bool hasVariable(const QString &name) const							{ return mVariables.find(name) != mVariables.end(); }

	private:
		QList<ScriptParameter> mParameters;
		QList<Action *> mActions;
		ActionFactory *mActionFactory;
		QVariantHash mOptions;
		QHash<QString, QVariant> mVariables;

		Q_DISABLE_COPY(Script)
	};
}

#endif // SCRIPT_H
