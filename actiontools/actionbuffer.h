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

#ifndef ACTIONBUFFER_H
#define ACTIONBUFFER_H

#include "actiontools_global.h"
#include "action.h"

#include <QSharedData>

namespace ActionTools
{
	class ActionBufferData : public QSharedData
	{
	public:
		ActionBufferData()									{}
		ActionBufferData(const ActionBufferData &other)
			: QSharedData(other),
			actionId(other.actionId)
		{
			action.copyActionDataFrom(other.action);
		}

		QString actionId;
		Action action;
	};

	class ACTIONTOOLSSHARED_EXPORT ActionBuffer
	{
	public:
		ActionBuffer(const QString &actionId, const Action &action)
		{
			d = new ActionBufferData();
			setActionId(actionId);
			setAction(action);
		}
		ActionBuffer(const QString &actionId, const QVariant &action)
		{
			d = new ActionBufferData();
			setActionId(actionId);
			setActionFromVariant(action);
		}
		ActionBuffer()
		{
			d = new ActionBufferData();
		}
		ActionBuffer(const ActionBuffer &other)
			: d(other.d)									{}

		void setActionId(const QString &actionId)			{ d->actionId = actionId; }
		void setAction(const Action &action)				{ d->action.copyActionDataFrom(action); }
		void setActionFromVariant(const QVariant &variant)	{ d->action.copyActionDataFrom(variant.value<Action>()); }

		QString actionId() const							{ return d->actionId; }
		Action action() const								{ return d->action; }
		QVariant actionAsVariant() const					{ return QVariant::fromValue<Action>(d->action); }

	private:
		QSharedDataPointer<ActionBufferData> d;
	};

	ACTIONTOOLSSHARED_EXPORT QDataStream &operator << (QDataStream &s, const ActionBuffer &actionBuffer);
	ACTIONTOOLSSHARED_EXPORT QDataStream &operator >> (QDataStream &s, ActionBuffer &actionBuffer);
}

Q_DECLARE_METATYPE(ActionTools::ActionBuffer)

#endif // ACTIONBUFFER_H
