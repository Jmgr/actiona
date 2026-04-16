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

#include "actiontools/sessionguard.hpp"

#include <QLabel>
#include <QMessageBox>
#include <QProcessEnvironment>
#include <QTextStream>

namespace ActionTools
{
	namespace
	{
		const QString x11NotDetectedUrl = QStringLiteral("https://wiki.actiona.tools/doku.php?id=en:x11notdetected");
	}

	bool isWaylandSession()
	{
#ifdef Q_OS_UNIX
		return QProcessEnvironment::systemEnvironment().value(QStringLiteral("XDG_SESSION_TYPE")).compare(QStringLiteral("wayland"), Qt::CaseInsensitive) == 0;
#else
		return false;
#endif
	}

	int showUnsupportedSessionMessage(const QString &applicationName, QTextStream *stream)
	{
		if(stream)
		{
			*stream << QObject::tr("%1 does not support Wayland sessions, including XWayland. Please restart it in an X11 session.").arg(applicationName) << "\n";
			*stream << x11NotDetectedUrl << "\n";
			stream->flush();
		}

		QMessageBox box(QMessageBox::Critical,
						QObject::tr("Unsupported session"),
						QObject::tr("%1 does not support Wayland sessions, including XWayland.<br><br>Please restart it in an X11 session.<br><br><a href=\"%2\">More information</a>").arg(applicationName, x11NotDetectedUrl),
						QMessageBox::Ok,
						nullptr);
		box.setTextFormat(Qt::RichText);
		box.setTextInteractionFlags(Qt::TextBrowserInteraction);

		const auto labels = box.findChildren<QLabel *>();
		for(QLabel *label : labels)
		{
			label->setTextInteractionFlags(Qt::TextBrowserInteraction);
			label->setOpenExternalLinks(true);
		}

		box.exec();
		return -1;
	}
}
