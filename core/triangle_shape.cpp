/**********************************************************************
 * Copyright (c) 2020 - 2022, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/triangle_shape.h"
#include "core/include/rinvid_gfx.h"
#include "core/include/rinvid_gl.h"
#include "util/include/vector3.h"

namespace rinvid
{

TriangleShape::TriangleShape(Vector2<float> vert1, Vector2<float> vert2, Vector2<float> vert3)
{
    vertices_.at(0) = vert1;
    vertices_.at(1) = vert2;
    vertices_.at(2) = vert3;

    calculate_origin();

    update_gl_buffer_data();
}

void TriangleShape::draw()
{
    FixedPolygonShape::draw();
    draw_arrays(GL_TRIANGLES);
}

void TriangleShape::draw(double delta_time)
{
    /// This suppresses unused parameter warning. This method needs to be overriden because
    /// FixedPolygonShape inherits Drawable interface which declares this version of draw as pure
    /// virtual function.
    (void)delta_time;

    draw();
}

} // namespace rinvid
