/*
	Actiona
	Copyright (C) 2005 Jonathan Mercier-Ganady

	Actiona is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actiona is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact: jmgr@jmgr.info
*/

#pragma once

#include "actiontools_global.hpp"
#include "scriptparameter.hpp"
#include "resource.hpp"

#include <QVariant>
#include <QStringList>
#include <QMap>
#include <QStack>
#include <QVersionNumber>

#include <functional>
#include <utility>

class QIODevice;

namespace ActionTools
{
	class ActionInstance;
	class ActionFactory;
    class ElementDefinition;
    class ScriptLineModel;

	class ACTIONTOOLSSHARED_EXPORT Script : public QObject
	{
		Q_OBJECT

	public:
		enum ReadResult
		{
            ReadSuccess,                // Ok
            ReadInternal,               // Internal error
            ReadInvalidScriptVersion,	// Script version is newer than ours
            ReadCanceled                // Loading was canceled
		};

        static const QRegularExpression CodeVariableDeclarationRegExp;

		Script(ActionFactory *actionFactory, QObject *parent = nullptr);
		~Script() override;

        void appendAction(ActionInstance *actionInstance);
		ActionInstance *appendAction(const QString &actionDefinitionId);
		ActionInstance *actionAt(int line) const;
		void insertAction(int line, ActionInstance *actionInstance);
		void setAction(int line, ActionInstance *actionInstance);
		void removeActions(int line, int count);
		void removeAction(int line);
		void removeAction(ActionInstance *actionInstance);
		void removeAll();
		void moveAction(int startLine, int endLine);
        int actionCount() const                                                         { return mActionInstances.count(); }
		int labelLine(const QString &label) const;
		bool hasEnabledActions() const;
		QSet<int> usedActions() const;

        void invalidateLabelList()                                                      { mRebuildLabelList = true; }

        bool write(QIODevice *device, const QVersionNumber &programVersion, const QVersionNumber &scriptVersion, std::function<void(int, int, QString)> *progressCallback = nullptr);
        ReadResult read(QIODevice *device,
                        const QVersionNumber &scriptVersion,
                        std::function<void(int, int, QString)> *progressCallback = nullptr,
                        std::function<void()> *resetCallback = nullptr,
                        std::function<void(QList<ActionTools::ActionInstance *>)> *addActionsCallback = nullptr);

        const QString &statusMessage() const                                            { return mStatusMessage; }
        int line() const                                                                { return mLine; }
        int column() const                                                          	{ return mColumn; }
        const QString &programName() const                                          	{ return mProgramName; }
        const QVersionNumber &programVersion() const                                    { return mProgramVersion; }
        const QVersionNumber &scriptVersion() const                                     { return mScriptVersion; }
        const QString &os() const                                                       { return mOs; }
        const QStringList &missingActions() const                                       { return mMissingActions; }
        int pauseBefore() const                                                         { return mPauseBefore; }
        int pauseAfter() const                                                          { return mPauseAfter; }

        void addParameter(const ScriptParameter &parameter)                             { mParameters.append(parameter); }
        int parameterCount() const                                                      { return mParameters.count(); }
        const ScriptParameter &parameter(int index) const                               { return mParameters.at(index); }
        void removeAllParameters()                                                      { mParameters.clear(); }
        QList<ScriptParameter> parameters() const                                       { return mParameters; }
        void setPauseBefore(int pauseBefore)                                            { mPauseBefore = pauseBefore; }
        void setPauseAfter(int pauseAfter)                                              { mPauseAfter = pauseAfter; }

        void addProcedure(const QString &procedureName, int line)                       { mProcedures.insert(procedureName, line); }
        int findProcedure(const QString &procedureName) const                           { return mProcedures.value(procedureName, -1); }
        void clearProcedures()                                                          { mProcedures.clear(); }

        void addProcedureCall(int callerLine)                                           { mCallStack.push(callerLine); }
        bool hasProcedureCall() const                                                   { return !mCallStack.isEmpty(); }
        int popProcedureCall()                                                          { return mCallStack.pop(); }
        void clearCallStack()                                                           { mCallStack.clear(); }

        void addResource(const QString &id, const QByteArray &data, Resource::Type type){ mResources.insert(id, Resource(data, type)); }
        bool hasResource(const QString &id) const                                       { return mResources.contains(id); }
        Resource resource(const QString &id) const                                      { return mResources.value(id); }
        void clearResources()                                                           { mResources.clear(); }
		const QMap<QString, Resource> &resources() const								{ return mResources; }

        std::pair<int, int> minMaxExecutionCounter() const                              { return mMinMaxExecutionCounter; }
        bool hasBeenExecuted() const                                                    { return mMinMaxExecutionCounter.second > 0; }
        std::pair<qint64, qint64> minMaxExecutionDuration() const                       { return mMinMaxExecutionDuration; }

		int actionIndexFromRuntimeId(qint64 runtimeId) const;
		QStringList procedureNames() const;
		QStringList labels() const;
        QSet<QString> findVariables(ActionInstance *actionInstance = nullptr, ActionInstance *excludedActionInstance = nullptr) const;

        void executionStopped();

        ScriptLineModel *lineModel() const                                              { return mLineModel; }
        void updateLineModel();

	private:
        void parametersFromDefinition(QSet<QString> &variables, const ActionInstance *actionInstance, const ActionTools::ElementDefinition *elementDefinition) const;
        void findVariablesInAction(ActionInstance *actionInstance, QSet<QString> &result) const;

		QList<ScriptParameter> mParameters;
		QList<ActionInstance *> mActionInstances;
		ActionFactory *mActionFactory;
		QString mStatusMessage;
        int mLine{-1};
        int mColumn{-1};
		QString mProgramName;
		QVersionNumber mProgramVersion;
		QVersionNumber mScriptVersion;
		QString mOs;
		QStringList mMissingActions;
        int mPauseBefore{0};
        int mPauseAfter{0};
		QMap<QString, int> mProcedures;
		QStack<int> mCallStack;
		QMap<QString, Resource> mResources;
        std::pair<int, int> mMinMaxExecutionCounter;
        std::pair<qint64, qint64> mMinMaxExecutionDuration;
        ScriptLineModel *mLineModel;
        mutable QStringList mLabels;
        mutable bool mRebuildLabelList{};

		Q_DISABLE_COPY(Script)
	};
}

