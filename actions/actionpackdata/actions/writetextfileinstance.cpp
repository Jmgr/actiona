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

#include "writetextfileinstance.h"

#include <QFile>
#include <QTextStream>

namespace Actions
{
    Tools::StringListPair WriteTextFileInstance::modes =
    {
        {
            QStringLiteral("truncate"), QStringLiteral("append")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("WriteTextFileInstance::modes", "Overwrite any content")),
            QStringLiteral(QT_TRANSLATE_NOOP("WriteTextFileInstance::modes", "Append to the end"))
        }
    };

	void WriteTextFileInstance::startExecution()
	{
		bool ok = true;

		QString filepath = evaluateString(ok, QStringLiteral("file"));
		QString text = evaluateString(ok, QStringLiteral("text"));
		Mode mode = evaluateListElement<Mode>(ok, modes, QStringLiteral("mode"));

		if(!ok)
			return;

		QFile file(filepath);
		if(!file.open(QIODevice::WriteOnly | QIODevice::Text | (mode == Truncate ? QIODevice::Truncate : QIODevice::Append)))
		{
			setCurrentParameter(QStringLiteral("file"));
			emit executionException(CannotWriteFileException, tr("Cannot open file"));
			return;
		}

		QTextStream textStream(&file);

		textStream << text;

		file.close();

		executionEnded();
	}
}
