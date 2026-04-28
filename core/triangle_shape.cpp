/**********************************************************************
 * Copyright (c) 2020 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <rinvid/core/rinvid_gl.h>
#include <rinvid/core/triangle_shape.h>
#include <rinvid/util/vector3.h>

namespace rinvid
{

TriangleShape::TriangleShape(Vector2f vert1, Vector2f vert2, Vector2f vert3)
{
    vertices_.at(0) = vert1;
    vertices_.at(1) = vert2;
    vertices_.at(2) = vert3;

    calculate_origin();

    update_gl_buffer_data();
}

} // namespace rinvid
