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

#include "actiontools_global.hpp"

#include <QDialog>

namespace Ui
{
	class CodeEditorDialog;
}

class QAbstractItemModel;
class QMenu;

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT CodeEditorDialog : public QDialog
	{
		Q_OBJECT
	public:
        CodeEditorDialog(QAbstractItemModel *completionModel, QMenu *variablesMenu, QMenu *resourcesMenu, QWidget *parent = nullptr);
		~CodeEditorDialog() override ;

		void setText(const QString &text);
		void setCode(bool code);
		void setAllowTextCodeChange(bool allowTextCodeChange);
		void setCurrentLine(int line);
		void setCurrentColumn(int column);

		QString text() const;
		bool isCode() const;

	public slots:
		void accept() override ;

	private slots:
		void on_codePushButton_toggled(bool checked);
        void on_insertPushButton_clicked();
		void on_checkSyntax_clicked();
		void swapCode();
        void insertVariable(QAction *action);

    private:
        void insertVariable(const QString &variable);
		void showSyntaxCheckError();
		void moveCursorToLine(int line);

		Ui::CodeEditorDialog *ui;
        QMenu *mVariablesMenu;
        QMenu *mResourcesMenu;

		Q_DISABLE_COPY(CodeEditorDialog)
	};
}

