/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#ifndef OPENURLINSTANCE_H
#define OPENURLINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "actioninstance.h"
#include "script.h"

#include <QUrl>
#include <QDesktopServices>

namespace Actions
{
	class OpenURLInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		enum Exceptions
		{
			FailedToOpenURL = ActionTools::ActionException::UserException
		};

		OpenURLInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)
		{
		}

		void startExecution()
		{
			ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
			QString urlString;

			if(!actionInstanceExecutionHelper.evaluateString(urlString, "url"))
				return;

			if(!QDesktopServices::openUrl(QUrl(urlString, QUrl::TolerantMode)))
			{
				emit executionException(FailedToOpenURL, tr("Failed to open URL"));
				return;
			}

			emit executionEnded();
		}

	private:
		Q_DISABLE_COPY(OpenURLInstance)
	};
}

#endif // OPENURLINSTANCE_H
