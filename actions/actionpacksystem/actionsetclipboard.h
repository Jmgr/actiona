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

#ifndef ACTIONSETCLIPBOARD_H
#define ACTIONSETCLIPBOARD_H

#include "actioninterface.h"
#include "actionsetclipboardinstance.h"
#include "textparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPackInterface;
	class Action;
}

class ActionSetClipboard : public QObject, public ActionTools::ActionInterface
{
   Q_OBJECT

public:
	explicit ActionSetClipboard(ActionTools::ActionPackInterface *pack)
	: ActionSetClipboard::ActionInterface(pack)
	{
		ActionTools::TextParameterDefinition *input = new ActionTools::TextParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"value",
																								 tr("Value"),
																								 this);
		input->setTooltip(tr("The new clipboard value"));
		addElement(input);
	}

	QString name() const											{ return QObject::tr("Set clipboard"); }
	QString id() const												{ return metaObject()->className(); }
	Flag flags() const												{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const										{ return QObject::tr("Set the clipboard contents"); }
	Tools::Version version() const									{ return Tools::Version(1, 0, 0); }
	ActionTools::Action *newAction()								{ return new ActionSetClipboardInstance(this, 0); }
	Status status() const											{ return Stable; }
	Category category() const										{ return System; }
	QPixmap icon() const											{ return QPixmap(":/icons/clipboard.png"); }
	ActionTools::Action *scriptInit(QScriptEngine *scriptEngine)	{ SCRIPT_INIT(ActionSetClipboard) }

private:
	Q_DISABLE_COPY(ActionSetClipboard)
};

#endif // ACTIONSETCLIPBOARD_H
