/*
    Actionaz
    Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#ifndef RESOURCE_H
#define RESOURCE_H

#include "actiontools_global.h"

#include <QSharedData>
#include <QByteArray>

namespace ActionTools
{
    enum ResourceType
    {
        BinaryType,
        TextType,
        ImageType
    };

    class ResourceData : public QSharedData
    {
    public:
        ResourceData() : type(BinaryType)       {}
        ResourceData(const ResourceData &other) :
            QSharedData(other),
            data(other.data),
            type(other.type)					{}

        QByteArray data;
        ResourceType type;
    };

    class ACTIONTOOLSSHARED_EXPORT Resource
    {
    public:
        Resource()
            : d(new ResourceData())
        {
        }
        Resource(const QByteArray &data, ResourceType type)
            : d(new ResourceData())
        {
            setData(data);
            setType(type);
        }
        Resource(const Resource &other)
            : d(other.d)						{}

        const QByteArray &data() const          { return d->data; }
        ResourceType type() const               { return d->type; }

        void setData(const QByteArray &data)    { d->data = data; }
        void setType(ResourceType type)         { d->type = type; }

    private:
        QSharedDataPointer<ResourceData> d;
    };
}

#endif // RESOURCE_H
