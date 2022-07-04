/**********************************************************************
 * Copyright (c) 2020 - 2022, Filip Vasiljevic
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
class QuadShape : public FixedPolygonShape<4>
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
    QuadShape(Vector2<float> top_left, Vector2<float> top_right, Vector2<float> bottom_right,
              Vector2<float> bottom_left);

    /**************************************************************************************************
     * @brief Draws the quad.
     *
     *************************************************************************************************/
    virtual void draw() override;

    /**************************************************************************************************
     * @brief Draws the quad.
     *
     * This version of draw() is intended for animated objects, nevertheless needs to be overriden
     * even in objects that can't be animated because it's a pure virtual method.
     *
     * @param delta_time Time passed in seconds since last frame (not used in this case)
     *
     *************************************************************************************************/
    virtual void draw(double delta_time) override;
};

} // namespace rinvid

#endif // CORE_INCLUDE_QUAD_SHAPE_H
