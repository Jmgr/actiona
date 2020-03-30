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

#include "actiontools/actioninstance.hpp"
#include "actiontools/script.hpp"

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

		OpenURLInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent)
		{
		}

		void startExecution() override
		{
			bool ok = true;

			QString urlString = evaluateString(ok, QStringLiteral("url"));

			if(!ok)
				return;

			QUrl url(urlString, QUrl::TolerantMode);
			if(!url.isValid())
			{
				emit executionException(FailedToOpenURL, tr("Failed to open URL"));
				return;
			}

			if(url.scheme() == QString())
				url = QUrl(QStringLiteral("http://") + urlString, QUrl::TolerantMode);

			if(!QDesktopServices::openUrl(url))
			{
				emit executionException(FailedToOpenURL, tr("Failed to open URL"));
				return;
			}

			executionEnded();
		}

	private:
		Q_DISABLE_COPY(OpenURLInstance)
	};
}

