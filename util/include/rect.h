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

#include "util/include/vector2.h"

namespace rinvid
{

struct Rect
{
    /**************************************************************************************************
     * @brief Scales rect
     *
     * This is mainly intended to use together with bounding_rect() method on objects that have it,
     * for collision detection, to adjust the size of the bounding box.
     *
     * @param scale_factor how much to scale rect (e.g. 0.5 means rect will be half of its current
     * size)
     *
     * @return Returns reference to itself. Handy for chaining function calls.
     *
     *************************************************************************************************/
    Rect& scale(float scale_factor)
    {
        position.x += (((1.0F - scale_factor) * width) / 2.0F);
        position.y += (((1.0F - scale_factor) * height) / 2.0F);
        width *= scale_factor;
        height *= scale_factor;

        return *this;
    }

    Vector2f     position;
    std::int32_t width;
    std::int32_t height;
};

} // namespace rinvid

#endif // UTIL_RECT_H
