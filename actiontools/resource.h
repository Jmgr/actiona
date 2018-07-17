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

#pragma once

#include "actiontools_global.h"

#include <QSharedData>
#include <QByteArray>
#include <QStringList>

namespace ActionTools
{
    class ResourceData : public QSharedData
    {
    public:
        ResourceData() = default;
        ResourceData(const ResourceData &other) = default;

        QByteArray data;
        int type{0};
    };

    class ACTIONTOOLSSHARED_EXPORT Resource
    {
    public:
        enum Type
        {
            BinaryType,
            TextType,
            ImageType,

            TypeCount
        };

        Resource()
            : d(new ResourceData())
        {
        }
        Resource(const QByteArray &data, Type type)
            : d(new ResourceData())
        {
            setData(data);
            setType(type);
        }
        Resource(const Resource &other)         = default;

        const QByteArray &data() const          { return d->data; }
        Type type() const                       { return static_cast<Type>(d->type); }

        void setData(const QByteArray &data)    { d->data = data; }
        void setType(Type type)                 { d->type = type; }

        static QStringList typeNames;

    private:
        QSharedDataPointer<ResourceData> d;
    };

    inline bool operator==(const Resource &first, const Resource &second)
    {
        return (first.type() == second.type() && first.data() == second.data());
    }
}

