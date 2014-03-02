/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef READBINARYFILEINSTANCE_H
#define READBINARYFILEINSTANCE_H

#include "datacopyactioninstance.h"
#include "code/rawdata.h"

#include <QFile>
#include <QBuffer>

namespace Actions
{
	class ReadBinaryFileInstance : public ActionTools::DataCopyActionInstance
	{
		Q_OBJECT

	public:
		enum Exceptions
		{
			UnableToReadFileException = ActionTools::ActionException::UserException
		};

		ReadBinaryFileInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::DataCopyActionInstance(definition, parent)
		{
		}

		void startExecution()
		{
			bool ok = true;

			QString filename = evaluateString(ok, "file");
			mVariable = evaluateVariable(ok, "variable");

			if(!ok)
				return;

			mFile.setFileName(filename);

			if(!DataCopyActionInstance::startCopy(&mFile, &mResult))
			{
				setCurrentParameter("file");
				emit executionException(UnableToReadFileException, tr("Unable to read the file \"%1\"").arg(filename));
				return;
			}

			emit showProgressDialog("Reading file", 100);
			emit updateProgressDialog("Reading in progress");
		}

	private slots:
		void done()
		{
            setVariable(mVariable, Code::RawData::constructor(mResult.buffer(), scriptEngine()));

			DataCopyActionInstance::done();
		}

	private:
		void clean()
		{
			DataCopyActionInstance::clean();

			mResult.buffer().clear();
		}

		QFile mFile;
		QBuffer mResult;
		QString mVariable;

		Q_DISABLE_COPY(ReadBinaryFileInstance)
	};
}

#endif // READBINARYFILEINSTANCE_H
