#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <type_traits>

namespace Random
{
    inline std::mt19937 mt{std::random_device{}()};

    template <typename T>
    T get(T start, T end)
    {
        std::uniform_int_distribution<T> range(start, end);
        return range(mt);
    }
}

#endif
