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
#include "actionpackinterface.h"
#include "actioninterface.h"

#include <QPluginLoader>
#include <QDir>
#include <QFileInfo>

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

	void ActionFactory::loadActionPacks()
	{
		clear();

		QDir actionDirectory(QDir::currentPath() + "/actions/");

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
			loadActionPack(actionDirectory.absoluteFilePath(actionFilename));
	}

	ActionInterface *ActionFactory::actionInterface(const QString &actionId) const
	{
		foreach(ActionInterface *actionInterface, mActions)
		{
			if(actionInterface->id() == actionId)
				return actionInterface;
		}

		return 0;
	}

	ActionInterface *ActionFactory::actionInterface(int index) const
	{
		if(index < 0 || index >= mActions.count())
			return 0;

		return mActions.at(index);
	}

	Action *ActionFactory::newAction(const QString &actionId) const
	{
		ActionInterface *interface = actionInterface(actionId);

		if(!interface)
			return 0;

		return newAction(interface);
	}

	Action *ActionFactory::newAction(ActionInterface *interface) const
	{
		return interface->newAction();
	}

	int ActionFactory::actionCount(ActionInterface::Category category) const
	{
		if(category == ActionInterface::None)
			return mActions.count();

		int count = 0;
		foreach(const ActionInterface *actionInterface, mActions)
		{
			if(actionInterface->category() == category)
				++count;
		}

		return count;
	}

	void ActionFactory::clear()
	{
		qDeleteAll(mActions);
		qDeleteAll(mActionPacks);

		mActions.clear();
		mActionPacks.clear();
	}

	void ActionFactory::loadActionPack(const QString &filename)
	{
		QPluginLoader pluginLoader(filename);
		QObject *actionPack = pluginLoader.instance();
		QString shortFilename = QFileInfo(filename).baseName();

		if(!actionPack)
		{
			emit packLoadError(tr("%1 : \"%2\"").arg(shortFilename).arg(pluginLoader.errorString()));
			return;
		}

		ActionPackInterface *actionPackInterface = qobject_cast<ActionPackInterface *>(actionPack);
		if(!actionPackInterface)
		{
			emit packLoadError(tr("%1 : bad interface version").arg(shortFilename));
			return;
		}

		foreach(ActionInterface *interface, actionPackInterface->actionsInterfaces())
		{
			if(actionInterface(interface->id()))
			{
				emit packLoadError(tr("%1 : <b>%2</b> already loaded").arg(shortFilename).arg(interface->id()));
				continue;
			}

		#ifdef Q_WS_WIN
			if(!(interface->flags() & ActionInterface::WorksOnWindows))
				continue;
		#endif
		#ifdef Q_WS_X11
			if(!(interface->flags() & ActionInterface::WorksOnGnuLinux))
				continue;
		#endif
		#ifdef Q_WS_MAC
			if(!(interface->flags() & ActionInterface::WorksOnMac))
				continue;
		#endif

			mActions << interface;
		}

		mActionPacks << actionPackInterface;
	}
}
