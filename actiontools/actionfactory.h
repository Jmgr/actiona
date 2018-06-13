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

#pragma once

#include "actiontools_global.h"
#include "actiondefinitionenums.h"

#include <QObject>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
	class ActionDefinition;

	class ACTIONTOOLSSHARED_EXPORT ActionFactory : public QObject
	{
		Q_OBJECT

	public:
		explicit ActionFactory(QObject *parent = nullptr);
		~ActionFactory() override ;

		void loadActionPacks(const QString &directory, const QString &locale);
		ActionDefinition *actionDefinition(const QString &actionId) const;
		ActionDefinition *actionDefinition(int index) const;
		ActionPack *actionPack(int index) const;
		ActionInstance *newActionInstance(const QString &actionDefinitionId) const;
		int actionDefinitionCount(ActionCategory category = None) const;
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

