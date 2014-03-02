/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef PARAMETERDEFINITION_H
#define PARAMETERDEFINITION_H

#include "elementdefinition.h"
#include "actiontools_global.h"
#include "parameter.h"
#include "actiondefinition.h"

#include <QList>

class QWidget;

namespace ActionTools
{
	class ActionInstance;
	class Script;

	class ACTIONTOOLSSHARED_EXPORT ParameterDefinition : public ElementDefinition
	{
		Q_OBJECT

	public:
        ParameterDefinition(const Name &name, QObject *parent = 0);
        virtual ~ParameterDefinition()                                                  {}

		virtual void buildEditors(Script *script, QWidget *parent);
		virtual void load(const ActionInstance *actionInstance) = 0;
		virtual void save(ActionInstance *actionInstance) = 0;
		virtual void setDefaultValues(ActionInstance *actionInstance);
        virtual Qt::Orientation editorsOrientation() const                              { return Qt::Horizontal; }

        const QList<QWidget *> &editors() const                                         { return mEditors; }
        Flag operatingSystems() const                                                   { return mOperatingSystems; }
		
		virtual QVariant defaultValue(QVariant defaultValue = QVariant()) const;
        virtual void setDefaultValue(const QVariant &defaultValue)                      { mDefaultValue = defaultValue; }
        virtual void setOperatingSystems(Flag operatingSystems)                         { mOperatingSystems = operatingSystems; }
		
        //Called when the action is edited
        virtual void actionUpdate(Script *script)                                       { Q_UNUSED(script) }

        QWidget *parentWidget()                                                         { return mParentWidget; }

    signals:
        void changed();
		
	protected:
		void addEditor(QWidget *editor);
		
	private:
		QList<QWidget *> mEditors;
		QVariant mDefaultValue;
		Flag mOperatingSystems;
		QWidget *mParentWidget;

		Q_DISABLE_COPY(ParameterDefinition)
	};
}

#endif // PARAMETERDEFINITION_H
