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

#ifndef ACTIONTEXT_H
#define ACTIONTEXT_H

#include "actioninterface.h"
#include "actiontextinstance.h"
#include "textparameterdefinition.h"

namespace ActionTools
{
	class ActionPackInterface;
	class Action;
}

class ActionText : public QObject, public ActionTools::ActionInterface
{
   Q_OBJECT

public:
	explicit ActionText(ActionTools::ActionPackInterface *pack)
	: ActionText::ActionInterface(pack)
	{
		ActionTools::TextParameterDefinition *text = new ActionTools::TextParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"text",
																								tr("Text"),
																								this);
		text->setTooltip(tr("The text to write"));
		addElement(text);
	}

	QString name() const											{ return QObject::tr("Text"); }
	QString id() const												{ return metaObject()->className(); }
	Flag flags() const												{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const										{ return QObject::tr("Write some text"); }
	Tools::Version version() const									{ return Tools::Version(0, 0, 1); }
	ActionTools::Action *newAction()								{ return new ActionTextInstance(this, 0); }
	Status status() const											{ return Alpha; }
	Category category() const										{ return Device; }
	QPixmap icon() const											{ return QPixmap(":/icons/key.png"); }
	ActionTools::Action *scriptInit(QScriptEngine *scriptEngine)	{ SCRIPT_INIT(ActionText) }

private:
	Q_DISABLE_COPY(ActionText)
};

#endif // ACTIONTEXT_H
