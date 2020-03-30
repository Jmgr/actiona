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

#include "actiontools/linecombobox.hpp"
#include "actiontools/script.hpp"
#include "actiontools/scriptlinemodel.hpp"

namespace ActionTools
{
    LineComboBox::LineComboBox(Script &script, QWidget *parent)
        : CodeComboBox(parent),
          mScript{script}
	{
        delete model();
        setModel(mScript.lineModel());
    }

    void LineComboBox::setFromSubParameter(const SubParameter &subParameter)
    {
        setValue(subParameter.isCode(), subParameter.value());
    }

    void LineComboBox::setValue(bool code, const QString &lineOrLabel)
    {
        setCode(code);

        if(isCode())
            setEditText(lineOrLabel);
        else
        {
            auto line = findText(lineOrLabel);
            if(line != -1)
                setCurrentIndex(line);
            else
                setEditText(lineOrLabel);
        }
    }
}
