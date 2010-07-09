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

#ifndef ACTIONNOTIFY_H
#define ACTIONNOTIFY_H

#include "actioninterface.h"
#include "actionnotifyinstance.h"
#include "textparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "fileparameterdefinition.h"

namespace ActionTools
{
	class ActionPackInterface;
	class Action;
}

class ActionNotify : public QObject, public ActionTools::ActionInterface
{
   Q_OBJECT

public:
	explicit ActionNotify(ActionTools::ActionPackInterface *pack)
	: ActionInterface(pack)
	{
		ActionTools::TextParameterDefinition *title = new ActionTools::TextParameterDefinition( ActionTools::ElementDefinition::INPUT,
																								"title",
																								 tr("Title"),
																								 this);
		title->setTooltip(tr("The notification title"));
		addElement(title);
		
		ActionTools::TextParameterDefinition *text = new ActionTools::TextParameterDefinition( ActionTools::ElementDefinition::INPUT,
																								"text",
																								 tr("Text"),
																								 this);
		text->setTooltip(tr("The notification text"));
		addElement(text);
		
		ActionTools::NumberParameterDefinition *timeout = new ActionTools::NumberParameterDefinition( ActionTools::ElementDefinition::INPUT,
																								"timeout",
																								 tr("Timeout"),
																								 this);
		timeout->setTooltip(tr("The notification timeout"));
		timeout->setMinimum(0);
		timeout->setMaximum(INT_MAX);
		timeout->setOption("default", 3000);
		addElement(timeout, 1);
		
		ActionTools::FileParameterDefinition *icon = new ActionTools::FileParameterDefinition( ActionTools::ElementDefinition::INPUT,
																								"icon",
																								 tr("Icon"),
																								 this);
		icon->setTooltip(tr("The notification icon"));
		addElement(icon, 1);
	}

	QString name() const											{ return QObject::tr("Notify"); }
	QString id() const												{ return metaObject()->className(); }
	Flag flags() const												{ return WorksOnGnuLinux | Official; }
	QString description() const										{ return QObject::tr("Shows a message using the notify system"); }
	Tools::Version version() const									{ return Tools::Version(1, 0, 0); }
	ActionTools::Action *newAction()								{ return new ActionNotifyInstance(this, 0); }
	Status status() const											{ return Stable; }
	Category category() const										{ return System; }
	QPixmap icon() const											{ return QPixmap(":/icons/clipboard.png"); }
	ActionTools::Action *scriptInit(QScriptEngine *scriptEngine)	{ SCRIPT_INIT(ActionNotify) }
	QStringList tabs() const										{ return QStringList() << tr("Standard") << tr("Advanced"); }

private:
	Q_DISABLE_COPY(ActionNotify)
};

#endif // ACTIONNOTIFY_H
