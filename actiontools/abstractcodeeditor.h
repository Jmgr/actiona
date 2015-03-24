/*
	Actiona
	Copyright (C) 2008-2015 Jonathan Mercier-Ganady

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

#ifndef ABSTRACTCODEEDITOR_H
#define ABSTRACTCODEEDITOR_H

#include "actiontools_global.h"

#include <QPainter>

class QAbstractItemModel;

namespace ActionTools
{
    class ParameterContainer;

	class ACTIONTOOLSSHARED_EXPORT AbstractCodeEditor
	{
	public:
		virtual ~AbstractCodeEditor()			{}

		virtual void openEditor(int line = -1, int column = -1) = 0;
		virtual void setCompletionModel(QAbstractItemModel *completionModel) = 0;
        virtual void setParameterContainer(const ParameterContainer *parameterContainer) = 0;
        virtual QSet<QString> findVariables() const = 0;
	};
}

#endif // ABSTRACTCODEEDITOR_H
