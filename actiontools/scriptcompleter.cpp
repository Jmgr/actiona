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

#include "scriptcompleter.h"

namespace ActionTools
{
	ScriptCompleter::ScriptCompleter(QObject *parent)
		: QCompleter(parent)
	{
	}
	
	ScriptCompleter::ScriptCompleter(QAbstractItemModel *model, QObject *parent)
		: QCompleter(model, parent)
	{
	}
	
	QStringList ScriptCompleter::splitPath(const QString &path) const
	{
		return path.split(QStringLiteral("."));
	}
	
	QString ScriptCompleter::pathFromIndex(const QModelIndex &index) const
	{
		QStringList dataList;
		
		for(QModelIndex i = index; i.isValid(); i = i.parent())
			dataList.prepend(model()->data(i, completionRole()).toString());
   
		return dataList.join(QStringLiteral("."));
	}
}
