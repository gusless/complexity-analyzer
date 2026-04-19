#pragma once

#include "complexity-enum.hpp"

enum class Complexity {
    n, //O(n)
    n_2, //O(n^2)
    n_3, //O(n^3)
    nlogn, //O(n*log(n))
    logn, //O(log(n))
    _2_n, //O(2^n)
    _3_n, //O(3^n)
    fat_n, //O(n!)
};