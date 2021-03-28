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

#include "backend/3rdparty/functional_unwrap.hpp"

#include <QString>

#include <functional>

namespace Backend
{
    namespace detail
    {
        auto defaultChecker = []{ return true; };

        template<typename Func>
        class Implementation final
        {
        public:
            Implementation(const QString &name, Func func):
                mName(name),
                mFunc(func),
                mChecker(defaultChecker)
            {
            }
            Implementation(const QString &name, Func func, std::function<bool()> checker):
                mName(name),
                mFunc(func),
                mChecker(checker)
            {
            }

            operator bool() const
            {
                return mFunc != nullptr;
            }
            bool check() const
            {
                return mChecker();
            }
            std::function<Func> func() const
            {
                return mFunc;
            }

        private:
            QString mName;
            std::function<Func> mFunc;
            std::function<bool()> mChecker;
        };
    }

    inline static const QString windowImpl = QStringLiteral("Windows");
    inline static const QString x11Impl = QStringLiteral("X11");
    inline static const QString dbusGnomeImpl = QStringLiteral("BDus-Gnome");
    inline static const QString qtImpl = QStringLiteral("Qt");
    inline static const QString unixImpl = QStringLiteral("Unix");

    template<typename Func>
    class Feature final
    {
        Q_DISABLE_COPY(Feature)

    public:
        Feature(QString name, Func dummyFunc):
            mName(name),
            mChosenImplementation(QStringLiteral("dummy"), dummyFunc)
        {
        }

        template<typename... Args>
        typename fu::unwrap_function<Func>::return_type operator()(Args&&...args) const
        {
            return mChosenImplementation.func()(std::forward<Args>(args)...);
        }

        void addImplementation(const QString &name, Func implementation)
        {
            mImplementations.emplace_back(name, implementation);
        }
        void addImplementation(const QString &name, Func implementation, std::function<bool()> checker)
        {
            mImplementations.emplace_back(name, implementation, checker);
        }
        void choose()
        {
            for(auto impl: mImplementations)
            {
                if(impl && impl.check())
                {
                    mChosenImplementation = impl;
                    mIsAvailable = true;
                    return;
                }
            }
        }
        bool isAvailable() const
        {
            return mIsAvailable;
        }
        QString info() const
        {
            if(!isAvailable())
                return QStringLiteral("%1: not available").arg(mName);

            return QStringLiteral("%1: %2").arg(mName).arg(mChosenImplementation.mName);
        }

    private:
        QString mName;
        std::vector<detail::Implementation<Func>> mImplementations;
        detail::Implementation<Func> mChosenImplementation;
        bool mIsAvailable{};
    };
}
