/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef UTIL_RECT_H
#define UTIL_RECT_H

#include <cstdint>

namespace rinvid
{

struct Rect
{
    std::int32_t x;
    std::int32_t y;
    std::int32_t width;
    std::int32_t height;
};

} // namespace rinvid

#endif // UTIL_RECT_H
