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

#include "actiontools_global.hpp"

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
		explicit ScreenPositionWidget(QWidget *parent = nullptr);

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

