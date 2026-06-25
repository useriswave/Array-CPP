#pragma once

#include <random>

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

