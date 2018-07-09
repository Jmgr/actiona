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

#include "codecombobox.h"
#include "codelineedit.h"
#include "codelineeditbutton.h"

#include <QPainter>
#include <QAbstractItemView>

namespace ActionTools
{
	//Taken and adapted from http://qtwiki.org/QCombobox_header_item
	void CodeComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		QString valueString = index.model()->data(index, Qt::DisplayRole).toString();

		if(!(index.model()->flags(index) & Qt::ItemIsEnabled))
		{
			painter->fillRect(option.rect, option.palette.brush(QPalette::Inactive, QPalette::Highlight));
			painter->drawText(option.rect, Qt::AlignLeft | Qt::TextSingleLine, valueString);
		}
		else
			QStyledItemDelegate::paint(painter, option, index);
	}

	Qt::ItemFlags CodeComboBoxModel::flags(const QModelIndex & index) const
	{
		Qt::ItemFlags flags = QStandardItemModel::flags(index);

		if (!index.isValid())
			return flags;

		QString value =	index.data(Qt::UserRole).toString();
		if(value == QLatin1String("header"))
		{
			flags &= ~Qt::ItemIsSelectable;
			flags &= ~Qt::ItemIsEnabled;
		}

		return flags;
	}

    CodeComboBox::CodeComboBox(QWidget *parent) :
		QComboBox(parent)
	{
        delete model();
		setModel(new CodeComboBoxModel(this));
		setItemDelegate(new CodeComboBoxDelegate(this));

        CodeLineEdit *codeLineEdit = new CodeLineEdit(parent);
		codeLineEdit->setEmbedded(true);
		
		setLineEdit(codeLineEdit);
		setEditable(true);
		setInsertPolicy(QComboBox::NoInsert);

        connect(codeLineEdit, &CodeLineEdit::codeChanged, this, &CodeComboBox::codeChanged);

		addActions(codeLineEdit->actions());
		
		setMinimumWidth(sizeHint().width() + codeLineEdit->codeButton()->maximumWidth() + codeLineEdit->editorButton()->maximumWidth() + 50);
	}

	CodeLineEdit *CodeComboBox::codeLineEdit() const
	{
		return qobject_cast<CodeLineEdit *>(lineEdit());
	}

	bool CodeComboBox::isCode() const
	{
		return codeLineEdit()->isCode();
	}

	void CodeComboBox::setCode(bool code)
	{
		codeLineEdit()->setCode(code);
	}

	void CodeComboBox::openEditor(int line, int column)
	{
		codeLineEdit()->openEditor(line, column);
	}
	
	void CodeComboBox::setCompletionModel(QAbstractItemModel *completionModel)
	{
        codeLineEdit()->setCompletionModel(completionModel);
    }

    void CodeComboBox::setParameterContainer(const ParameterContainer *parameterContainer)
    {
        codeLineEdit()->setParameterContainer(parameterContainer);
    }

    QSet<QString> CodeComboBox::findVariables() const
    {
        return codeLineEdit()->findVariables();
    }
}
