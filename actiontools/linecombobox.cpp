/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#include "linecombobox.h"

namespace ActionTools
{
    LineComboBox::LineComboBox(const QStringList &labels, int lineCount, QWidget *parent)
        : CodeComboBox(parent)
	{
		setup(labels, lineCount);
	}
	
	void LineComboBox::setup(const QStringList &labels, int lineCount)
	{
		clear();
		
		if(labels.size() > 0)
		{
			addItem(QObject::tr("Labels"), "header");
			addItems(labels);
		}

		if(lineCount > 0)
		{
			addItem(QObject::tr("Lines"), "header");
			for(int i=0 ; i < lineCount ; ++i)
				addItem(QString("%1").arg(i + 1, 3, 10, QChar('0')));
		}

		if(labels.size() > 0)
			setCurrentIndex(1);
		else if(lineCount > 0)
			setCurrentIndex(1);
	}
}
