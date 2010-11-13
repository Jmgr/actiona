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
#include "actiondefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;

	class ACTIONTOOLSSHARED_EXPORT ActionFactory : public QObject
	{
		Q_OBJECT

	public:
		explicit ActionFactory(QObject *parent = 0);
		~ActionFactory();

		void loadActionPacks(const QString &locale);
		ActionDefinition *actionDefinition(const QString &actionId) const;
		ActionDefinition *actionDefinition(int index) const;
		ActionPack *actionPack(int index) const;
		ActionInstance *newActionInstance(const QString &actionDefinitionId) const;
		int actionDefinitionCount(ActionDefinition::Category category = ActionDefinition::None) const;
		int actionPackCount()	const					{ return mActionPacks.count(); }

	signals:
		void actionPackLoadError(QString error);

	private:
		void clear();
		void loadActionPack(const QString &filename, const QString &locale);

		QList<ActionDefinition *> mActionDefinitions;
		QList<ActionPack *> mActionPacks;

		Q_DISABLE_COPY(ActionFactory)
	};
}

#endif // ACTIONFACTORY_H
