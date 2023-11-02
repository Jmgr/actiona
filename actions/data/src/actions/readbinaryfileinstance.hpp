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

#include "actiontools/datacopyactioninstance.hpp"
#include "actiontools/code/rawdata.hpp"

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

		ReadBinaryFileInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::DataCopyActionInstance(definition, parent)
		{
		}

		void startExecution() override
		{
			bool ok = true;

			QString filename = evaluateString(ok, QStringLiteral("file"));
			mVariable = evaluateVariable(ok, QStringLiteral("variable"));

			if(!ok)
				return;

			mFile.setFileName(filename);

			if(!DataCopyActionInstance::startCopy(&mFile, &mResult))
			{
				setCurrentParameter(QStringLiteral("file"));
				emit executionException(UnableToReadFileException, tr("Unable to read the file \"%1\"").arg(filename));
				return;
			}

			emit showProgressDialog(tr("Reading file"), 100);
			emit updateProgressDialog(tr("Reading in progress"));
		}

	private slots:
		void done() override
		{
            setVariable(mVariable, Code::CodeClass::construct<Code::RawData>(mResult.buffer(), *scriptEngine()));

			DataCopyActionInstance::done();
		}

	private:
		void clean() override
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

