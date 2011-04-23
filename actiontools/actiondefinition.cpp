/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#include "actiondefinition.h"
#include "actioninstance.h"
#include "actionexception.h"
#include "elementdefinition.h"

#include <QScriptEngine>
#include <QScriptValue>
#include <QSysInfo>
#include <QApplication>

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include "xdisplayhelper.h"
#endif

namespace ActionTools
{
	ActionDefinition::~ActionDefinition()
	{
		qDeleteAll(mElements);
		qDeleteAll(mExceptions);
	}
	
	QString ActionDefinition::CategoryName[CategoryCount] =
	{
		QT_TRANSLATE_NOOP("ActionDefinition::CategoryName", "Windows"),
		QT_TRANSLATE_NOOP("ActionDefinition::CategoryName", "Device"),
		QT_TRANSLATE_NOOP("ActionDefinition::CategoryName", "System"),
		QT_TRANSLATE_NOOP("ActionDefinition::CategoryName", "Internal"),
		QT_TRANSLATE_NOOP("ActionDefinition::CategoryName", "Data")
	};

	QStringList ActionDefinition::StandardTabs = QStringList() << QT_TRANSLATE_NOOP("ActionTabs", "Standard") << QT_TRANSLATE_NOOP("ActionTabs", "Advanced");
	
	void ActionDefinition::translateItems(const char *context, StringListPair &items) const
	{
		for(int index = 0; index < items.second.size(); ++index)
			items.second[index] = QApplication::instance()->translate(context, items.second.at(index).toLatin1());
	}

	void ActionDefinition::addElement(ElementDefinition *element, int tab)
	{
		if(tab > 0 && tabs().count() > 0)
		{
			if(tab < tabs().count())
				element->setTab(tab);
			else
				qWarning("Trying to add an element with an incorrect tab number");
		}

		mElements.append(element);
	}

	void ActionDefinition::addException(int id, const QString &name)
	{
		mExceptions.append(new ActionException(id, name));
	}

	bool ActionDefinition::requirementCheckXTest(QStringList &missingRequirements) const
	{
#ifdef Q_WS_X11
		int unused;
		XDisplayHelper xDisplayHelper;
		
		if(!XTestQueryExtension(xDisplayHelper.display(), &unused, &unused, &unused, &unused))
		{
			missingRequirements << QObject::tr("missing XTest extension");
			return false;
		}
		
		return true;
#else
		Q_UNUSED(missingRequirements)

		return true;
#endif
	}
}
