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
#include "subparameter.hpp"
#include "abstractcodeeditor.hpp"

#include <QDateTimeEdit>

namespace ActionTools
{
	class CodeLineEdit;

	class ACTIONTOOLSSHARED_EXPORT CodeDateTimeEdit : public QDateTimeEdit, public AbstractCodeEditor
	{
		Q_OBJECT

	public:
		explicit CodeDateTimeEdit(QWidget *parent = nullptr);

		CodeLineEdit *codeLineEdit() const;
		bool isCode() const;
		void setCode(bool code);

		void setFromSubParameter(const SubParameter &subParameter);

		void openEditor(int line = -1, int column = -1) override;
		void setCompletionModel(QAbstractItemModel *completionModel) override;
        void setParameterContainer(const ParameterContainer *parameterContainer) override;
         QSet<QString> findVariables() const override;

	private slots:
		void codeChanged(bool code);

	private:
		QValidator::State validate(QString &text, int &pos) const override;
		QString textFromDateTime(const QDateTime &dateTime) const override;
		void contextMenuEvent(QContextMenuEvent *event) override;

		Q_DISABLE_COPY(CodeDateTimeEdit)
	};
}

