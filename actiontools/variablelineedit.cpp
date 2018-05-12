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

#include "variablelineedit.h"
#include "actioninstance.h"

#include <QSet>
#include <QMenu>

namespace ActionTools
{
    VariableLineEdit::VariableLineEdit(QWidget *parent)
        : CodeLineEdit(parent, ActionInstance::NameRegExp)
    {
    }

    QSet<QString> VariableLineEdit::findVariables() const
    {
        QSet<QString> back = CodeLineEdit::findVariables();

        if(!isCode() && !text().isEmpty())
            back.insert(text());

        return back;
    }

    void VariableLineEdit::insertVariable(const QString &variable)
    {
        if(isCode())
            insert(variable);
        else
            setText(variable);
    }

    QMenu *VariableLineEdit::createResourcesMenu(QMenu *parentMenu, bool ignoreMultiline)
    {
        Q_UNUSED(ignoreMultiline)

        //Do not allow inserting resources here, it doen't make any sense since we cannot overwrite resources

        QMenu *resourceMenu = new QMenu(tr("Cannot insert resources here"), parentMenu);
        resourceMenu->setEnabled(false);
		resourceMenu->setIcon(QIcon(QStringLiteral(":/images/resource.png")));

        return resourceMenu;
    }
}
