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

#include "helpbutton.h"

#include <QMessageBox>
#include <QProcess>
#include <QLibraryInfo>
#include <QTextStream>

namespace ActionTools
{
	HelpButton::HelpButton(QWidget *parent)
		: QPushButton(parent)
	{
		setText(QString());
		setIcon(QIcon(":/images/help.png"));
		setIconSize(QSize(16, 16));
		setMaximumWidth(24);
		setMaximumHeight(24);
		
		connect(this, SIGNAL(clicked()), this, SLOT(clicked()));
	}
	
	void HelpButton::clicked()
	{
		//TODO : Start the assistant
		/*
		QProcess *process = new QProcess(this);
		QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QLatin1String("/assistant");
		
		process->start(app, QStringList() << QLatin1String("-enableRemoteControl"));
		if (!process->waitForStarted()) 
		{
			QMessageBox::critical(this, tr("Remote Control"),
			tr("Could not start Qt Assistant from %1.").arg(app));
			return;
		}
		
		// show index page
		QTextStream str(process);
		str << QLatin1String("SetSource qthelp://mycompany.com/doc/index.html") << QLatin1Char('\0') << endl;
		*/
	}
}
