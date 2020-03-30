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

	Contact: jmgr@jmgr.info
*/

#include "newactionmodel.hpp"

#include <QMimeData>

NewActionModel::NewActionModel(QObject *parent)
    : QStandardItemModel(parent)
{
}

Qt::DropActions NewActionModel::supportedDropActions() const
{
	return Qt::CopyAction;
}

QMimeData *NewActionModel::mimeData(const QModelIndexList &indexes) const
{
    if(indexes.count() != 1)
		return nullptr;

	auto mimeData = new QMimeData();
	QByteArray data;

	data.append(indexes.at(0).data(ActionIdRole).toString().toUtf8());

	mimeData->setData(QStringLiteral("application/actiona.add.action"), data);

	return mimeData;
}
