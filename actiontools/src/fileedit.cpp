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

#include "actiontools/fileedit.hpp"
#include "ui_fileedit.h"

#include <QFileDialog>

namespace ActionTools
{
	FileEdit::FileEdit(QWidget *parent)
		: QWidget(parent),
		ui(new Ui::FileEdit)
		
	{
		ui->setupUi(this);
	}

	FileEdit::~FileEdit()
	{
		delete ui;
	}

	void FileEdit::setText(const QString &text)
	{
		ui->path->setText(text);
	}

	QString FileEdit::text() const
	{
		return ui->path->text();
	}

	CodeLineEdit *FileEdit::codeLineEdit() const
	{
		return ui->path;
	}

	bool FileEdit::isCode() const
	{
		return ui->path->isCode();
	}

	void FileEdit::setCode(bool code)
	{
		ui->path->setCode(code);
	}

	void FileEdit::setFromSubParameter(const SubParameter &subParameter)
	{
		ui->path->setFromSubParameter(subParameter);
	}

	void FileEdit::openEditor(int line, int column)
	{
		ui->path->openEditor(line, column);
	}
	
	void FileEdit::setCompletionModel(QAbstractItemModel *completionModel)
	{
        codeLineEdit()->setCompletionModel(completionModel);
    }

    void FileEdit::setParameterContainer(const ParameterContainer *parameterContainer)
    {
        codeLineEdit()->setParameterContainer(parameterContainer);
    }

    QSet<QString> FileEdit::findVariables() const
    {
        return codeLineEdit()->findVariables();
    }

	void FileEdit::on_browse_clicked()
	{
		QFileDialog fileDialog(this, mCaption, mDirectory, mFilter);

        fileDialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);

		switch(mMode)
		{
		case FileOpen:
			fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
			fileDialog.setFileMode(QFileDialog::ExistingFile);
			break;
		case FileSave:
			fileDialog.setAcceptMode(QFileDialog::AcceptSave);
			fileDialog.setFileMode(QFileDialog::AnyFile);
			break;
		case DirectoryOpen:
			fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
			fileDialog.setFileMode(QFileDialog::Directory);
			fileDialog.setOption(QFileDialog::ShowDirsOnly);
			break;
		case DirectorySave:
			fileDialog.setAcceptMode(QFileDialog::AcceptSave);
			fileDialog.setFileMode(QFileDialog::Directory);
			fileDialog.setOption(QFileDialog::ShowDirsOnly);
			break;
		}

		if(fileDialog.exec())
		{
			QStringList fileNames = fileDialog.selectedFiles();
			if(fileNames.size() > 0)
				ui->path->setText(fileNames.at(0));
		}
	}
}
