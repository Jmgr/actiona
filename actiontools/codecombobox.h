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

#ifndef CODECOMBOBOX_H
#define CODECOMBOBOX_H

#include "actiontools_global.h"
#include "subparameter.h"
#include "abstractcodeeditor.h"

#include <QComboBox>
#include <QItemDelegate>
#include <QStandardItemModel>

namespace ActionTools
{
	class CodeLineEdit;

	class CodeComboBoxDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:
		explicit CodeComboBoxDelegate(QObject *parent = 0) : QItemDelegate(parent)		{}
		void paint(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	};

	class CodeComboBoxModel : public QStandardItemModel
	{
	public:
		explicit CodeComboBoxModel(QObject *parent = 0) : QStandardItemModel(parent)	{}
		Qt::ItemFlags flags(const QModelIndex &index) const;
	};

	class ACTIONTOOLSSHARED_EXPORT CodeComboBox : public QComboBox, public AbstractCodeEditor
	{
		Q_OBJECT
		Q_PROPERTY(bool code READ isCode WRITE setCode)

	public:
		explicit CodeComboBox(QWidget * parent = 0);

		CodeLineEdit *codeLineEdit() const;

		bool isCode() const;
		void setCode(bool code);

		void openEditor(int line = -1, int column = -1);
		void setCompletionModel(QAbstractItemModel *completionModel);

	signals:
		void codeChanged(bool code);

	private:
		Q_DISABLE_COPY(CodeComboBox)
	};
}

#endif // CODECOMBOBOX_H
