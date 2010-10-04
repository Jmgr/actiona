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

#ifndef WRITEBINARYFILEINSTANCE_H
#define WRITEBINARYFILEINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "datacopyactioninstance.h"

#include <QFile>
#include <QBuffer>

namespace Actions
{
	class WriteBinaryFileInstance : public ActionTools::DataCopyActionInstance
	{
		Q_OBJECT

	public:
		enum Exceptions
		{
			UnableToWriteFileException = ActionTools::ActionException::UserException
		};

		WriteBinaryFileInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::DataCopyActionInstance(definition, parent)
		{
		}

		void startExecution()
		{
			ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
			QString filename;
			QVariant data;

			if(!actionInstanceExecutionHelper.evaluateString(filename, "file") ||
			   !actionInstanceExecutionHelper.evaluateVariant(data, "data"))
				return;

			mData = data.toByteArray();
			mFile.setFileName(filename);
			mDataBuffer.setBuffer(&mData);

			if(!DataCopyActionInstance::startCopy(&mDataBuffer, &mFile))
			{
				actionInstanceExecutionHelper.setCurrentParameter("file");
				emit executionException(UnableToWriteFileException, tr("Unable to write to the file \"%1\"").arg(filename));
				return;
			}

			emit showProgressDialog("Writing file", 100);
			emit updateProgressDialog("Writing in progress");
		}

	private:
		void clean()
		{
			DataCopyActionInstance::clean();

			mDataBuffer.buffer().clear();
		}

		QFile mFile;
		QByteArray mData;
		QBuffer mDataBuffer;

	private:
		Q_DISABLE_COPY(WriteBinaryFileInstance)
	};
}

#endif // WRITEBINARYFILEINSTANCE_H
