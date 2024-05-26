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

#include "basewindow.hpp"

#include <QFileDialog>
#include <QDir>

namespace Code
{
	class FileDialog : public BaseWindow
	{
        Q_OBJECT
		Q_PROPERTY(QJSValue onClosed READ onClosed WRITE setOnClosed)
		Q_PROPERTY(QJSValue onCurrentChanged READ onCurrentChanged WRITE setOnCurrentChanged)
		Q_PROPERTY(QJSValue onDirectoryEntered READ onDirectoryEntered WRITE setOnDirectoryEntered)
		Q_PROPERTY(QJSValue onFileSelected READ onFileSelected WRITE setOnFileSelected)
		Q_PROPERTY(QJSValue onFilesSelected READ onFilesSelected WRITE setOnFilesSelected)
		Q_PROPERTY(QJSValue onFilterSelected READ onFilterSelected WRITE setOnFilterSelected)
		
	public:
		enum AcceptMode
		{
			Open = QFileDialog::AcceptOpen,
			Save = QFileDialog::AcceptSave
		};
        Q_ENUM(AcceptMode)
		enum FileMode
		{
			AnyFile = QFileDialog::AnyFile,
			ExistingFile = QFileDialog::ExistingFile,
			ExistingFiles = QFileDialog::ExistingFiles,
			Directory = QFileDialog::Directory
		};
        Q_ENUM(FileMode)
		enum ViewMode
		{
			Detail = QFileDialog::Detail,
			List = QFileDialog::List
		};
        Q_ENUM(ViewMode)
		enum DialogLabel
		{
			LookIn = QFileDialog::LookIn,
			FileName = QFileDialog::FileName,
			FileType = QFileDialog::FileType,
			Accept = QFileDialog::Accept,
			Reject = QFileDialog::Reject
		};
        Q_ENUM(DialogLabel)
		enum Filters
		{
			Dirs = QDir::Dirs,
			AllDirs = QDir::AllDirs,
			Files = QDir::Files,
			Drives = QDir::Drives,
			NoSymLinks = QDir::NoSymLinks,
			NoDotAndDotDot = QDir::NoDotAndDotDot,
			NoDot = QDir::NoDot,
			NoDotDot = QDir::NoDotDot,
			AllEntries = QDir::AllEntries,
			Readable = QDir::Readable,
			Writable = QDir::Writable,
			Executable = QDir::Executable,
			Modified = QDir::Modified,
			Hidden = QDir::Hidden,
			System = QDir::System,
			CaseSensitive = QDir::CaseSensitive
		};
        Q_ENUM(Filters)
		
        Q_INVOKABLE FileDialog();
        Q_INVOKABLE FileDialog(const QJSValue &parameters);
		~FileDialog() override;
		
		void setOnClosed(const QJSValue &onClosed)						{ mOnClosed = onClosed; }
		void setOnCurrentChanged(const QJSValue &onCurrentChanged)		{ mOnCurrentChanged = onCurrentChanged; }
		void setOnDirectoryEntered(const QJSValue &onDirectoryEntered)	{ mOnDirectoryEntered = onDirectoryEntered; }
		void setOnFileSelected(const QJSValue &onFileSelected)			{ mOnFileSelected = onFileSelected; }
		void setOnFilesSelected(const QJSValue &onFilesSelected)		{ mOnFilesSelected = onFilesSelected; }
		void setOnFilterSelected(const QJSValue &onFilterSelected)		{ mOnFilterSelected = onFilterSelected; }
		
		QJSValue onClosed() const										{ return mOnClosed; }
		QJSValue onCurrentChanged() const								{ return mOnCurrentChanged; }
		QJSValue onDirectoryEntered() const								{ return mOnDirectoryEntered; }
		QJSValue onFileSelected() const									{ return mOnFileSelected; }
		QJSValue onFilesSelected() const								{ return mOnFilesSelected; }
		QJSValue onFilterSelected() const								{ return mOnFilterSelected; }
		
        Q_INVOKABLE QString toString() const override					{ return QStringLiteral("FileDialog"); }
        Q_INVOKABLE FileDialog *setAcceptMode(AcceptMode acceptMode);
        Q_INVOKABLE FileDialog *setFileMode(FileMode fileMode);
        Q_INVOKABLE FileDialog *setViewMode(ViewMode viewMode);
        Q_INVOKABLE FileDialog *setLabelText(DialogLabel dialogLabel, const QString &text);
        Q_INVOKABLE FileDialog *selectFile(const QString &filename);
        Q_INVOKABLE FileDialog *selectNameFilter(const QString &filter);
        Q_INVOKABLE FileDialog *setDefaultSuffix(const QString &defaultSuffix);
        Q_INVOKABLE FileDialog *setDirectory(const QString &directory);
        Q_INVOKABLE FileDialog *setFilter(Filters filters);
        Q_INVOKABLE FileDialog *setNameFilter(const QString &nameFilter);
        Q_INVOKABLE FileDialog *setNameFilters(const QJSValue &nameFilters);
        Q_INVOKABLE FileDialog *setShowDirectoriesOnly(bool showDirectoriesOnly);
        Q_INVOKABLE FileDialog *setDontResolveSymlinks(bool dontResolveSymlinks);
        Q_INVOKABLE FileDialog *setDontConfirmOverwrite(bool dontConfirmOverwrite);
        Q_INVOKABLE FileDialog *setDontUseNativeDialog(bool dontUseNativeDialog);
        Q_INVOKABLE FileDialog *setReadOnly(bool readOnly);
        Q_INVOKABLE FileDialog *setHideNameFilterDetails(bool hideNameFilterDetails);
        Q_INVOKABLE FileDialog *setSidebarUrls(const QJSValue &sidebarUrls);
        Q_INVOKABLE QString selectedFile() const;
        Q_INVOKABLE QJSValue selectedFiles() const;
        Q_INVOKABLE QString selectedNameFilter() const;
        Q_INVOKABLE FileDialog *show();
        Q_INVOKABLE int showModal();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
		
	private slots:
		void finished(int result);
		void currentChanged(const QString &path);
		void directoryEntered(const QString &directory);
		void fileSelected(const QString &file);
		void filesSelected(const QStringList &files);
		void filterSelected(const QString &filter);
		
	private:
		QFileDialog *mFileDialog;
		QJSValue mOnClosed;
		QJSValue mOnCurrentChanged;
		QJSValue mOnDirectoryEntered;
		QJSValue mOnFileSelected;
		QJSValue mOnFilesSelected;
		QJSValue mOnFilterSelected;
	};
}

