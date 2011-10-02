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

#ifndef WEBDOWNLOADDEFINITION_H
#define WEBDOWNLOADDEFINITION_H

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
	class WebDownloadDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit WebDownloadDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("WebDownloadInstance::destinations", WebDownloadInstance::destinations);

			ActionTools::TextParameterDefinition *url = new ActionTools::TextParameterDefinition("url", tr("URL"), this);
			url->setTooltip(tr("The URL of the data to download"));
			addElement(url);

			ActionTools::ListParameterDefinition *destination = new ActionTools::ListParameterDefinition("destination", tr("Destination"), this);
			destination->setTooltip(tr("The destination where to write the downloaded data"));
			destination->setItems(WebDownloadInstance::destinations);
			destination->setDefaultValue(WebDownloadInstance::destinations.second.at(WebDownloadInstance::Variable));
			addElement(destination);

			ActionTools::GroupDefinition *variableMode = new ActionTools::GroupDefinition("variablemode", QString(), this);
			variableMode->setMasterList(destination);
			variableMode->setMasterValues(QStringList() << WebDownloadInstance::destinations.first.at(WebDownloadInstance::Variable));

			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition("variable", tr("Variable"), this);
			variable->setTooltip(tr("The variable where to write the downloaded data"));
			variableMode->addMember(variable);

			addElement(variableMode);

			ActionTools::GroupDefinition *fileMode = new ActionTools::GroupDefinition("filemode", QString(), this);
			fileMode->setMasterList(destination);
			fileMode->setMasterValues(QStringList() << WebDownloadInstance::destinations.first.at(WebDownloadInstance::File));

			ActionTools::FileParameterDefinition *file = new ActionTools::FileParameterDefinition("file", tr("File"), this);
			file->setTooltip(tr("The file where to write the downloaded data"));
			file->setMode(ActionTools::FileEdit::FileSave);
			file->setCaption(tr("Choose the destination file"));
			file->setFilter(tr("All files (*.*)"));
			fileMode->addMember(file);

			addElement(fileMode);

			ActionTools::BooleanParameterDefinition *showProgress = new ActionTools::BooleanParameterDefinition("showProgress", tr("Show progress"), this);
			showProgress->setTooltip(tr("Should the download progress be shown"));
			showProgress->setDefaultValue(true);
			addElement(showProgress, 1);

			addException(WebDownloadInstance::CannotOpenFileException, tr("Cannot write to file"));
			addException(WebDownloadInstance::DownloadException, tr("Download error"));
		}

		QString name() const													{ return QObject::tr("Web download"); }
		QString id() const														{ return "ActionWebDownload"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Downloads data from the Web"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new WebDownloadInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Data; }
		QPixmap icon() const													{ return QPixmap(":/icons/webdownload.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(WebDownloadDefinition)
	};
}

#endif // WEBDOWNLOADDEFINITION_H
