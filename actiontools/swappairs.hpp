#pragma once

#include <map>

template <class T1, class T2>
std::map<T2, T1> swapPairs(std::map<T1, T2> m)
{
    std::map<T2, T1> m1;

    for(auto&& item : m)
    {
        m1.emplace(item.second, item.first);
    }

    return m1;
}
