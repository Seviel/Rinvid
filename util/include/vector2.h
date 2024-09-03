/**********************************************************************
 * Copyright (c) 2020 - 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef UTIL_VECTOR2_H
#define UTIL_VECTOR2_H

#include <type_traits>

namespace rinvid
{

template <typename T>
struct Vector2
{
    Vector2() : x{}, y{}
    {
        static_assert(std::is_arithmetic<T>::value == true);
    }

    Vector2(T X, T Y) : x{X}, y{Y}
    {
        static_assert(std::is_arithmetic<T>::value == true);
    }

    inline void move(const Vector2<T> move_vector)
    {
        x += move_vector.x;
        y += move_vector.y;
    }

    inline void set(const Vector2<T> vector)
    {
        x = vector.x;
        y = vector.y;
    }

    T x;
    T y;
};

} // namespace rinvid

#endif // UTIL_VECTOR2_H
