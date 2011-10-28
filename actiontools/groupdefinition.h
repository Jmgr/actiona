/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#ifndef GROUPDEFINITION_H
#define GROUPDEFINITION_H

#include "elementdefinition.h"
#include "parameterdefinition.h"
#include "actiontools_global.h"

#include <QList>
#include <QStringList>

class QGroupBox;

namespace ActionTools
{
	class ListParameterDefinition;
	class CodeComboBox;

	class ACTIONTOOLSSHARED_EXPORT GroupDefinition : public ElementDefinition
	{
		Q_OBJECT

	public:
		GroupDefinition(const Name &name, QObject *parent = 0);

		void addMember(ParameterDefinition *parameter, int tab = 0)		{ parameter->setTab(tab); mMembers.append(parameter); }
		QList<ParameterDefinition *> members() const					{ return mMembers; }

		void setGroupBox(QGroupBox *groupBox)							{ mGroupBox = groupBox; }

		void setMasterList(ListParameterDefinition *masterList);
		void setMasterValues(const QStringList &masterValues)			{ mMasterValues = masterValues; }

		void init();
		
		void setDefaultValues(ActionInstance *actionInstance);

	private slots:
		void masterEditorBuilt();
		void masterTextChanged(const QString &text);
		void masterCodeChanged(bool code);

	private:
		QList<ParameterDefinition *> mMembers;
		ListParameterDefinition *mMasterList;
		QStringList mMasterValues;
		QGroupBox *mGroupBox;
		CodeComboBox *mMasterCodeComboBox;

		Q_DISABLE_COPY(GroupDefinition)
	};
}

#endif // GROUPDEFINITION_H
