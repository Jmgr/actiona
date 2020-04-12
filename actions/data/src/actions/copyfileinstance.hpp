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

#include <QFile>

namespace Actions
{
	class CopyFileInstance : public ActionTools::DataCopyActionInstance
	{
		Q_OBJECT

	public:
		enum Exceptions
		{
			UnableToReadFileException = ActionTools::ActionException::UserException,
			UnableToWriteFileException
		};

		CopyFileInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::DataCopyActionInstance(definition, parent)
		{
		}

		void startExecution() override
		{
			bool ok = true;

			QString sourceFile = evaluateString(ok, QStringLiteral("source"));
			QString destinationFile = evaluateString(ok, QStringLiteral("destination"));

			if(!ok)
				return;

			mSourceFile.setFileName(sourceFile);
			mDestinationFile.setFileName(destinationFile);

			if(!mSourceFile.open(QIODevice::ReadOnly))
			{
				setCurrentParameter(QStringLiteral("source"));
				emit executionException(UnableToReadFileException, tr("Unable to read the source file \"%1\"").arg(sourceFile));
				return;
			}

			if(!mDestinationFile.open(QIODevice::WriteOnly))
			{
				mSourceFile.close();
				setCurrentParameter(QStringLiteral("destination"));
				emit executionException(UnableToWriteFileException, tr("Unable to write to \"%1\"").arg(destinationFile));
				return;
			}

			DataCopyActionInstance::startCopy(&mSourceFile, &mDestinationFile);

			emit showProgressDialog(tr("Copying file"), 100);
			emit updateProgressDialog(tr("Copying in progress"));
		}

	private:
		QFile mSourceFile;
		QFile mDestinationFile;

		Q_DISABLE_COPY(CopyFileInstance)
	};
}

