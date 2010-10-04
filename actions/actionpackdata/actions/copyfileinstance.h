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

#ifndef ACTIONCOPYFILEINSTANCE_H
#define ACTIONCOPYFILEINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "datacopyactioninstance.h"

#include <QFile>

class ActionCopyFileInstance : public ActionTools::DataCopyActionInstance
{
	Q_OBJECT

public:
	enum Exceptions
	{
		UnableToReadFileException = ActionTools::ActionException::UserException,
		UnableToWriteFileException
	};

	ActionCopyFileInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
		: ActionTools::DataCopyActionInstance(definition, parent)
	{
	}

	void startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
		QString sourceFile;
		QString destinationFile;

		if(!actionInstanceExecutionHelper.evaluateString(sourceFile, "source") ||
		   !actionInstanceExecutionHelper.evaluateString(destinationFile, "destination"))
			return;
		
		mSourceFile.setFileName(sourceFile);
		mDestinationFile.setFileName(destinationFile);
		
		if(!mSourceFile.open(QIODevice::ReadOnly))
		{
			actionInstanceExecutionHelper.setCurrentParameter("source");
			emit executionException(UnableToReadFileException, tr("Unable to read the source file \"%1\"").arg(sourceFile));
			return;
		}
		
		if(!mDestinationFile.open(QIODevice::WriteOnly))
		{
			mSourceFile.close();
			actionInstanceExecutionHelper.setCurrentParameter("destination");
			emit executionException(UnableToWriteFileException, tr("Unable to write to \"%1\"").arg(destinationFile));
			return;
		}

		DataCopyActionInstance::startCopy(&mSourceFile, &mDestinationFile);
		
		emit showProgressDialog("Copying file", 100);
		emit updateProgressDialog("Copying in progress");
	}

private:
	QFile mSourceFile;
	QFile mDestinationFile;
	
	Q_DISABLE_COPY(ActionCopyFileInstance)
};

#endif // ACTIONCOPYFILEINSTANCE_H
