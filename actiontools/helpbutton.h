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

#ifndef HELPBUTTON_H
#define HELPBUTTON_H

#include "actiontools_global.h"

#include <QPushButton>

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT HelpButton : public QPushButton
	{
		Q_OBJECT
		
	public:
		explicit HelpButton(QWidget *parent = 0);
		
		void key(const QString &key)						{ mKey = key; }
		
	private slots:
		void clicked();
		
	private:
		QString mKey;
	};
}

#endif // HELPBUTTON_H
