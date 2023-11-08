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
#include "tools/stringlistpair.hpp"

namespace ActionTools
{
	class CodeComboBox;
	class LineComboBox;
	class CodeLineEdit;

	class ACTIONTOOLSSHARED_EXPORT IfActionParameterDefinition : public ItemsParameterDefinition
	{
		Q_OBJECT

	public:
        enum Actions
        {
            DoNothing,
            Goto,
            RunCode,
            CallProcedure,
            StopExecution,
            Wait
        };

        static Tools::StringListPair actions;

        IfActionParameterDefinition(const Name &name, QObject *parent);

		void buildEditors(Script *script, QWidget *parent) override;
		void load(const ActionInstance *actionInstance) override;
		void save(ActionInstance *actionInstance) override;
        void applyDefaultValuesTo(ActionInstance *actionInstance) override;

        void setAllowWait(bool allowWait);
		
		void setDefaultAction(const QString &action)							{ mDefaultAction = action; }
		QString defaultAction(const QString &defaultValue = QString()) const	{ return (mDefaultAction.isEmpty()) ? defaultValue : mDefaultAction; }
		
		void setDefaultLine(const QString &line)								{ mDefaultLine = line; }
		QString defaultLine() const												{ return mDefaultLine; }
		
		void actionUpdate(Script *script) override;
		
	private slots:
		void codeChanged(bool code);
		void textChanged(const QString &text);

	private:
		enum Editor
		{
			LineEditor,
			CodeEditor,
			TextCodeEditor,
			ProcedureEditor,
			NoEditor
		};

		Editor findAppropriateEditor(const QString &actionText) const;
		void updateStatus(const QString &actionText);
        void setDefaultValue(const QString &defaultValue, const QString &subParameter = QStringLiteral("value")) override						{ Q_UNUSED(defaultValue); }

		static bool translated;

		CodeComboBox *mActionEdit;
		LineComboBox *mLineComboBox;
		CodeLineEdit *mCodeLineEdit;
		CodeComboBox *mProcedureComboBox;
		bool mAllowWait;
		QString mDefaultAction;
		QString mDefaultLine;
        Script *mScript;

		Q_DISABLE_COPY(IfActionParameterDefinition)
	};
}

