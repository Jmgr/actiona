/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#ifndef TEXT_H
#define TEXT_H

#include "actiontools_global.h"
#include "actioninstance.h"

#include <QSharedData>
#include <QString>

namespace ActionTools
{
	class Text
	{
	public:
		Text(QSharedDataPointer<ActionInstanceData> actionData, QString StringToEval);

		QString Parse(bool &ok);

	private:
		QSharedDataPointer<ActionInstanceData> d;

		int mIndex;
		QString mStringToEval;
		QString mResultOfEval;
	};
}

#endif // TEXT_H
