/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef DATA_TYPES_INCLUDE_RECT_H
#define DATA_TYPES_INCLUDE_RECT_H

#include <cstdint>

#include "util/include/vector2.h"

namespace rinvid
{

/// @brief A plain data struct intended to be inherited by complex objects who have rectangle
/// properties.
struct RectPOD
{
  protected:
    Vector2f     position_;
    std::int32_t width_;
    std::int32_t height_;
};

} // namespace rinvid

#endif // DATA_TYPES_INCLUDE_RECT_H
