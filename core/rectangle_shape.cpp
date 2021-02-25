/**********************************************************************
 * Copyright (c) 2020 - 2021, Filip Vasiljevic
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
#include "include/rectangle_shape.h"
#include "util/include/vector3.h"

namespace rinvid
{

RectangleShape::RectangleShape(Vector2<float> center, float width, float height)
{
    vertices_.at(0) = Vector2<float>{center.x - width / 2.0F, center.y - height / 2.0F};
    vertices_.at(1) = Vector2<float>{center.x + width / 2.0F, center.y - height / 2.0F};
    vertices_.at(2) = Vector2<float>{center.x + width / 2.0F, center.y + height / 2.0F};
    vertices_.at(3) = Vector2<float>{center.x - width / 2.0F, center.y + height / 2.0F};
}

void RectangleShape::draw()
{
    FixedPolygonShape::draw();
    draw_arrays(GL_QUADS);
}

} // namespace rinvid
