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

#include "actiontools/actionfactory.hpp"
#include "actiontools/actionpack.hpp"
#include "actiontools/actiondefinition.hpp"
#include "tools/languages.hpp"

#ifdef ACT_PROFILE
#include "tools/highresolutiontimer.hpp"
#endif

#include <QDir>
#include <QFileInfo>
#include <QApplication>

#include <algorithm>

namespace ActionTools
{
	ActionFactory::ActionFactory(QObject *parent) :
		QObject(parent)
	{
	}

	ActionFactory::~ActionFactory()
	{
		clear();
	}

	bool actionDefinitionLessThan(const ActionDefinition *s1, const ActionDefinition *s2)
	{
		return s1->name() < s2->name();
	}

    void ActionFactory::loadActionPacks(const QList<ActionPack *> packs, const QString &locale)
    {
        mActionPacks.clear();

        for(auto pack: packs)
        {
            Tools::Languages::installTranslator(QStringLiteral("actionpack%1").arg(pack->id()), locale);

            pack->createDefinitions();

            for(ActionDefinition *definition: pack->actionsDefinitions())
            {
                if(actionDefinition(definition->id()))
                {
                    emit actionPackLoadError(tr("%1: <b>%2</b> already loaded").arg(pack->id()).arg(definition->id()));
                    continue;
                }

                if(definition->worksUnderThisOS())
                {
                    QStringList missingFeatures;
                    if(!definition->requirementCheck(missingFeatures))
                    {
                        emit actionPackLoadError(tr("%1: <b>%2</b> cannot be loaded:<ul><li>%3</ul>")
                                                     .arg(pack->id())
                                                     .arg(definition->id())
                                                     .arg(missingFeatures.join(QStringLiteral("<li>"))));
                        continue;
                    }
                }

                mActionDefinitions << definition;
            }

            mActionPacks << pack;
        }

        std::sort(mActionDefinitions.begin(), mActionDefinitions.end(), actionDefinitionLessThan);

        for(int index = 0; index < mActionDefinitions.count(); ++index)
            mActionDefinitions.at(index)->setIndex(index);
    }

	ActionDefinition *ActionFactory::actionDefinition(const QString &actionId) const
	{
        for(ActionDefinition *actionDefinition: mActionDefinitions)
		{
			if(actionDefinition->id() == actionId)
				return actionDefinition;
		}

		return nullptr;
	}

	ActionDefinition *ActionFactory::actionDefinition(int index) const
	{
		if(index < 0 || index >= mActionDefinitions.count())
			return nullptr;

		return mActionDefinitions.at(index);
	}
	
	ActionPack *ActionFactory::actionPack(int index) const
	{
		if(index < 0 || index >= mActionPacks.count())
			return nullptr;

		return mActionPacks.at(index);
	}

	ActionInstance *ActionFactory::newActionInstance(const QString &actionDefinitionId) const
	{
		ActionDefinition *definition = actionDefinition(actionDefinitionId);

		if(!definition)
			return nullptr;

		return definition->newActionInstance();
	}

	int ActionFactory::actionDefinitionCount(ActionCategory category) const
	{
		if(category == None)
			return mActionDefinitions.count();

		int count = 0;
        for(const ActionDefinition *actionDefinition: mActionDefinitions)
		{
			if(actionDefinition->category() == category)
				++count;
		}

		return count;
	}

	void ActionFactory::clear()
	{
		qDeleteAll(mActionPacks);

		mActionDefinitions.clear();
		mActionPacks.clear();
	}
}
