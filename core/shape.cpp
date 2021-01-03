/**********************************************************************
 * Copyright (c) 2020, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/shape.h"
#include "util/include/color.h"

namespace rinvid
{

Vector2 Shape::get_origin()
{
    return origin_;
}

void Shape::set_color(const Color color)
{
    color_ = color;
}

} // namespace rinvid
