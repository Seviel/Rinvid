/**********************************************************************
 * Copyright (c) 2020 - 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "core/include/rinvid_gl.h"

#include "core/include/rinvid_gfx.h"
#include "include/quad_shape.h"
#include "util/include/vector3.h"

namespace rinvid
{

QuadShape::QuadShape(Vector2f top_left, Vector2f top_right, Vector2f bottom_right,
                     Vector2f bottom_left)
{
    vertices_.at(0) = top_left;
    vertices_.at(1) = top_right;
    vertices_.at(2) = bottom_right;
    vertices_.at(3) = bottom_left;

    calculate_origin();

    update_gl_buffer_data();
}

} // namespace rinvid
