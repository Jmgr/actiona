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

#pragma once

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

		WriteBinaryFileInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::DataCopyActionInstance(definition, parent)
		{
		}

		void startExecution() override
		{
			bool ok = true;

			QString filename = evaluateString(ok, QStringLiteral("file"));
			QScriptValue data = evaluateValue(ok, QStringLiteral("data"));

			if(!ok)
				return;

            mData = data.toVariant().toByteArray();
			mFile.setFileName(filename);
			mDataBuffer.setBuffer(&mData);

			if(!DataCopyActionInstance::startCopy(&mDataBuffer, &mFile))
			{
				setCurrentParameter(QStringLiteral("file"));
				emit executionException(UnableToWriteFileException, tr("Unable to write to the file \"%1\"").arg(filename));
				return;
			}

			emit showProgressDialog(tr("Writing file"), 100);
			emit updateProgressDialog(tr("Writing in progress"));
		}

	private:
		void clean() override
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

