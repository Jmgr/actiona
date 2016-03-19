/*
	Actiona
	Copyright (C) 2005-2016 Jonathan Mercier-Ganady

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

#ifndef CODESPINBOX_H
#define CODESPINBOX_H

#include "actiontools_global.h"
#include "subparameter.h"
#include "abstractcodeeditor.h"

#include <QSpinBox>

namespace ActionTools
{
	class CodeLineEdit;

	class ACTIONTOOLSSHARED_EXPORT CodeSpinBox : public QSpinBox, public AbstractCodeEditor
	{
		Q_OBJECT

	public:
		explicit CodeSpinBox(QWidget *parent = 0);

		CodeLineEdit *codeLineEdit() const;
		bool isCode() const;
		void setCode(bool code);

		void setFromSubParameter(const SubParameter &subParameter);

		void openEditor(int line = -1, int column = -1);
		void setCompletionModel(QAbstractItemModel *completionModel);
        void setParameterContainer(const ParameterContainer *parameterContainer);
        QSet<QString> findVariables() const;

		QString text();

	private slots:
		void codeChanged(bool code);

	private:
		QValidator::State validate(QString &text, int &pos) const;
		QString textFromValue(int value) const;
		void contextMenuEvent(QContextMenuEvent *event);
		
		QString mPrefix;
		QString mSuffix;

		Q_DISABLE_COPY(CodeSpinBox)
	};
}

#endif // CODESPINBOX_H
