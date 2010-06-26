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

#include "actiontools_global.h"
#include "version.h"
#include "scriptparameter.h"

#include <QVariant>
#include <QStringList>

class QIODevice;

namespace ActionTools
{
	class Action;
	class ActionFactory;

	class ACTIONTOOLSSHARED_EXPORT Script : public QObject
	{
		Q_OBJECT

	public:
		enum ReadResult
		{
			ReadSuccess,			// Ok
			ReadInternal,			// Internal error
			ReadBadSchema,			// Did not pass schema validation
			ReadBadScriptVersion	// Script version is newer than ours
		};
		
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
		bool hasEnabledActions() const;
		
		QStringList usedActions() const;

		bool write(QIODevice *device, const Tools::Version &programVersion, const Tools::Version &scriptVersion);
		ReadResult read(QIODevice *device, const Tools::Version &scriptVersion);
		bool validateContent(const QString &content);
		const QString &statusMessage() const								{ return mStatusMessage; }
		int line() const													{ return mLine; }
		int column() const													{ return mColumn; }		
		const QString &programName() const									{ return mProgramName; }
		const Tools::Version &programVersion() const						{ return mProgramVersion; }
		const Tools::Version &scriptVersion() const							{ return mScriptVersion; }
		const QString &os() const											{ return mOs; }
		const QStringList &missingActions() const							{ return mMissingActions; }

		void addParameter(const ScriptParameter &parameter)					{ mParameters.append(parameter); }
		int parameterCount() const											{ return mParameters.count(); }
		ScriptParameter parameter(int index) const							{ return mParameters.at(index); }
		void removeAllParameters()											{ mParameters.clear(); }
		QList<ScriptParameter> parameters() const							{ return mParameters; }

		QStringList labels() const;

		void setVariable(const QString &name, const QVariant &value)		{ if(!name.isEmpty()) mVariables.insert(name, value); }
		const QHash<QString, QVariant> &variables() const					{ return mVariables;  }
		QVariant variable(const QString &name) const						{ return mVariables.value(name); }
		void clearVariables()												{ mVariables.clear(); }
		bool hasVariable(const QString &name) const							{ return mVariables.find(name) != mVariables.end(); }

	private:
		QList<ScriptParameter> mParameters;
		QList<Action *> mActions;
		ActionFactory *mActionFactory;
		QHash<QString, QVariant> mVariables;
		QString mStatusMessage;
		int mLine;
		int mColumn;
		QString mProgramName;
		Tools::Version mProgramVersion;
		Tools::Version mScriptVersion;
		QString mOs;
		QStringList mMissingActions;

		Q_DISABLE_COPY(Script)
	};
}

#endif // SCRIPT_H
