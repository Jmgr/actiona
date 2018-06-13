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

#include "basewindow.h"

#include <QFileDialog>
#include <QDir>

namespace Code
{
	class FileDialog : public BaseWindow
	{
		Q_OBJECT
		Q_ENUMS(AcceptMode)
		Q_ENUMS(FileMode)
		Q_ENUMS(ViewMode)
		Q_ENUMS(DialogLabel)
		Q_ENUMS(Filters)
		Q_PROPERTY(QScriptValue onClosed READ onClosed WRITE setOnClosed)
		Q_PROPERTY(QScriptValue onCurrentChanged READ onCurrentChanged WRITE setOnCurrentChanged)
		Q_PROPERTY(QScriptValue onDirectoryEntered READ onDirectoryEntered WRITE setOnDirectoryEntered)
		Q_PROPERTY(QScriptValue onFileSelected READ onFileSelected WRITE setOnFileSelected)
		Q_PROPERTY(QScriptValue onFilesSelected READ onFilesSelected WRITE setOnFilesSelected)
		Q_PROPERTY(QScriptValue onFilterSelected READ onFilterSelected WRITE setOnFilterSelected)
		
	public:
		enum AcceptMode
		{
			Open = QFileDialog::AcceptOpen,
			Save = QFileDialog::AcceptSave
		};
		enum FileMode
		{
			AnyFile = QFileDialog::AnyFile,
			ExistingFile = QFileDialog::ExistingFile,
			ExistingFiles = QFileDialog::ExistingFiles,
			Directory = QFileDialog::Directory
		};
		enum ViewMode
		{
			Detail = QFileDialog::Detail,
			List = QFileDialog::List
		};
		enum DialogLabel
		{
			LookIn = QFileDialog::LookIn,
			FileName = QFileDialog::FileName,
			FileType = QFileDialog::FileType,
			Accept = QFileDialog::Accept,
			Reject = QFileDialog::Reject
		};
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
		
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		
		FileDialog();
		~FileDialog() override;
		
		void setOnClosed(const QScriptValue &onClosed)						{ mOnClosed = onClosed; }
		void setOnCurrentChanged(const QScriptValue &onCurrentChanged)		{ mOnCurrentChanged = onCurrentChanged; }
		void setOnDirectoryEntered(const QScriptValue &onDirectoryEntered)	{ mOnDirectoryEntered = onDirectoryEntered; }
		void setOnFileSelected(const QScriptValue &onFileSelected)			{ mOnFileSelected = onFileSelected; }
		void setOnFilesSelected(const QScriptValue &onFilesSelected)		{ mOnFilesSelected = onFilesSelected; }
		void setOnFilterSelected(const QScriptValue &onFilterSelected)		{ mOnFilterSelected = onFilterSelected; }
		
		QScriptValue onClosed() const										{ return mOnClosed; }
		QScriptValue onCurrentChanged() const								{ return mOnCurrentChanged; }
		QScriptValue onDirectoryEntered() const								{ return mOnDirectoryEntered; }
		QScriptValue onFileSelected() const									{ return mOnFileSelected; }
		QScriptValue onFilesSelected() const								{ return mOnFilesSelected; }
		QScriptValue onFilterSelected() const								{ return mOnFilterSelected; }
		
	public slots:
		QString toString() const override					{ return QStringLiteral("FileDialog"); }
		QScriptValue setAcceptMode(AcceptMode acceptMode);
		QScriptValue setFileMode(FileMode fileMode);
		QScriptValue setViewMode(ViewMode viewMode);
		QScriptValue setLabelText(DialogLabel dialogLabel, const QString &text);
		QScriptValue selectFile(const QString &filename);
		QScriptValue selectNameFilter(const QString &filter);
		QScriptValue setDefaultSuffix(const QString &defaultSuffix);
		QScriptValue setDirectory(const QString &directory);
		QScriptValue setFilter(Filters filters);
		QScriptValue setNameFilter(const QString &nameFilter);
		QScriptValue setNameFilters(const QScriptValue &nameFilters);
		QScriptValue setShowDirectoriesOnly(bool showDirectoriesOnly);
		QScriptValue setDontResolveSymlinks(bool dontResolveSymlinks);
		QScriptValue setDontConfirmOverwrite(bool dontConfirmOverwrite);
		QScriptValue setDontUseNativeDialog(bool dontUseNativeDialog);
		QScriptValue setReadOnly(bool readOnly);
		QScriptValue setHideNameFilterDetails(bool hideNameFilterDetails);
		QScriptValue setSidebarUrls(const QScriptValue &sidebarUrls);
		QString selectedFile() const;
		QScriptValue selectedFiles() const;
		QString selectedNameFilter() const;
		QScriptValue show();
		int showModal();
		
	private slots:
		void finished(int result);
		void currentChanged(const QString &path);
		void directoryEntered(const QString &directory);
		void fileSelected(const QString &file);
		void filesSelected(const QStringList &files);
		void filterSelected(const QString &filter);
		
	private:
		QFileDialog *mFileDialog;
		QScriptValue mOnClosed;
		QScriptValue mOnCurrentChanged;
		QScriptValue mOnDirectoryEntered;
		QScriptValue mOnFileSelected;
		QScriptValue mOnFilesSelected;
		QScriptValue mOnFilterSelected;
	};
}

