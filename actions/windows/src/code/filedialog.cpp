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

#include "filedialog.hpp"
#include "actiontools/scriptengine.hpp"

#include <QJSValueIterator>
#include <QUrl>

namespace Code
{
	FileDialog::FileDialog()
		: BaseWindow(),
		mFileDialog(new QFileDialog)
	{
        mFileDialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);

		setWidget(mFileDialog);
		
        connect(mFileDialog, &QFileDialog::finished, this, &FileDialog::finished);
        connect(mFileDialog, &QFileDialog::currentChanged, this, &FileDialog::currentChanged);
        connect(mFileDialog, &QFileDialog::directoryEntered, this, &FileDialog::directoryEntered);
        connect(mFileDialog, &QFileDialog::fileSelected, this, &FileDialog::fileSelected);
        connect(mFileDialog, &QFileDialog::filesSelected, this, &FileDialog::filesSelected);
        connect(mFileDialog, &QFileDialog::filterSelected, this, &FileDialog::filterSelected);
	}

    FileDialog::FileDialog(const QJSValue &parameters)
        : FileDialog()
    {
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return;
        }

        setupConstructorParameters(parameters);

        QJSValueIterator it(parameters);

        while(it.hasNext())
        {
            it.next();

            if(it.name() == QLatin1String("acceptMode"))
                mFileDialog->setAcceptMode(static_cast<QFileDialog::AcceptMode>(it.value().toInt()));
            else if(it.name() == QLatin1String("fileMode"))
                mFileDialog->setFileMode(static_cast<QFileDialog::FileMode>(it.value().toInt()));
            else if(it.name() == QLatin1String("viewMode"))
                mFileDialog->setViewMode(static_cast<QFileDialog::ViewMode>(it.value().toInt()));
            else if(it.name() == QLatin1String("selectedFile"))
                mFileDialog->selectFile(it.value().toString());
            else if(it.name() == QLatin1String("selectedNameFilter"))
                mFileDialog->selectNameFilter(it.value().toString());
            else if(it.name() == QLatin1String("defaultSuffix"))
                mFileDialog->setDefaultSuffix(it.value().toString());
            else if(it.name() == QLatin1String("directory"))
                mFileDialog->setDirectory(it.value().toString());
            else if(it.name() == QLatin1String("filter"))
                mFileDialog->setFilter(static_cast<QDir::Filter>(it.value().toInt()));
            else if(it.name() == QLatin1String("nameFilter"))
                mFileDialog->setNameFilter(it.value().toString());
            else if(it.name() == QLatin1String("nameFilters"))
                mFileDialog->setNameFilters(arrayParameterToStringList(it.value()));
            else if(it.name() == QLatin1String("showDirectoriesOnly"))
                mFileDialog->setOption(QFileDialog::ShowDirsOnly, it.value().toBool());
            else if(it.name() == QLatin1String("dontResolveSymlinks"))
                mFileDialog->setOption(QFileDialog::DontResolveSymlinks, it.value().toBool());
            else if(it.name() == QLatin1String("dontConfirmOverwrite"))
                mFileDialog->setOption(QFileDialog::DontConfirmOverwrite, it.value().toBool());
            else if(it.name() == QLatin1String("dontUseNativeDialog"))
                mFileDialog->setOption(QFileDialog::DontUseNativeDialog, it.value().toBool());
            else if(it.name() == QLatin1String("readOnly"))
                mFileDialog->setOption(QFileDialog::ReadOnly, it.value().toBool());
            else if(it.name() == QLatin1String("hideNameFilterDetails"))
                mFileDialog->setOption(QFileDialog::HideNameFilterDetails, it.value().toBool());
            else if(it.name() == QLatin1String("sidebarUrls"))
            {
                QList<QUrl> urls;

                const auto surls = arrayParameterToStringList(it.value());
                for(const QString &url: surls)
                    urls.append(QUrl::fromLocalFile(url));

                mFileDialog->setSidebarUrls(urls);
            }
            else if(it.name() == QLatin1String("onClosed"))
                mOnClosed = it.value();
            else if(it.name() == QLatin1String("onCurrentChanged"))
                mOnCurrentChanged = it.value();
            else if(it.name() == QLatin1String("onDirectoryEntered"))
                mOnDirectoryEntered = it.value();
            else if(it.name() == QLatin1String("onFileSelected"))
                mOnFileSelected = it.value();
            else if(it.name() == QLatin1String("onFilesSelected"))
                mOnFilesSelected = it.value();
            else if(it.name() == QLatin1String("onFilterSelected"))
                mOnFilterSelected = it.value();
        }
    }
	
	FileDialog::~FileDialog()
	{
		delete mFileDialog;
	}
	
    FileDialog *FileDialog::setAcceptMode(AcceptMode acceptMode)
	{
		mFileDialog->setAcceptMode(static_cast<QFileDialog::AcceptMode>(acceptMode));
		
        return this;
	}

    FileDialog *FileDialog::setFileMode(FileMode fileMode)
	{
		mFileDialog->setFileMode(static_cast<QFileDialog::FileMode>(fileMode));
				
        return this;
	}

    FileDialog *FileDialog::setViewMode(ViewMode viewMode)
	{
		mFileDialog->setViewMode(static_cast<QFileDialog::ViewMode>(viewMode));
		
        return this;
	}

    FileDialog *FileDialog::setLabelText(DialogLabel dialogLabel, const QString &text)
	{
		mFileDialog->setLabelText(static_cast<QFileDialog::DialogLabel>(dialogLabel), text);
		
        return this;
	}

    FileDialog *FileDialog::selectFile(const QString &filename)
	{
		mFileDialog->selectFile(filename);
		
        return this;
	}

    FileDialog *FileDialog::selectNameFilter(const QString &filter)
	{
		mFileDialog->selectNameFilter(filter);
		
        return this;
	}

    FileDialog *FileDialog::setDefaultSuffix(const QString &defaultSuffix)
	{
		mFileDialog->setDefaultSuffix(defaultSuffix);
		
        return this;
	}

    FileDialog *FileDialog::setDirectory(const QString &directory)
	{
		mFileDialog->setDirectory(directory);
		
        return this;
	}

    FileDialog *FileDialog::setFilter(Filters filters)
	{
		mFileDialog->setFilter(static_cast<QDir::Filters>(filters));
		
        return this;
	}

    FileDialog *FileDialog::setNameFilter(const QString &nameFilter)
	{
		mFileDialog->setNameFilter(nameFilter);
		
        return this;
	}

    FileDialog *FileDialog::setNameFilters(const QJSValue &nameFilters)
	{
		mFileDialog->setNameFilters(arrayParameterToStringList(nameFilters));
		
        return this;
	}

    FileDialog *FileDialog::setShowDirectoriesOnly(bool showDirectoriesOnly)
	{
		mFileDialog->setOption(QFileDialog::ShowDirsOnly, showDirectoriesOnly);
		
        return this;
	}

    FileDialog *FileDialog::setDontResolveSymlinks(bool dontResolveSymlinks)
	{
		mFileDialog->setOption(QFileDialog::DontResolveSymlinks, dontResolveSymlinks);
		
        return this;
	}

    FileDialog *FileDialog::setDontConfirmOverwrite(bool dontConfirmOverwrite)
	{
		mFileDialog->setOption(QFileDialog::DontConfirmOverwrite, dontConfirmOverwrite);
		
        return this;
	}

    FileDialog *FileDialog::setDontUseNativeDialog(bool dontUseNativeDialog)
	{
		mFileDialog->setOption(QFileDialog::DontUseNativeDialog, dontUseNativeDialog);
		
        return this;
	}

    FileDialog *FileDialog::setReadOnly(bool readOnly)
	{
		mFileDialog->setOption(QFileDialog::ReadOnly, readOnly);
		
        return this;
	}

    FileDialog *FileDialog::setHideNameFilterDetails(bool hideNameFilterDetails)
	{
		mFileDialog->setOption(QFileDialog::HideNameFilterDetails, hideNameFilterDetails);
		
        return this;
	}

    FileDialog *FileDialog::setSidebarUrls(const QJSValue &sidebarUrls)
	{
		QList<QUrl> urls;
		
        const auto surls = arrayParameterToStringList(sidebarUrls);
        for(const QString &url: surls)
			urls.append(QUrl::fromLocalFile(url));
			
		mFileDialog->setSidebarUrls(urls);
		
        return this;
	}

    QString FileDialog::selectedFile() const
	{
		const QStringList &selectedFiles = mFileDialog->selectedFiles();
		
		if(selectedFiles.count() > 0)
			return selectedFiles.at(0);
		
		return QString();
	}
	
	QJSValue FileDialog::selectedFiles() const
	{
        return ActionTools::ScriptEngine::current()->toScriptValue(mFileDialog->selectedFiles());
	}
	
	QString FileDialog::selectedNameFilter() const
	{
		return mFileDialog->selectedNameFilter();
	}
	
    FileDialog *FileDialog::show()
	{
		mFileDialog->open();

        return this;
	}

	int FileDialog::showModal()
	{
		return mFileDialog->exec();
	}

    void FileDialog::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<FileDialog*>("const FileDialog *");

        CodeClass::registerClass<FileDialog>(QStringLiteral("FileDialog"), scriptEngine);
    }

	void FileDialog::finished(int result)
	{
        if(!mOnClosed.isUndefined())
            mOnClosed.call(QJSValueList() << result);
	}

	void FileDialog::currentChanged(const QString &path)
	{
        if(!mOnCurrentChanged.isUndefined())
            mOnCurrentChanged.call(QJSValueList() << path);
	}

	void FileDialog::directoryEntered(const QString &directory)
	{
        if(!mOnDirectoryEntered.isUndefined())
            mOnDirectoryEntered.call(QJSValueList() << directory);
	}

	void FileDialog::fileSelected(const QString &file)
	{
        if(!mOnFileSelected.isUndefined())
            mOnFileSelected.call(QJSValueList() << file);
	}

	void FileDialog::filesSelected(const QStringList &files)
	{
        if(!mOnFilesSelected.isUndefined())
            mOnFilesSelected.call(QJSValueList() << ActionTools::ScriptEngine::current()->toScriptValue(files));
	}

	void FileDialog::filterSelected(const QString &filter)
	{
        if(!mOnFilterSelected.isUndefined())
            mOnFilterSelected.call(QJSValueList() << filter);
	}
}
