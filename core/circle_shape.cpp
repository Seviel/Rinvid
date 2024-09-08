/**********************************************************************
 * Copyright (c) 2020 - 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <cmath>

#include "core/include/rinvid_gfx.h"
#include "core/include/rinvid_gl.h"
#include "include/circle_shape.h"
#include "util/include/math_utils.h"
#include "util/include/vector3.h"

namespace rinvid
{

CircleShape::CircleShape(Vector2f center, float radius)
{
    float angle{};
    for (std::uint32_t i{0}; i < number_of_vertices_; ++i)
    {
        angle           = i * (DEGREES_IN_A_CIRCLE / number_of_vertices_);
        vertices_.at(i) = Vector2f{center.x + std::cos(DEGREES_TO_RADIANS(angle)) * radius,
                                   center.y + std::sin(DEGREES_TO_RADIANS(angle)) * radius};
    }

    calculate_origin();

    update_gl_buffer_data();
}

} // namespace rinvid
