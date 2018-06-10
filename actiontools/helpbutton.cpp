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

	Contact : jmgr@jmgr.info
*/

#include "helpbutton.h"

#include <QDesktopServices>
#include <QUrl>

namespace ActionTools
{
	HelpButton::HelpButton(QWidget *parent)
		: QPushButton(parent)
	{
		setText(QString());
		setIcon(QIcon(QStringLiteral(":/images/help.png")));
		setIconSize(QSize(16, 16));
		setMaximumWidth(24);
		setMaximumHeight(24);
		
        connect(this, &HelpButton::clicked, this, &HelpButton::onClicked);
	}
	
    void HelpButton::onClicked()
	{
		if(mTopic.isEmpty())
			return;

		QDesktopServices::openUrl(QUrl(QStringLiteral("http://wiki.actiona.tools/doku.php?id=%1").arg(mTopic)));
	}
}
