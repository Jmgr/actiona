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

#include "writetextfileinstance.h"
#include "actioninstanceexecutionhelper.h"

#include <QFile>
#include <QTextStream>

namespace Actions
{
	ActionTools::StringListPair WriteTextFileInstance::modes = qMakePair(
			QStringList() << "truncate" << "append",
			QStringList() << QObject::tr("Overwrite any content") << QObject::tr("Append to the end"));

	void WriteTextFileInstance::startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
		QString filepath;
		QString text;
		Mode mode;

		if(!actionInstanceExecutionHelper.evaluateString(filepath, "file") ||
		   !actionInstanceExecutionHelper.evaluateString(text, "text") ||
		   !actionInstanceExecutionHelper.evaluateListElement(mode, modes, "mode"))
			return;

		QFile file(filepath);
		if(!file.open(QIODevice::WriteOnly | QIODevice::Text | (mode == Truncate ? QIODevice::Truncate : QIODevice::Append)))
		{
			actionInstanceExecutionHelper.setCurrentParameter("file");
			emit executionException(CannotWriteFileException, tr("Cannot open file"));
			return;
		}

		QTextStream textStream(&file);

		textStream << text;

		file.close();

		emit executionEnded();
	}
}
