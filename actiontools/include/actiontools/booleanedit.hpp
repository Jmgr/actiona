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
#include "abstractcodeeditor.hpp"

#include <QWidget>

namespace Ui
{
    class BooleanEdit;
}

namespace ActionTools
{
	class SubParameter;

	class ACTIONTOOLSSHARED_EXPORT BooleanEdit : public QWidget, public AbstractCodeEditor
	{
		Q_OBJECT

	public:
		explicit BooleanEdit(QWidget *parent = nullptr);
		~BooleanEdit() override;

		void setText(bool isCode, const QString &text);
		QString text() const;
		bool isCode() const;
		void setCode(bool code);
		void setFromSubParameter(const SubParameter &subParameter);
		void openEditor(int line, int column) override;
		void setCompletionModel(QAbstractItemModel *completionModel) override;
        void setParameterContainer(const ParameterContainer *parameterContainer) override;
        QSet<QString> findVariables() const override;

	private slots:
		void on_switchTextModePushButton_clicked();

	private:
		Ui::BooleanEdit *ui;
	};
}

