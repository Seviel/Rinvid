/**********************************************************************
 * Copyright (c) 2020 - 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef UTIL_VECTOR3_H
#define UTIL_VECTOR3_H

namespace rinvid
{

template <typename T>
struct Vector3
{
    Vector3()
    {
        static_assert(std::is_arithmetic<T>::value == true);
    }

    Vector3(T X, T Y, T Z) : x{X}, y{Y}, z{Z}
    {
        static_assert(std::is_arithmetic<T>::value == true);
    }

    inline void move(const Vector3<T> move_vector)
    {
        x += move_vector.x;
        y += move_vector.y;
        z += move_vector.z;
    }

    inline void set(const Vector3<T> vector)
    {
        x = vector.x;
        y = vector.y;
        z = vector.z;
    }

    T x;
    T y;
    T z;
};

} // namespace rinvid

#endif // UTIL_VECTOR3_H
