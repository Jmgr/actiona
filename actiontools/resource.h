/*
    Actionaz
    Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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
#include <QStringList>

namespace ActionTools
{
    class ResourceData : public QSharedData
    {
    public:
        ResourceData() : type(0)                {}
        ResourceData(const ResourceData &other) :
            QSharedData(other),
            data(other.data),
            type(other.type)					{}

        QByteArray data;
        int type;
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
        Resource(const Resource &other)
            : d(other.d)						{}

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

#endif // RESOURCE_H
