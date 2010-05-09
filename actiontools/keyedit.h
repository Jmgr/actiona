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

#ifndef KEYEDIT_H
#define KEYEDIT_H

#include "codelineedit.h"
#include "actiontools_global.h"

#include <QKeySequence>

class QMenu;

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT KeyEdit : public CodeLineEdit
	{
		Q_OBJECT

	public:
		explicit KeyEdit(QWidget *parent = 0);

		QKeySequence	keySequence() const							{return mKeySequence;}
		void			setKeySequence(QKeySequence keySequence);

	protected:
		void contextMenuEvent(QContextMenuEvent *event);
		void keyPressEvent(QKeyEvent *event);

	private slots:
		void menuKeyTriggered();

	private:
		void addKeyToMenu(QMenu *menu, const QString &key);

		QKeySequence mKeySequence;

		Q_DISABLE_COPY(KeyEdit)
	};
}

#endif // KEYEDIT_H
