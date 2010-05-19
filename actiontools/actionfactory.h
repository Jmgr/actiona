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

#ifndef ACTIONFACTORY_H
#define ACTIONFACTORY_H

#include "actiontools_global.h"
#include "actioninterface.h"

namespace ActionTools
{
	class ActionPackInterface;
	class Action;

	class ACTIONTOOLSSHARED_EXPORT ActionFactory : public QObject
	{
		Q_OBJECT

	public:
		explicit ActionFactory(QObject *parent = 0);
		~ActionFactory();

		void loadActionPacks();
		ActionInterface *actionInterface(const QString &actionId) const;
		ActionInterface *actionInterface(int index) const;
		Action *newAction(const QString &actionId) const;
		Action *newAction(ActionInterface *interface) const;
		int actionCount(ActionInterface::Category category = ActionInterface::None) const;
		int packCount()	const					{ return mActionPacks.count(); }

	signals:
		void packLoadError(QString error);

	private:
		void clear();
		void loadActionPack(const QString &filename);

		QList<ActionInterface *> mActions;
		QList<ActionPackInterface *> mActionPacks;

		Q_DISABLE_COPY(ActionFactory)
	};
}

#endif // ACTIONFACTORY_H
