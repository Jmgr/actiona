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
        ParameterDefinition(const Name &name, QObject *parent = nullptr);
        ~ParameterDefinition()                                                  override = default;

		virtual void buildEditors(Script *script, QWidget *parent);
		virtual void load(const ActionInstance *actionInstance) = 0;
		virtual void save(ActionInstance *actionInstance) = 0;
        void setDefaultValues(ActionInstance *actionInstance) override;
        virtual Qt::Orientation editorsOrientation() const                              { return Qt::Horizontal; }

        const QList<QWidget *> &editors() const                                         { return mEditors; }
        Flag operatingSystems() const                                                   { return mOperatingSystems; }
		
        virtual QString defaultValue(QString defaultValue = QString()) const;
        virtual void setDefaultValue(const QString &defaultValue)                       { mDefaultValue = defaultValue; }
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
        QString mDefaultValue;
		Flag mOperatingSystems;
		QWidget *mParentWidget;

		Q_DISABLE_COPY(ParameterDefinition)
	};
}

