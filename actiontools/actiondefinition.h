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

	Contact : jmgr@jmgr.info
*/

#pragma once

#include "actiontools_global.h"
#include "version.h"
#include "stringlistpair.h"
#include "actiondefinitionenums.h"

#include <QString>
#include <QPixmap>
#include <QList>
#include <QDebug>
#include <QPixmapCache>

class QScriptEngine;

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
	class ElementDefinition;
    class ParameterDefinition;
    class GroupDefinition;
	class ActionException;
    class Name;

    class ACTIONTOOLSSHARED_EXPORT ActionDefinition : public QObject
	{
        Q_OBJECT

	public:
		explicit ActionDefinition(ActionPack *pack) : mPack(pack), mIndex(-1)	{}
		virtual ~ActionDefinition();

		virtual QString name() const = 0;
		virtual QString id() const = 0;
        virtual Flag flags() const                                                                      { return WorksOnWindows | WorksOnGnuLinux | WorksOnMac; }
        virtual QString description() const                                                             { return QObject::tr("No description"); }
        virtual Tools::Version version() const                                                          { return Tools::Version(1, 0, 0); }
		virtual ActionInstance *newActionInstance() const = 0;
        virtual ActionStatus status() const                                                             { return Stable; }
		virtual ActionCategory category() const = 0;
        virtual QString author() const                                                                  { return (flags() & Official) ? QObject::tr("The Actiona Team") : QString{}; }
        virtual QString website() const                                                                 { return {}; }
        virtual QString email() const                                                                   { return {}; }
        virtual QPixmap icon() const                                                                    { return {}; }
        QPixmap cachedIcon() const;
        virtual QStringList tabs() const                                                                { return {}; }

        virtual void updateAction(ActionInstance *actionInstance, const Tools::Version &version) const  { Q_UNUSED(actionInstance) Q_UNUSED(version) }

        void setIndex(int index)                                                                        { mIndex = index; }
        int index() const                                                                               { return mIndex; }

        ActionPack *pack() const                                                                        { return mPack; }
        const QList<ElementDefinition *> &elements() const                                              { return mElements; }
        const QList<ActionException *> &exceptions() const                                              { return mExceptions; }

		bool worksUnderThisOS() const;

        virtual bool requirementCheck(QStringList &missingRequirements) const                           { Q_UNUSED(missingRequirements) return true; }

		static QString CategoryName[CategoryCount];
		static QStringList StandardTabs;

	protected:
        void translateItems(const char *context, Tools::StringListPair &items) const;
        template<class ParameterDefinitionT>
        ParameterDefinitionT &addParameter(const Name &name, int tab = 0)
        {
            static_assert(std::is_convertible<ParameterDefinitionT*, ParameterDefinition*>::value, "ParameterDefinitionT must inherit ParameterDefinition");

            return *static_cast<ParameterDefinitionT *>(addElement(new ParameterDefinitionT(name, this), tab));
        }
        GroupDefinition &addGroup(int tab = 0);
		void addException(int id, const QString &name);
		bool requirementCheckXTest(QStringList &missingRequirements) const;

	private:
        ElementDefinition *addElement(ElementDefinition *element, int tab);

		ActionPack *mPack;
		QList<ElementDefinition *> mElements;
		QList<ActionException *> mExceptions;
		int mIndex;
        mutable QPixmap mIcon;

		Q_DISABLE_COPY(ActionDefinition)
	};
}

