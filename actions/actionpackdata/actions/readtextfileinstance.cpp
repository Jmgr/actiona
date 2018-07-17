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

#include "readtextfileinstance.h"

#include <QFile>
#include <QTextStream>

namespace Actions
{
    Tools::StringListPair ReadTextFileInstance::modes =
    {
        {
            QStringLiteral("full"), QStringLiteral("selection")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("ReadTextFileInstance::modes", "Read the entire file")), QStringLiteral(QT_TRANSLATE_NOOP("ReadTextFileInstance::modes", "Read only a selection"))
        }
    };

	void ReadTextFileInstance::startExecution()
	{
		bool ok = true;

		QString filepath = evaluateString(ok, QStringLiteral("file"));
		QString variable = evaluateVariable(ok, QStringLiteral("variable"));
		Mode mode = evaluateListElement<Mode>(ok, modes, QStringLiteral("mode"));
		int firstline = evaluateInteger(ok, QStringLiteral("firstline"));
		int lastline = evaluateInteger(ok, QStringLiteral("lastline"));

		if(!ok)
			return;

		if(mode == Selection)
		{
			if(firstline < 1)
			{
				setCurrentParameter(QStringLiteral("firstline"));
				emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid first line value : %1").arg(firstline));
				return;
			}

			if(lastline < 1)
			{
				setCurrentParameter(QStringLiteral("lastline"));
				emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid last line value : %1").arg(lastline));
				return;
			}

			if(lastline < firstline)
			{
				setCurrentParameter(QStringLiteral("firstline"));
				emit executionException(ActionTools::ActionException::InvalidParameterException, tr("The first line has to be smaller than the last line"));
				return;
			}
		}

		QFile file(filepath);
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			setCurrentParameter(QStringLiteral("file"));
			emit executionException(CannotOpenFileException, tr("Cannot open file"));
			return;
		}

		//Line counting starts at 1
		--firstline;
		--lastline;

		QString result;
		QTextStream stream(&file);

		stream.setAutoDetectUnicode(true);

		if(mode == Full)
			result = stream.readAll();
		else
		{
			for(int line = 0; !stream.atEnd(); ++line)
			{
				QString readLine = stream.readLine();

				if(line >= firstline && line <= lastline)
				{
					if(!result.isEmpty())
						result += QLatin1Char('\n');

					result += readLine;
				}

				if(line > lastline)
					break;
			}
		}

        setVariable(variable, result);

		file.close();

		executionEnded();
	}
}
