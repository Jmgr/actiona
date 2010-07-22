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

#ifndef IFACTIONPARAMETERDEFINITION_H
#define IFACTIONPARAMETERDEFINITION_H

#include "itemsparameterdefinition.h"
#include "actiontools_global.h"
#include "stringlistpair.h"

namespace ActionTools
{
	class CodeComboBox;
	class LineComboBox;

	class ACTIONTOOLSSHARED_EXPORT IfActionParameterDefinition : public ItemsParameterDefinition
	{
		Q_OBJECT

	public:
		IfActionParameterDefinition(Category category, const QString &name, const QString &translatedName, QObject *parent);

		void buildEditors(Script *script, QWidget *parent);
		void load(const ActionInstance *actionInstance);
		void save(ActionInstance *actionInstance);
		void setDefaultValues(ActionInstance *actionInstance);

		void setAllowWait(bool allowWait)										{ mAllowWait = allowWait; }
		
		void setDefaultAction(const QString &action)							{ mDefaultAction = action; }
		QString defaultAction(const QString &defaultValue = QString()) const	{ return (mDefaultAction.isEmpty()) ? defaultValue : mDefaultAction; }
		
		void setDefaultLine(const QString &line)								{ mDefaultLine = line; }
		QString defaultLine() const												{ return mDefaultLine; }
		
	private slots:
		void codeChanged(bool code);
		void textChanged(const QString &text);

	private:
		enum Editor
		{
			ActionEditor,
			ParameterEditor
		};

		enum Actions
		{
			DoNothing,
			Goto,
			Wait
		};

		void updateStatus(const QString &text);
		QVariant defaultValue(QVariant defaultValue = QVariant()) const			{ Q_UNUSED(defaultValue); return QVariant(); }
		void setDefaultValue(const QVariant &defaultValue)						{ Q_UNUSED(defaultValue); }

		static StringListPair actions;

		CodeComboBox *mActionEdit;
		LineComboBox *mLineEdit;
		bool mAllowWait;
		QString mDefaultAction;
		QString mDefaultLine;

		Q_DISABLE_COPY(IfActionParameterDefinition)
	};
}

#endif // IFACTIONPARAMETERDEFINITION_H
