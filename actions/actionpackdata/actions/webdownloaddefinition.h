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

#include "actiondefinition.h"
#include "webdownloadinstance.h"
#include "textparameterdefinition.h"
#include "listparameterdefinition.h"
#include "variableparameterdefinition.h"
#include "fileparameterdefinition.h"
#include "groupdefinition.h"
#include "booleanparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class WebDownloadDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit WebDownloadDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("WebDownloadInstance::destinations", WebDownloadInstance::destinations);

			auto &url = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("url"), tr("URL")});
            url.setTooltip(tr("The URL of the data to download"));

			auto &destination = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("destination"), tr("Destination")});
            destination.setTooltip(tr("The destination where to write the downloaded data"));
            destination.setItems(WebDownloadInstance::destinations);
            destination.setDefaultValue(WebDownloadInstance::destinations.second.at(WebDownloadInstance::Variable));

            auto &variableMode = addGroup();
            variableMode.setMasterList(destination);
            variableMode.setMasterValues({WebDownloadInstance::destinations.first.at(WebDownloadInstance::Variable)});

            auto &variable = variableMode.addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("variable"), tr("Variable")});
            variable.setTooltip(tr("The variable where to write the downloaded data"));

            auto &fileMode = addGroup();
            fileMode.setMasterList(destination);
            fileMode.setMasterValues({WebDownloadInstance::destinations.first.at(WebDownloadInstance::File)});

            auto &file = fileMode.addParameter<ActionTools::FileParameterDefinition>({QStringLiteral("file"), tr("File")});
            file.setTooltip(tr("The file where to write the downloaded data"));
            file.setMode(ActionTools::FileEdit::FileSave);
            file.setCaption(tr("Choose the destination file"));
            file.setFilter(tr("All files (*.*)"));

            auto &showProgress = addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("showProgress"), tr("Show progress")}, 1);
            showProgress.setTooltip(tr("Should the download progress be shown"));
            showProgress.setDefaultValue(QStringLiteral("true"));

			addException(WebDownloadInstance::CannotOpenFileException, tr("Cannot write to file"));
			addException(WebDownloadInstance::DownloadException, tr("Download error"));
		}

		QString name() const override													{ return QObject::tr("Web download"); }
		QString id() const override														{ return QStringLiteral("ActionWebDownload"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Downloads data from the Web"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new WebDownloadInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Data; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/webdownload.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(WebDownloadDefinition)
	};
}

