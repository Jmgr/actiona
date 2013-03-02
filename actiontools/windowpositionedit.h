/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#ifndef WINDOWPOSITIONEDIT_H
#define WINDOWPOSITIONEDIT_H

#include "actiontools_global.h"
#include "subparameter.h"
#include "abstractcodeeditor.h"
#include "windowhandle.h"
#include "codelineedit.h"
#include "screenpositiondialog.h"

#include <QWidget>

namespace Ui
{
	class WindowPositionEdit;
}

class ScreenPositionDialog;

namespace ActionTools
{
	class CodeLineEdit;

	class ACTIONTOOLSSHARED_EXPORT WindowPositionEdit : public QWidget, public AbstractCodeEditor
	{
		Q_OBJECT

	public:
		explicit WindowPositionEdit(QWidget *parent = 0);
		~WindowPositionEdit();

		void setText(const QString &text);
		QString text() const;
		CodeLineEdit *codeLineEdit() const;
		bool isCode() const;
		void setCode(bool code);
		void setFromSubParameter(const SubParameter &subParameter);
		void openEditor(int line = -1, int column = -1);
		void setCompletionModel(QAbstractItemModel *completionModel);
		void setChooseByPositionButtonVisible(bool visible);

	signals:
		void positionChosen(QPoint position);

	public slots:
		void setPosition(QPoint position);

	private slots:
		void on_chooseByPosition_positionChosen(QPoint position);
		void on_choose_clicked();
		void on_windowPositionLineEdit_textChanged(const QString &text);
		void on_windowPositionLineEdit_codeChanged(bool code);

	private:
		void updateDialog(const QString &text);

		Ui::WindowPositionEdit *ui;
		ScreenPositionDialog *mScreenPositionDialog;

		Q_DISABLE_COPY(WindowPositionEdit)
	};
}

#endif // WINDOWPOSITIONEDIT_H
