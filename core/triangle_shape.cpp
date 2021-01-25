/**********************************************************************
 * Copyright (c) 2020, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>

#define GL_GLEXT_PROTOTYPES
#include <SFML/OpenGL.hpp>

#include "core/include/rinvid_gfx.h"
#include "include/triangle_shape.h"
#include "util/include/vector3.h"

namespace rinvid
{

TriangleShape::TriangleShape(Vector2 vert1, Vector2 vert2, Vector2 vert3)
{
    vertices_.at(0) = vert1;
    vertices_.at(1) = vert2;
    vertices_.at(2) = vert3;
}

void TriangleShape::draw()
{
    FixedPolygonShape::draw();
    draw_arrays(GL_TRIANGLES);
}

} // namespace rinvid
