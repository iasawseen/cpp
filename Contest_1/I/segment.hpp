//
// Created by ivan on 28.03.17.
//

#include <cstdint>

#ifndef I_SEGMENT_HPP
#define I_SEGMENT_HPP

#endif //I_SEGMENT_HPP

using int_t = uint32_t;

struct point {
    int_t x;
    int_t y;

    point(int_t x, int_t y)
    : x(x), y(y)
    {}
};

struct segment {
    point left;
    point right;

    segment(int_t x, int_t y)
            : x(x), y(y)
    {}

    bool operator<(segment const& other) {
        if
    }
};