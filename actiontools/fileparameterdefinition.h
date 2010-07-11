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

#ifndef FILEPARAMETERDEFINITION_H
#define FILEPARAMETERDEFINITION_H

#include "parameterdefinition.h"
#include "actiontools_global.h"
#include "fileedit.h"

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT FileParameterDefinition : public ParameterDefinition
	{
		Q_OBJECT

	public:
		FileParameterDefinition(Category category, const QString &name, const QString &translatedName, QObject *parent);

		void buildEditors(Script *script, QWidget *parent);
		void load(const ActionInstance *actionInstance);
		void save(ActionInstance *actionInstance);
		void setDefaultValues(Parameter &data);

		void setMode(FileEdit::Mode mode)							{ mMode = mode; }
		void setCaption(const QString &caption)						{ mCaption = caption; }
		void setFilter(const QString &filter)						{ mFilter = filter; }
		void setDirectory(const QString &directory)					{ mDirectory = directory; }

	private:
		FileEdit *mFileEdit;
		FileEdit::Mode mMode;
		QString mCaption;
		QString mFilter;
		QString mDirectory;

		Q_DISABLE_COPY(FileParameterDefinition)
	};
}

#endif // FILEPARAMETERDEFINITION_H
