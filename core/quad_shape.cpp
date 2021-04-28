/**********************************************************************
 * Copyright (c) 2020 - 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifdef __unix__
#define GL_GLEXT_PROTOTYPES
#include <SFML/OpenGL.hpp>
#else
#include "extern/glad/include/glad/glad.h"
#endif

#include "core/include/rinvid_gfx.h"
#include "include/quad_shape.h"
#include "util/include/vector3.h"

namespace rinvid
{

QuadShape::QuadShape(Vector2<float> top_left, Vector2<float> top_right, Vector2<float> bottom_right,
                     Vector2<float> bottom_left)
{
    vertices_.at(0) = top_left;
    vertices_.at(1) = top_right;
    vertices_.at(2) = bottom_right;
    vertices_.at(3) = bottom_left;

    calculate_origin();
}

void QuadShape::draw()
{
    FixedPolygonShape::draw();
    draw_arrays(GL_QUADS);
}

void QuadShape::draw(double delta_time)
{
    /// This suppresses unused parameter warning. This method needs to be overriden because
    /// FixedPolygonShape inherits Drawable interface which declares this version of draw as pure
    /// virtual function.
    (void)delta_time;

    draw();
}

} // namespace rinvid
