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

#include "filedialog.h"
#include "code/code.h"

#include <QScriptValueIterator>
#include <QUrl>

namespace Code
{
	QScriptValue FileDialog::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		FileDialog *fileDialog = new FileDialog;
		fileDialog->setupConstructorParameters(context->argument(0));

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == "acceptMode")
				fileDialog->mFileDialog->setAcceptMode(static_cast<QFileDialog::AcceptMode>(it.value().toInt32()));
			else if(it.name() == "fileMode")
				fileDialog->mFileDialog->setFileMode(static_cast<QFileDialog::FileMode>(it.value().toInt32()));
			else if(it.name() == "viewMode")
				fileDialog->mFileDialog->setViewMode(static_cast<QFileDialog::ViewMode>(it.value().toInt32()));
			else if(it.name() == "selectedFile")
				fileDialog->mFileDialog->selectFile(it.value().toString());
			else if(it.name() == "selectedNameFilter")
				fileDialog->mFileDialog->selectNameFilter(it.value().toString());
			else if(it.name() == "defaultSuffix")
				fileDialog->mFileDialog->setDefaultSuffix(it.value().toString());
			else if(it.name() == "directory")
				fileDialog->mFileDialog->setDirectory(it.value().toString());
			else if(it.name() == "filter")
				fileDialog->mFileDialog->setFilter(static_cast<QDir::Filter>(it.value().toInt32()));
			else if(it.name() == "nameFilter")
				fileDialog->mFileDialog->setNameFilter(it.value().toString());
			else if(it.name() == "nameFilters")
				fileDialog->mFileDialog->setNameFilters(Code::arrayParameterToStringList(it.value()));
			else if(it.name() == "showDirectoriesOnly")
				fileDialog->mFileDialog->setOption(QFileDialog::ShowDirsOnly, it.value().toBool());
			else if(it.name() == "dontResolveSymlinks")
				fileDialog->mFileDialog->setOption(QFileDialog::DontResolveSymlinks, it.value().toBool());
			else if(it.name() == "dontConfirmOverwrite")
				fileDialog->mFileDialog->setOption(QFileDialog::DontConfirmOverwrite, it.value().toBool());
			else if(it.name() == "dontUseNativeDialog")
				fileDialog->mFileDialog->setOption(QFileDialog::DontUseNativeDialog, it.value().toBool());
			else if(it.name() == "readOnly")
				fileDialog->mFileDialog->setOption(QFileDialog::ReadOnly, it.value().toBool());
			else if(it.name() == "hideNameFilterDetails")
				fileDialog->mFileDialog->setOption(QFileDialog::HideNameFilterDetails, it.value().toBool());
			else if(it.name() == "sidebarUrls")
			{
				QList<QUrl> urls;
				
				foreach(const QString &url, Code::arrayParameterToStringList(it.value()))
					urls.append(QUrl::fromLocalFile(url));
					
				fileDialog->mFileDialog->setSidebarUrls(urls);
			}
			else if(it.name() == "onClosed")
				fileDialog->mOnClosed = it.value();
		}

		return fileDialog->mThisObject = engine->newQObject(fileDialog, QScriptEngine::ScriptOwnership);
	}
	
	FileDialog::FileDialog()
		: BaseWindow(),
		mFileDialog(new QFileDialog)
	{
		setWidget(mFileDialog);
		
		connect(mFileDialog, SIGNAL(finished(int)), this, SLOT(finished(int)));
	}
	
	FileDialog::~FileDialog()
	{
		delete mFileDialog;
	}
	
	QScriptValue FileDialog::setAcceptMode(AcceptMode acceptMode)
	{
		mFileDialog->setAcceptMode(static_cast<QFileDialog::AcceptMode>(acceptMode));
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setFileMode(FileMode fileMode)
	{
		mFileDialog->setFileMode(static_cast<QFileDialog::FileMode>(fileMode));
				
		return context()->thisObject();
	}

	QScriptValue FileDialog::setViewMode(ViewMode viewMode)
	{
		mFileDialog->setViewMode(static_cast<QFileDialog::ViewMode>(viewMode));
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setLabelText(DialogLabel dialogLabel, const QString &text)
	{
		mFileDialog->setLabelText(static_cast<QFileDialog::DialogLabel>(dialogLabel), text);
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::selectFile(const QString &filename)
	{
		mFileDialog->selectFile(filename);
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::selectNameFilter(const QString &filter)
	{
		mFileDialog->selectNameFilter(filter);
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setDefaultSuffix(const QString &defaultSuffix)
	{
		mFileDialog->setDefaultSuffix(defaultSuffix);
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setDirectory(const QString &directory)
	{
		mFileDialog->setDirectory(directory);
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setFilter(Filters filters)
	{
		mFileDialog->setFilter(static_cast<QDir::Filters>(filters));
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setNameFilter(const QString &nameFilter)
	{
		mFileDialog->setNameFilter(nameFilter);
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setNameFilters(const QScriptValue &nameFilters)
	{
		mFileDialog->setNameFilters(Code::arrayParameterToStringList(nameFilters));
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setShowDirectoriesOnly(bool showDirectoriesOnly)
	{
		mFileDialog->setOption(QFileDialog::ShowDirsOnly, showDirectoriesOnly);
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setDontResolveSymlinks(bool dontResolveSymlinks)
	{
		mFileDialog->setOption(QFileDialog::DontResolveSymlinks, dontResolveSymlinks);
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setDontConfirmOverwrite(bool dontConfirmOverwrite)
	{
		mFileDialog->setOption(QFileDialog::DontConfirmOverwrite, dontConfirmOverwrite);
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setDontUseNativeDialog(bool dontUseNativeDialog)
	{
		mFileDialog->setOption(QFileDialog::DontUseNativeDialog, dontUseNativeDialog);
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setReadOnly(bool readOnly)
	{
		mFileDialog->setOption(QFileDialog::ReadOnly, readOnly);
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setHideNameFilterDetails(bool hideNameFilterDetails)
	{
		mFileDialog->setOption(QFileDialog::HideNameFilterDetails, hideNameFilterDetails);
		
		return context()->thisObject();
	}

	QScriptValue FileDialog::setSidebarUrls(const QScriptValue &sidebarUrls)
	{
		QList<QUrl> urls;
		
		foreach(const QString &url, Code::arrayParameterToStringList(sidebarUrls))
			urls.append(QUrl::fromLocalFile(url));
			
		mFileDialog->setSidebarUrls(urls);
		
		return context()->thisObject();
	}

	QString FileDialog::selectedFile() const
	{
		const QStringList &selectedFiles = mFileDialog->selectedFiles();
		
		if(selectedFiles.count() > 0)
			return selectedFiles.at(0);
		
		return QString();
	}
	
	QScriptValue FileDialog::selectedFiles() const
	{
		return Code::stringListToArrayParameter(engine(), mFileDialog->selectedFiles());
	}
	
	QString FileDialog::selectedNameFilter() const
	{
		return mFileDialog->selectedNameFilter();
	}
	
	QScriptValue FileDialog::show()
	{
		mFileDialog->open();

		return context()->thisObject();
	}

	int FileDialog::showModal()
	{
		return mFileDialog->exec();
	}

	void FileDialog::finished(int result)
	{
		if(mOnClosed.isValid())
			mOnClosed.call(mThisObject, QScriptValueList() << result);
	}
}
