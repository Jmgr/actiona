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

#include "actionfactory.h"
#include "actionpack.h"
#include "actiondefinition.h"
#include "languages.h"

#ifdef ACT_PROFILE
#include "highresolutiontimer.h"
#endif

#include <QPluginLoader>
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

	void ActionFactory::loadActionPacks(const QString &directory, const QString &locale)
	{
#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("ActionFactory loadActionPacks from " + directory);
#endif

		QDir actionDirectory(directory);

#if defined(Q_OS_WIN)
        QString actionMask = QStringLiteral("ActionPack*.dll");
#elif defined(Q_OS_MAC)
		QString actionMask = QStringLiteral("libActionPack*.dylib");
#else
		QString actionMask = QStringLiteral("libActionPack*.so");
#endif

        for(const QString &actionFilename: actionDirectory.entryList({actionMask}, QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks))
			loadActionPack(actionDirectory.absoluteFilePath(actionFilename), locale);

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

	void ActionFactory::loadActionPack(const QString &filename, const QString &locale)
	{
		QPluginLoader pluginLoader(filename);
		QObject *actionPackObject = pluginLoader.instance();
		QString shortFilename = QFileInfo(filename).baseName();

		if(!actionPackObject)
		{
			emit actionPackLoadError(tr("%1: \"%2\"").arg(shortFilename).arg(pluginLoader.errorString()));
			return;
		}

		ActionPack *actionPack = qobject_cast<ActionPack *>(actionPackObject);
		if(!actionPack)
		{
            emit actionPackLoadError(tr("%1: invalid definition version").arg(shortFilename));
			return;
		}

        Tools::Languages::installTranslator(QStringLiteral("actionpack%1").arg(actionPack->id()), locale);

		actionPack->createDefinitions();

        for(ActionDefinition *definition: actionPack->actionsDefinitions())
		{
			if(actionDefinition(definition->id()))
			{
				emit actionPackLoadError(tr("%1: <b>%2</b> already loaded").arg(shortFilename).arg(definition->id()));
				continue;
			}

			if(definition->worksUnderThisOS())
			{
				QStringList missingFeatures;
				if(!definition->requirementCheck(missingFeatures))
				{
					emit actionPackLoadError(tr("%1: <b>%2</b> cannot be loaded:<ul><li>%3</ul>")
									   .arg(shortFilename)
									   .arg(definition->id())
									   .arg(missingFeatures.join(QStringLiteral("<li>"))));
					continue;
				}
			}

			mActionDefinitions << definition;
		}

		actionPack->setFilename(filename);

		mActionPacks << actionPack;
	}
}
