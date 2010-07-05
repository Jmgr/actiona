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

#ifndef ACTIONINTERFACE_H
#define ACTIONINTERFACE_H

#include <QString>
#include <QPixmap>
#include <QList>
#include <QDebug>

#include "actiontools_global.h"
#include "version.h"
#include "elementdefinition.h"

class QScriptEngine;

namespace ActionTools
{
	class ActionPackInterface;
	class Action;

	class ACTIONTOOLSSHARED_EXPORT ActionInterface
	{
	public:
		typedef int Flag;

		enum Status
		{
			Alpha,
			Beta,
			Testing,
			Stable
		};

		enum Category
		{
			None = -1,
			Windows,
			Device,
			System,
			Internal,
			Other,

			CategoryCount
		};

		static QString CategoryName[CategoryCount];

		enum Flags
		{
			WorksOnWindows =    1 << 1,
			WorksOnGnuLinux =	1 << 2,
			WorksOnMac =	    1 << 3,
			Official =			1 << 4
		};

		explicit ActionInterface(ActionPackInterface *pack) : mPack(pack), mIndex(-1)	{}
		virtual ~ActionInterface() { qDeleteAll(mElements); }

		virtual QString name() const = 0;
		virtual QString id() const = 0;
		virtual Flag flags() const = 0;
		virtual QString description() const = 0;
		virtual Tools::Version version() const = 0;
		virtual Action *newAction() = 0;
		virtual Status status() const = 0;
		virtual Category category() const = 0;
		virtual QString author() const													{ return QString(); }
		virtual QString website() const													{ return QString(); }
		virtual QString email() const													{ return QString(); }
		virtual QPixmap icon() const = 0;
		virtual Action *scriptInit(QScriptEngine *scriptEngine) = 0;
		virtual QStringList tabs() const												{ return QStringList(); }

		void setIndex(int index)														{ mIndex = index; }
		int index() const																{ return mIndex; }

		ActionPackInterface *pack() const												{ return mPack; }
		const QList<ElementDefinition *> &elements() const								{ return mElements; }

	protected:
		void addElement(ElementDefinition *element, int tab = 0)
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

	private:
		ActionPackInterface *mPack;
		QList<ElementDefinition *> mElements;
		int mIndex;

		Q_DISABLE_COPY(ActionInterface)
	};
}

#endif // ACTIONINTERFACE_H
