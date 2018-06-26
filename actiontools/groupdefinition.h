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
#include "parameterdefinition.h"
#include "actiontools_global.h"

#include <QList>
#include <QStringList>

namespace ActionTools
{
	class ListParameterDefinition;
	class CodeComboBox;

	class ACTIONTOOLSSHARED_EXPORT GroupDefinition : public ElementDefinition
	{
		Q_OBJECT

	public:
		GroupDefinition(QObject *parent = nullptr);

        template<class ParameterDefinitionT>
        ParameterDefinitionT &addParameter(const Name &name, int tab = 0)
        {
            return *static_cast<ParameterDefinitionT *>(addParameter(new ParameterDefinitionT(name, this), tab));
        }

        QList<ParameterDefinition *> members() const                                    { return mMembers; }

        void setMasterList(ListParameterDefinition &masterList);
        void setMasterValues(const QStringList &masterValues)                           { mMasterValues = masterValues; }

		void init();
		
		void setDefaultValues(ActionInstance *actionInstance) override;

	private slots:
		void masterEditorBuilt();
		void masterTextChanged(const QString &text);
		void masterCodeChanged(bool code);

	private:
        ParameterDefinition *addParameter(ParameterDefinition *parameter, int tab = 0)
        {
            parameter->setTab(tab);

            mMembers.append(parameter);

            return parameter;
        }
		void enableMembers(bool enable);

		QList<ParameterDefinition *> mMembers;
		ListParameterDefinition *mMasterList{nullptr};
		QStringList mMasterValues;
		CodeComboBox *mMasterCodeComboBox{nullptr};

		Q_DISABLE_COPY(GroupDefinition)
	};
}

