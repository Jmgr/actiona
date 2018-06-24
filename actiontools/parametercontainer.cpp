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

#include "parametercontainer.h"
#include "script.h"

#include <QMenu>

namespace ActionTools
{
    ParameterContainer::ParameterContainer(Script *script)
        : mScript(script)
    {
    }

    ParameterContainer::~ParameterContainer() = default;

    QMenu *ParameterContainer::createResourcesMenu(QWidget *parent) const
    {
        if(mScript->resources().isEmpty())
            return nullptr;

        auto back = new QMenu(parent);

        for(const QString &key: mScript->resources().keys())
        {
            const ActionTools::Resource &resource = mScript->resources().value(key);
            switch(resource.type())
            {
            case ActionTools::Resource::BinaryType:
            case ActionTools::Resource::TypeCount:
				back->addAction(QIcon(QStringLiteral(":/images/binary.png")), key);
                break;
            case ActionTools::Resource::TextType:
				back->addAction(QIcon(QStringLiteral(":/images/text.png")), key);
                break;
            case ActionTools::Resource::ImageType:
				back->addAction(QIcon(QStringLiteral(":/images/image.png")), key);
                break;
            }
        }

        return back;
    }
}
