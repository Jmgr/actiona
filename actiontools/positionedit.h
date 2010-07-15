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

#ifndef POSITIONEDIT_H
#define POSITIONEDIT_H

#include "actiontools_global.h"
#include "subparameter.h"
#include "abstractcodeeditor.h"

#include <QWidget>

namespace Ui
{
	class PositionEdit;
}

namespace ActionTools
{
	class CodeLineEdit;

	class ACTIONTOOLSSHARED_EXPORT PositionEdit : public QWidget, public AbstractCodeEditor
	{
		Q_OBJECT

	public:
		explicit PositionEdit(QWidget *parent = 0);
		~PositionEdit();

		void setText(const QString &text);
		QString text() const;
		CodeLineEdit *codeLineEdit() const;
		bool isCode() const;
		void setCode(bool code);
		void setFromSubParameter(const SubParameter &subParameter);
		void openEditor(int line = -1, int column = -1);
		void setCompletionModel(QAbstractItemModel *completionModel);
		
	signals:
		void positionChosen(QPoint position);
		
	public slots:
		void setPosition(QPoint position);

	private slots:
		void on_choose_positionChosen(QPoint position);
		void on_position_codeChanged(bool code);

	private:
		Ui::PositionEdit *ui;
	};
}

#endif // POSITIONEDIT_H
