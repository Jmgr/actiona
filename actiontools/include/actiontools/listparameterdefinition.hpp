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

#pragma once

#include "itemsparameterdefinition.hpp"
#include "actiontools_global.hpp"

namespace ActionTools
{
	class CodeComboBox;

	class ACTIONTOOLSSHARED_EXPORT ListParameterDefinition : public ItemsParameterDefinition
	{
		Q_OBJECT

	public:
        ListParameterDefinition(const Name &name, QObject *parent);
        ~ListParameterDefinition()	override = default;

        void buildEditors(Script *script, QWidget *parent) override;
        void load(const ActionInstance *actionInstance) override;
        void save(ActionInstance *actionInstance) override;

		CodeComboBox *codeComboBox() const										{ return mComboBox; }
		
	signals:
		void editorBuilt();

	protected:
		CodeComboBox *mComboBox;

	private:
		Q_DISABLE_COPY(ListParameterDefinition)

	};
}

