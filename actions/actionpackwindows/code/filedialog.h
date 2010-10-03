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

#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include "window.h"

#include <QFileDialog>
#include <QDir>

namespace Code
{
	class FileDialog : public Window
	{
		Q_OBJECT
		Q_ENUMS(AcceptMode)
		Q_ENUMS(FileMode)
		Q_ENUMS(ViewMode)
		Q_ENUMS(DialogLabel)
		Q_ENUMS(Filters)
		
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
			Directory = QFileDialog::Directory,
			ExistingFiles = QFileDialog::ExistingFiles
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
		~FileDialog();
		
	public slots:
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
		
	private:
		QFileDialog *mFileDialog;
		QScriptValue mOnClosed;
		QScriptValue mOnCurrentChanged;
		QScriptValue mOnDirectoryEntered;
		QScriptValue mOnFileSelected;
		QScriptValue mOnFilesSelected;
		QScriptValue mOnFilterSelected;
		QScriptValue mThisObject;
	};
}

#endif // FILEDIALOG_H
