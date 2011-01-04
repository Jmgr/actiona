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

#include "actionfactory.h"
#include "actionpack.h"
#include "actiondefinition.h"

#include <QPluginLoader>
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QTranslator>

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

	void ActionFactory::loadActionPacks(const QString &locale)
	{
#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("ActionFactory loadActionPacks");
#endif
		clear();

		QDir actionDirectory(QApplication::applicationDirPath() + "/actions/");

	#ifdef Q_WS_WIN
		QString actionMask = "ActionPack*.dll";
	#endif
	#ifdef Q_WS_MAC
		QString actionMask = "libActionPack*.dylib";
	#endif
	#ifdef Q_WS_X11
		QString actionMask = "libActionPack*.so";
	#endif

		foreach(const QString actionFilename, actionDirectory.entryList(QStringList() << actionMask, QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks))
			loadActionPack(actionDirectory.absoluteFilePath(actionFilename), locale);

		qSort(mActionDefinitions.begin(), mActionDefinitions.end(), actionDefinitionLessThan);

		for(int index = 0; index < mActionDefinitions.count(); ++index)
			mActionDefinitions.at(index)->setIndex(index);
	}

	ActionDefinition *ActionFactory::actionDefinition(const QString &actionId) const
	{
		foreach(ActionDefinition *actionDefinition, mActionDefinitions)
		{
			if(actionDefinition->id() == actionId)
				return actionDefinition;
		}

		return 0;
	}

	ActionDefinition *ActionFactory::actionDefinition(int index) const
	{
		if(index < 0 || index >= mActionDefinitions.count())
			return 0;

		return mActionDefinitions.at(index);
	}
	
	ActionPack *ActionFactory::actionPack(int index) const
	{
		if(index < 0 || index >= mActionPacks.count())
			return 0;

		return mActionPacks.at(index);
	}

	ActionInstance *ActionFactory::newActionInstance(const QString &actionDefinitionId) const
	{
		ActionDefinition *definition = actionDefinition(actionDefinitionId);

		if(!definition)
			return 0;

		return definition->newActionInstance();
	}

	int ActionFactory::actionDefinitionCount(ActionCategory category) const
	{
		if(category == None)
			return mActionDefinitions.count();

		int count = 0;
		foreach(const ActionDefinition *actionDefinition, mActionDefinitions)
		{
			if(actionDefinition->category() == category)
				++count;
		}

		return count;
	}

	void ActionFactory::clear()
	{
		qDeleteAll(mActionDefinitions);
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
			emit actionPackLoadError(tr("%1: bad definition version").arg(shortFilename));
			return;
		}

		QTranslator *actionPackTranslator = new QTranslator(this);
		actionPackTranslator->load(QString("%1/locale/actionpack%2_%3").arg(QApplication::applicationDirPath()).arg(actionPack->id()).arg(locale));
		QApplication::installTranslator(actionPackTranslator);

		actionPack->createDefinitions();

		foreach(ActionDefinition *definition, actionPack->actionsDefinitions())
		{
			if(actionDefinition(definition->id()))
			{
				emit actionPackLoadError(tr("%1: <b>%2</b> already loaded").arg(shortFilename).arg(definition->id()));
				continue;
			}

		#ifdef Q_WS_WIN
			if(!(definition->flags() & WorksOnWindows))
				continue;
		#endif
		#ifdef Q_WS_X11
			if(!(definition->flags() & WorksOnGnuLinux))
				continue;
		#endif
		#ifdef Q_WS_MAC
			if(!(definition->flags() & WorksOnMac))
				continue;
		#endif

			QStringList missingFeatures;
			if(!definition->requirementCheck(missingFeatures))
			{
				emit actionPackLoadError(tr("%1: <b>%2</b> cannot be loaded:<ul><li>%3</ul>")
								   .arg(shortFilename)
								   .arg(definition->id())
								   .arg(missingFeatures.join("<li>")));//TODO
				continue;
			}

			mActionDefinitions << definition;
		}

		actionPack->setFilename(filename);

		mActionPacks << actionPack;
	}
}
