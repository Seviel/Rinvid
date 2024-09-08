/**********************************************************************
 * Copyright (c) 2020 - 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_QUAD_SHAPE_H
#define CORE_INCLUDE_QUAD_SHAPE_H

#include "core/include/fixed_polygon_shape.h"
#include "core/include/shape.h"
#include "util/include/vector2.h"

namespace rinvid
{

/**************************************************************************************************
 * @brief A drawable quad shape.
 *
 *************************************************************************************************/
class QuadShape final : public FixedPolygonShape<4, GL_QUADS>
{
  public:
    /**************************************************************************************************
     * @brief QuadShape constructor
     *
     * @param top_left top left point of the quad
     * @param top_right top right point of the quad
     * @param bottom_right bottom right point of the quad
     * @param bottom_left bottom left point of the quad
     *
     *************************************************************************************************/
    QuadShape(Vector2f top_left, Vector2f top_right, Vector2f bottom_right, Vector2f bottom_left);
};

} // namespace rinvid

#endif // CORE_INCLUDE_QUAD_SHAPE_H
