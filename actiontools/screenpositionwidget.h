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

#ifndef SCREENPOSITIONWIDGET_H
#define SCREENPOSITIONWIDGET_H

#include "actiontools_global.h"

#include <QWidget>
#include <QList>

class QButtonGroup;
class QRadioButton;

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT ScreenPositionWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit ScreenPositionWidget(QWidget *parent = 0);

		void setPosition(int screen, int position);

		int screen() const;
		int position() const;

	private:
		QList<QRadioButton *> mRadioButtons;
		QButtonGroup *mButtonGroup;

		static QString iconNames[3][3];

		Q_DISABLE_COPY(ScreenPositionWidget)
	};
}

#endif // SCREENPOSITIONWIDGET_H
